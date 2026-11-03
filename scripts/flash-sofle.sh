#!/usr/bin/env bash
# Flash one Sofle half (Pro Micro, Caterina bootloader) from the host with avrdude.
#
#   scripts/flash-sofle.sh                 the archived build of the current commit:
#                                          ../qmk-data/qmk-artifacts/<git describe>/sofle_rev1_sekogan.hex
#                                          (create it with: scripts/build_all.sh --archive)
#   scripts/flash-sofle.sh <dir|hex>       an archive/baseline directory, or a .hex inside one
#
# Never flashes from qmk_firmware/.build (or the hex copies next to it), which any
# concurrent build may delete or replace. The .hex must sit in a directory with a
# SHA256SUMS file listing it; the hash is checked before and right before writing.
# Start the script, then double-tap the reset button of the half to flash; it waits
# for the bootloader's new /dev/ttyACM* (present for ~8 s). EEPROM is not touched.
set -euo pipefail
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel)
QMK_DATA=$(cd "$root/.." && pwd)/qmk-data
name=sofle_rev1_sekogan.hex

if [ $# -eq 0 ]; then
  describe=$(git -C "$root" describe --always --dirty --tags)
  case "$describe" in *-dirty) echo "working tree has uncommitted changes; pass an explicit archive path" >&2; exit 1;; esac
  hex="$QMK_DATA/qmk-artifacts/$describe/$name"
  [ -f "$hex" ] || { echo "no archived build for $describe: $hex" >&2; echo "run: scripts/build_all.sh --archive" >&2; exit 1; }
elif [ -d "$1" ]; then
  hex="$1/$name"
else
  hex=$1
fi
[ -f "$hex" ] || { echo "no such file: $hex" >&2; exit 1; }
hex=$(realpath "$hex"); dir=$(dirname "$hex"); base=$(basename "$hex")
case "$hex" in
  "$(realpath "$root")"/*) echo "refusing to flash from inside the repo (mutable build output): $hex" >&2
                           echo "use an archive: scripts/build_all.sh --archive, then scripts/flash-sofle.sh" >&2; exit 1;;
  *_canary*) echo "refusing to flash a canary (non-LTO) build: $hex" >&2; exit 1;;
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
if [ -f "$dir/BUILDINFO" ]; then sed 's/^/        /' "$dir/BUILDINFO"; else echo "        (no BUILDINFO: not a build_all.sh archive)"; fi

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
