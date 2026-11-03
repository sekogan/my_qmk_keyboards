#!/usr/bin/env bash
# SV-build (plan §6): build every target in the pinned image, print size + sha256,
# append them to build-results.txt (git-ignored).
#   scripts/build_all.sh            build all three targets
#   scripts/build_all.sh --archive  also store an immutable copy in ../qmk-data/qmk-artifacts/<git-describe>/
#                                   (hex files + BUILDINFO + SHA256SUMS; clean commits only). Flash from there.
set -euo pipefail
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel)
QMK_DATA=$(cd "$root/.." && pwd)/qmk-data
. "$root/scripts/toolchain.env"
archive=no; [ "${1:-}" = "--archive" ] && archive=yes

targets=(sofle/rev1 kbdfans/kbd75hs kbdfans/kbd75/rev2)   # keep in sync with qmk.json
km=sekogan
build="$root/qmk_firmware/.build"
log=$(mktemp); trap 'rm -f "$log"' EXIT

hexes=()
for kb in "${targets[@]}"; do
  name="${kb//\//_}_$km"
  rm -f "$build/$name.hex"
  if ! "$root/scripts/qmk" compile -kb "$kb" -km "$km" -e SKIP_VERSION=yes >"$log" 2>&1; then
    cat "$log" >&2; echo "BUILD FAILED: $kb" >&2; exit 1
  fi
  grep -E 'warning:' "$log" >&2 || true
  hexes+=("$name.hex")
done

describe=$(git -C "$root" describe --always --dirty --tags)
qmkver=$(git -C "$root/qmk_firmware" describe --tags --always)
sizes=$(podman run --rm --pull=never -v "$build":/b:ro,Z -w /b "$QMK_IMAGE" \
          sh -c 'for h in "$@"; do avr-size --target=ihex "$h" | awk -v h="$h" "NR==2{print h, \$4}"; done' sh "${hexes[@]}")
{
  echo "# $(date -u +%FT%TZ)  repo $describe  qmk $qmkver  image ${QMK_IMAGE_ID#sha256:}"
  while read -r h size; do
    printf '%-32s %6s B  %s\n' "${h%.hex}" "$size" "$(sha256sum "$build/$h" | cut -d' ' -f1)"
  done <<<"$sizes"
} | tee -a "$root/build-results.txt"

if [ $archive = yes ]; then
  # Immutable copy for flashing/bisection: only from a clean commit, never overwritten.
  case "$describe" in *-dirty) echo "--archive refused: uncommitted changes ($describe)" >&2; exit 1;; esac
  dest="$QMK_DATA/qmk-artifacts/$describe"
  if [ -e "$dest" ]; then
    # Builds are reproducible (SKIP_VERSION=yes), so an existing archive must match exactly.
    for h in "${hexes[@]}"; do
      cmp -s "$build/$h" "$dest/$h" || { echo "--archive refused: $dest/$h exists and differs" >&2; exit 1; }
    done
    echo "already archived (identical): $dest"
  else
    tmpdest="$dest.tmp.$$"; mkdir -p "$tmpdest"
    for h in "${hexes[@]}"; do cp "$build/$h" "$tmpdest/"; done
    {
      echo "commit=$(git -C "$root" rev-parse HEAD)"
      echo "describe=$describe"
      echo "qmk=$qmkver $(git -C "$root/qmk_firmware" rev-parse HEAD)"
      echo "image_id=$QMK_IMAGE_ID"
      echo "built=$(date -u +%FT%TZ)"
    } > "$tmpdest/BUILDINFO"
    (cd "$tmpdest" && sha256sum "${hexes[@]}" BUILDINFO > SHA256SUMS && chmod a-w ./*)
    mv "$tmpdest" "$dest"
    echo "archived to $dest"
  fi
fi
