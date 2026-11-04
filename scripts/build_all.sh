#!/usr/bin/env bash
# SV-build (plan §6): build every target in the pinned image and print size + sha256.
# QMK's build directory is build/qmk (see scripts/qmk). Everything under build/ is disposable.
#   scripts/build_all.sh            build all three targets; an unversioned copy (short names +
#                                   SHA256SUMS + README) replaces build/scratch/check/
#   scripts/build_all.sh --release  also make a release: build/firmware/qmk-<QMK version>-<VERSION>/
#                                   (sofle.hex kbd75hs.hex kbd75v2.hex read-only + SHA256SUMS + README
#                                   with the origin). VERSION is the repo's SemVer firmware version (file
#                                   VERSION at HEAD). Clean tree and a submodule on a QMK release tag only.
#                                   If the directory exists, the new build must be identical (then: no-op);
#                                   a different build under the same name is refused (VERSION not bumped?).
#                                   A release is reproducible (rebuild its commit), so it may live in build/.
set -euo pipefail
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel)
. "$root/scripts/toolchain.env"
release=no; [ "${1:-}" = "--release" ] && release=yes

targets=(sofle/rev1 kbdfans/kbd75hs kbdfans/kbd75/rev2)   # keep in sync with qmk.json
declare -A short=([sofle_rev1_sekogan]=sofle [kbdfans_kbd75hs_sekogan]=kbd75hs [kbdfans_kbd75_rev2_sekogan]=kbd75v2)
km=sekogan
build="$root/build/qmk"; fw="$root/build/firmware"; scratch="$root/build/scratch"
mkdir -p "$scratch"
log=$(mktemp -p "$scratch" .build.XXXXXX); stage=; trap 'rm -f "$log"; [ -z "$stage" ] || rm -rf "$stage"' EXIT
describe=$(git -C "$root" describe --always --dirty --tags)

if [ $release = yes ]; then
  case "$describe" in *-dirty) echo "--release refused: uncommitted changes ($describe)" >&2; exit 1;; esac
  qmkver=$(git -C "$root/qmk_firmware" describe --tags --exact-match 2>/dev/null) \
    || { echo "--release refused: qmk_firmware is not on a release tag" >&2; exit 1; }
  version=$(git -C "$root" show HEAD:VERSION 2>/dev/null | tr -d '[:space:]') || true
  [[ "$version" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]] || { echo "--release refused: no valid VERSION (MAJOR.MINOR.PATCH) at HEAD" >&2; exit 1; }
  dest="$fw/qmk-$qmkver-$version"
  echo "release target: build/firmware/qmk-$qmkver-$version"
fi

names=()
for kb in "${targets[@]}"; do
  t="${kb//\//_}_$km"
  rm -f "$build/$t.hex"
  if ! "$root/scripts/qmk" compile -kb "$kb" -km "$km" -e SKIP_VERSION=yes >"$log" 2>&1; then
    cat "$log" >&2; echo "BUILD FAILED: $kb" >&2; exit 1
  fi
  grep -E 'warning:' "$log" >&2 || true
  names+=("$t")
done

qmkdesc=$(git -C "$root/qmk_firmware" describe --tags --always)
sizes=$("$root/scripts/qmk" --exec sh -c \
          'for t in "$@"; do avr-size --target=ihex "build/qmk/$t.hex" | awk -v t="$t" "NR==2{print t, \$4}"; done' sh "${names[@]}")
echo "# $(date -u +%FT%TZ)  repo $describe  qmk $qmkdesc  image ${QMK_IMAGE_ID#sha256:}"
while read -r t size; do
  printf '%-32s %6s B  %s\n' "$t" "$size" "$(sha256sum "$build/$t.hex" | cut -d' ' -f1)"
done <<<"$sizes"

if [ $release = no ]; then
  rm -rf "$scratch/check"; mkdir "$scratch/check"
  for t in "${names[@]}"; do cp "$build/$t.hex" "$scratch/check/${short[$t]}.hex"; done
  (cd "$scratch/check" && sha256sum *.hex > SHA256SUMS)
  printf 'Check build (unversioned, disposable; replaced by the next scripts/build_all.sh)\n\n  describe:  %s\n  commit:    %s\n  qmk:       %s\n  toolchain: %s\n  built:     %s\n' \
    "$describe" "$(git -C "$root" rev-parse HEAD)" "$qmkdesc" "$QMK_IMAGE_ID" "$(date -u +%F)" > "$scratch/check/README"
  exit 0
fi

stage=$(mktemp -d -p "$scratch" .release.XXXXXX)
files=()
for t in "${names[@]}"; do cp "$build/$t.hex" "$stage/${short[$t]}.hex"; files+=("${short[$t]}.hex"); done
if [ -e "$dest" ]; then
  # Same name = same firmware: every .hex must exist there and be identical.
  for f in "${files[@]}"; do
    [ -f "$dest/$f" ] || { echo "--release refused: $dest has no $f (VERSION not bumped?)" >&2; exit 1; }
    cmp -s "$stage/$f" "$dest/$f" || { echo "--release refused: $dest/$f differs from this build (VERSION not bumped?)" >&2; exit 1; }
  done
  (cd "$dest" && sha256sum -c --quiet SHA256SUMS) || { echo "$dest: SHA256SUMS check failed" >&2; exit 1; }
  echo "already released, identical: build/firmware/qmk-$qmkver-$version"
  exit 0
fi
tags=$(git -C "$root" tag --points-at HEAD | tr '\n' ' ' | sed 's/ $//')
{
  echo "Firmware $version (QMK $qmkver)"
  echo
  echo "  version:   $version"
  echo "  source:    commit $(git -C "$root" rev-parse HEAD)${tags:+ (tag $tags)}"
  echo "  qmk:       $qmkver $(git -C "$root/qmk_firmware" rev-parse HEAD)"
  echo "  toolchain: $QMK_IMAGE_ID (qmk_cli image; $(head -n1 "$build/obj_sofle_rev1_sekogan/compiler.txt" | sed 's/.*) //;s/^/avr-gcc /'))"
  echo "  built:     $(date -u +%F)"
  echo "  files:     $(printf '%s\n' "${files[@]}" | sort | tr '\n' ' ' | sed 's/ $//')"
} > "$stage/README"
(cd "$stage" && sha256sum $(printf '%s\n' "${files[@]}" | sort) > SHA256SUMS && chmod a-w ./* && chmod 755 .)
mkdir -p "$fw"; mv "$stage" "$dest"; stage=
echo "released to build/firmware/qmk-$qmkver-$version"
