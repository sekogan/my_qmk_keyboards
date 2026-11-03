#!/usr/bin/env bash
# SV-build (plan §6): build every target in the pinned image, print size + sha256,
# append them to build-results.txt (git-ignored).
#   scripts/build_all.sh            build all three targets
#   scripts/build_all.sh --archive  also copy the hex files to ../qmk-data/qmk-artifacts/<git-describe>/
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
  dest="$QMK_DATA/qmk-artifacts/$describe"
  mkdir -p "$dest"
  for h in "${hexes[@]}"; do cp "$build/$h" "$dest/"; done
  (cd "$dest" && sha256sum "${hexes[@]}" > SHA256SUMS)
  echo "archived to $dest"
fi
