#!/usr/bin/env bash
# Flash one Halcyon Elora half (RP2040, UF2 bootloader) from the host, by copying the .uf2 to
# the bootloader's USB drive. Both halves take the same file; run this once per half.
#
#   scripts/flash-elora.sh             the release of this commit's VERSION:
#                                      build/firmware/qmk-<QMK version>-<VERSION>/elora.uf2
#                                      (create it with: make release = scripts/build_all.sh --release)
#   scripts/flash-elora.sh <dir|uf2>   a firmware directory such as build/firmware/qmk-0.34.7-1.1.0,
#                                      the last check build build/scratch/check, or an elora.uf2 in
#                                      one of them
#
# Never flashes from QMK's build directory (build/qmk), which any concurrent build may
# delete or replace. The .uf2 must sit in a directory with a SHA256SUMS file listing it;
# the hash is checked before and right before writing. Start the script, then double-tap
# the reset button of the half to flash; it waits for the RPI-RP2 drive.
set -euo pipefail
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel)
name=elora.uf2

if [ $# -eq 0 ]; then
  version=$(git -C "$root" show HEAD:VERSION 2>/dev/null | tr -d '[:space:]') || true
  [ -n "$version" ] || { echo "no VERSION at HEAD; pass an explicit firmware directory" >&2; exit 1; }
  qmkver=$(git -C "$root/qmk_firmware" describe --tags --exact-match 2>/dev/null) \
    || { echo "qmk_firmware is not on a release tag; pass an explicit firmware directory" >&2; exit 1; }
  if [ -n "$(git -C "$root" status --porcelain --untracked-files=no)" ]; then
    echo "note: uncommitted changes are NOT in this firmware (it is the release of VERSION $version)" >&2
  fi
  d="$root/build/firmware/qmk-$qmkver-$version"
  [ -d "$d" ] || { echo "no release $d" >&2; echo "run: make release" >&2; exit 1; }
  uf2="$d/$name"
elif [ -d "$1" ]; then
  uf2="${1%/}/$name"
else
  uf2=$1
fi
[ -f "$uf2" ] || { echo "no such file: $uf2" >&2; exit 1; }
uf2=$(realpath "$uf2"); dir=$(dirname "$uf2"); base=$(basename "$uf2")
r=$(realpath "$root")
case "$uf2" in
  *_canary*) echo "refusing to flash a canary (non-LTO) build: $uf2" >&2; exit 1;;
  "$r"/build/firmware/*|"$r"/build/scratch/*) ;;            # releases and copies with SHA256SUMS
  "$r"/build/qmk/*) echo "refusing to flash from QMK's build directory (mutable): $uf2" >&2
                    echo "use build/scratch/check (make) or a release (make release)" >&2; exit 1;;
  "$r"/*) echo "refusing to flash from inside the repo outside build/firmware and build/scratch: $uf2" >&2; exit 1;;
esac
[ -f "$dir/SHA256SUMS" ] || { echo "no SHA256SUMS next to $uf2" >&2; exit 1; }
want=$(awk -v f="$base" '$2 == f || $2 == "*" f {print $1}' "$dir/SHA256SUMS")
[ -n "$want" ] || { echo "$base is not listed in $dir/SHA256SUMS" >&2; exit 1; }
check() {
  local got; got=$(sha256sum "$uf2" | cut -d' ' -f1)
  [ "$got" = "$want" ] || { echo "sha256 MISMATCH for $uf2: got $got, SHA256SUMS says $want" >&2; exit 1; }
}
check
echo "uf2:    $uf2"
echo "sha256: $want (matches SHA256SUMS)"
if [ -f "$dir/README" ]; then grep -v 'cflags' "$dir/README" | sed 's/^/        /'; else echo "        (no README)"; fi

# The RP2040 bootloader mounts as a USB drive labeled RPI-RP2.
find_mount() {
  local m
  for m in "/run/media/$USER/RPI-RP2" "/media/$USER/RPI-RP2"; do
    if [ -f "$m/INFO_UF2.TXT" ]; then echo "$m"; return 0; fi
  done
  return 0
}
echo "Double-tap reset on the half to flash (waiting up to 60 s for the RPI-RP2 drive)..."
mnt=
for _ in $(seq 300); do
  mnt=$(find_mount)
  [ -n "$mnt" ] && break
  if [ -e /dev/disk/by-label/RPI-RP2 ]; then udisksctl mount -b /dev/disk/by-label/RPI-RP2 >/dev/null 2>&1 || true; fi
  sleep 0.2
done
[ -n "$mnt" ] || { echo "no mounted RPI-RP2 drive appeared" >&2; exit 1; }
echo "drive:  $mnt"
check
cp "$uf2" "$mnt/" && sync
echo "Copied; the half reboots by itself. Repeat for the other half (same file)."
