#!/usr/bin/env bash
# Flash one Sofle half (Pro Micro, Caterina bootloader) from the host with avrdude.
#   scripts/flash-sofle.sh [hex]     default: qmk_firmware/.build/sofle_rev1_sekogan.hex
# Start it, then double-tap the reset button of the half to flash; it waits for the
# bootloader's new /dev/ttyACM* (present for ~8 s) and writes the flash. EEPROM is not touched.
set -euo pipefail
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel)
hex=${1:-$root/qmk_firmware/.build/sofle_rev1_sekogan.hex}
[ -f "$hex" ] || { echo "no such file: $hex" >&2; exit 1; }
case "$hex" in *_canary*) echo "refusing to flash a canary (non-LTO) build: $hex" >&2; exit 1;; esac
echo "hex:    $hex"
echo "sha256: $(sha256sum "$hex" | cut -d' ' -f1)"

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
exec avrdude -p atmega32u4 -c avr109 -P "$port" -U "flash:w:$hex:i"
