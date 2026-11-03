#!/usr/bin/env bash
# Flash one Sofle half (Pro Micro, Caterina bootloader) from the host with avrdude.
#
#   scripts/flash-sofle.sh             the archived build of this commit's VERSION:
#                                      build/firmware/qmk-<QMK version>-<VERSION>/sofle.hex
#                                      (create it with: scripts/build_all.sh --archive)
#   scripts/flash-sofle.sh <dir|hex>   a firmware directory such as build/firmware/qmk-0.26.0-1.0.3,
#                                      the last check build build/scratch/check, a bisect build
#                                      build/scratch/bisect/<commit>, or a sofle.hex in one of them
#
# Never flashes from QMK's build directory (build/qmk), which any concurrent build may
# delete or replace. The .hex must sit in a directory with a SHA256SUMS file listing it;
# the hash is checked before and right before writing. Start the script, then double-tap
# the reset button of the half to flash; it waits for the bootloader's new /dev/ttyACM*
# (present for ~8 s). EEPROM is not touched.
set -euo pipefail
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel)
name=sofle.hex

if [ $# -eq 0 ]; then
  version=$(git -C "$root" show HEAD:VERSION 2>/dev/null | tr -d '[:space:]') || true
  [ -n "$version" ] || { echo "no VERSION at HEAD; pass an explicit firmware directory" >&2; exit 1; }
  qmkver=$(git -C "$root/qmk_firmware" describe --tags --exact-match 2>/dev/null) \
    || { echo "qmk_firmware is not on a release tag; pass an explicit firmware directory" >&2; exit 1; }
  if [ -n "$(git -C "$root" status --porcelain --untracked-files=no)" ]; then
    echo "note: uncommitted changes are NOT in this firmware (it is the archived build of VERSION $version)" >&2
  fi
  d="$root/build/firmware/qmk-$qmkver-$version"
  [ -d "$d" ] || { echo "no archived build $d" >&2; echo "run: scripts/build_all.sh --archive" >&2; exit 1; }
  hex="$d/$name"
elif [ -d "$1" ]; then
  hex="${1%/}/$name"
else
  hex=$1
fi
[ -f "$hex" ] || { echo "no such file: $hex" >&2; exit 1; }
hex=$(realpath "$hex"); dir=$(dirname "$hex"); base=$(basename "$hex")
r=$(realpath "$root")
case "$hex" in
  *_canary*) echo "refusing to flash a canary (non-LTO) build: $hex" >&2; exit 1;;
  "$r"/build/firmware/*|"$r"/build/scratch/*) ;;            # archives and copies with SHA256SUMS
  "$r"/build/qmk/*) echo "refusing to flash from QMK's build directory (mutable): $hex" >&2
                    echo "use build/scratch/check (scripts/build_all.sh) or an archive (scripts/build_all.sh --archive)" >&2; exit 1;;
  "$r"/*) echo "refusing to flash from inside the repo outside build/firmware and build/scratch: $hex" >&2; exit 1;;
esac
[ -f "$dir/SHA256SUMS" ] || { echo "no SHA256SUMS next to $hex" >&2; exit 1; }
want=$(awk -v f="$base" '$2 == f || $2 == "*" f {print $1}' "$dir/SHA256SUMS")
[ -n "$want" ] || { echo "$base is not listed in $dir/SHA256SUMS" >&2; exit 1; }
check() {
  local got; got=$(sha256sum "$hex" | cut -d' ' -f1)
  [ "$got" = "$want" ] || { echo "sha256 MISMATCH for $hex: got $got, SHA256SUMS says $want" >&2; exit 1; }
}
check
echo "hex:    $hex"
echo "sha256: $want (matches SHA256SUMS)"
if [ -f "$dir/README" ]; then grep -v 'cflags' "$dir/README" | sed 's/^/        /'; else echo "        (no README)"; fi

list() { ls /dev/ttyACM* 2>/dev/null | sort || true; }
before=$(list)
echo "Double-tap reset on the half to flash (waiting up to 60 s)..."
port=
for _ in $(seq 600); do
  port=$(comm -13 <(echo "$before") <(list) | head -n1)
  [ -n "$port" ] && break
  before=$(comm -12 <(echo "$before") <(list))   # forget ports that disappeared
  sleep 0.1
done
[ -n "$port" ] || { echo "no new /dev/ttyACM* appeared" >&2; exit 1; }
for _ in $(seq 20); do [ -w "$port" ] && break; sleep 0.1; done   # udev may need a moment
echo "port:   $port"
check
exec avrdude -p atmega32u4 -c avr109 -P "$port" -U "flash:w:$hex:i"
