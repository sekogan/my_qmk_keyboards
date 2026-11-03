#!/usr/bin/env bash
# SV-build (plan §6): build every target in the pinned image and print size + sha256.
# QMK's build directory is ../tmp/qmk-build (see scripts/qmk); nothing is written into the repo.
#   scripts/build_all.sh            build all three targets; an unversioned copy (short names +
#                                   SHA256SUMS + README) replaces ../tmp/build/ each time
#   scripts/build_all.sh --archive  also keep the build as ../firmware/qmk-<QMK version>-<VERSION>/
#                                   (sofle/kbd75hs/kbd75v2 .hex, read-only + SHA256SUMS + README; .elf/.map
#                                   are not kept: rebuild the tag into tmp/ if you need them).
#                                   VERSION is the repo's SemVer firmware version (file VERSION at HEAD).
#                                   Clean tree and a submodule on a QMK release tag only. If the directory
#                                   exists, the new build must be identical to it (then: no-op);
#                                   a different build under the same name is refused (VERSION not bumped?).
set -euo pipefail
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel)
PROJECT_ROOT=$(cd "$root/.." && pwd)
. "$root/scripts/toolchain.env"
archive=no; [ "${1:-}" = "--archive" ] && archive=yes

targets=(sofle/rev1 kbdfans/kbd75hs kbdfans/kbd75/rev2)   # keep in sync with qmk.json
declare -A short=([sofle_rev1_sekogan]=sofle [kbdfans_kbd75hs_sekogan]=kbd75hs [kbdfans_kbd75_rev2_sekogan]=kbd75v2)
km=sekogan
build="$PROJECT_ROOT/tmp/qmk-build"
fw="$PROJECT_ROOT/firmware"; tmpd="$PROJECT_ROOT/tmp"
log=$(mktemp); trap 'rm -f "$log"' EXIT
describe=$(git -C "$root" describe --always --dirty --tags)

if [ $archive = yes ]; then
  case "$describe" in *-dirty) echo "--archive refused: uncommitted changes ($describe)" >&2; exit 1;; esac
  qmkver=$(git -C "$root/qmk_firmware" describe --tags --exact-match 2>/dev/null) \
    || { echo "--archive refused: qmk_firmware is not on a release tag" >&2; exit 1; }
  version=$(git -C "$root" show HEAD:VERSION 2>/dev/null | tr -d '[:space:]') || true
  [[ "$version" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]] || { echo "--archive refused: no valid VERSION (MAJOR.MINOR.PATCH) at HEAD" >&2; exit 1; }
  name="qmk-$qmkver-$version"; dest="$fw/$name"
  echo "archive target: $dest"
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
sizes=$(podman run --rm --pull=never -v "$build":/b:ro,Z -w /b "$QMK_IMAGE" \
          sh -c 'for t in "$@"; do avr-size --target=ihex "$t.hex" | awk -v t="$t" "NR==2{print t, \$4}"; done' sh "${names[@]}")
{
  echo "# $(date -u +%FT%TZ)  repo $describe  qmk $qmkdesc  image ${QMK_IMAGE_ID#sha256:}"
  while read -r t size; do
    printf '%-32s %6s B  %s\n' "$t" "$size" "$(sha256sum "$build/$t.hex" | cut -d' ' -f1)"
  done <<<"$sizes"
}

if [ $archive = no ]; then
  mkdir -p "$tmpd"; rm -rf "$tmpd/build"; mkdir "$tmpd/build"
  for t in "${names[@]}"; do cp "$build/$t.hex" "$tmpd/build/${short[$t]}.hex"; done
  (cd "$tmpd/build" && sha256sum *.hex > SHA256SUMS)
  printf 'Check build (unversioned, disposable; replaced by the next scripts/build_all.sh)\nStatus: untested\n\n  describe:  %s\n  commit:    %s\n  qmk:       %s\n  toolchain: %s\n  built:     %s\n' \
    "$describe" "$(git -C "$root" rev-parse HEAD)" "$qmkdesc" "$QMK_IMAGE_ID" "$(date -u +%FT%TZ)" > "$tmpd/build/README"
fi

if [ $archive = yes ]; then
  mkdir -p "$fw" "$tmpd"
  stage=$(mktemp -d "$tmpd/.archive.XXXXXX"); trap 'rm -f "$log"; rm -rf "$stage"' EXIT
  files=()
  for t in "${names[@]}"; do
    cp "$build/$t.hex" "$stage/${short[$t]}.hex"; files+=("${short[$t]}.hex")
  done
  if [ -e "$dest" ]; then
    # Same name = same firmware: every .hex must exist there and be identical.
    for f in "${files[@]}"; do
      [ -f "$dest/$f" ] || { echo "--archive refused: $dest has no $f (VERSION not bumped?)" >&2; exit 1; }
      cmp -s "$stage/$f" "$dest/$f" || { echo "--archive refused: $dest/$f differs from this build (VERSION not bumped?)" >&2; exit 1; }
    done
    (cd "$dest" && sha256sum -c --quiet SHA256SUMS) || { echo "$dest: SHA256SUMS check failed" >&2; exit 1; }
    echo "already archived, identical: $dest"
    exit 0
  fi
  tags=$(git -C "$root" tag --points-at HEAD | tr '\n' ' ' | sed 's/ $//')
  {
    echo "Firmware $version (QMK $qmkver)"
    echo "Status: untested"
    echo
    echo "Origin (fixed; the Status line above may be updated later)"
    echo "  version:   $version"
    echo "  origin:    scripts/build_all.sh --archive"
    echo "  commit:    $(git -C "$root" rev-parse HEAD)"
    echo "  describe:  $describe"
    echo "  tags:      ${tags:--}"
    echo "  qmk:       $qmkver $(git -C "$root/qmk_firmware" rev-parse HEAD)"
    echo "  toolchain: $QMK_IMAGE_ID (qmk_cli image)"
    echo "  compiler:  $(head -n1 "$build/obj_sofle_rev1_sekogan/compiler.txt")"
    echo "  options:   SKIP_VERSION=yes"
    echo "  built:     $(date -u +%FT%TZ)"
    while read -r t size; do echo "  size:      ${short[$t]}.hex $size B"; done <<<"$sizes"
    echo "  files:     $(printf '%s\n' "${files[@]}" | sort | tr '\n' ' ' | sed 's/ $//')"
  } > "$stage/README"
  (cd "$stage" && sha256sum $(printf '%s\n' "${files[@]}" | sort) > SHA256SUMS && chmod a-w "${files[@]}" SHA256SUMS && chmod 644 README && chmod 755 .)
  mv "$stage" "$dest"
  echo "archived to $dest"
fi
