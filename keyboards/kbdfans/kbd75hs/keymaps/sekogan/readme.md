# Firmware for Kbd75 v3.1

## Status

**Builds; not hardware-tested since 2022.** The keymap was ported from QMK ~0.14 to the
current QMK version without access to the board, so the changes since then are only
verified by compiling.

## Build and flash

See the [root README](../../../../../README.md): `scripts/qmk compile -kb kbdfans/kbd75hs -km sekogan`,
then put the board into the bootloader and flash `build/firmware/qmk-<QMK version>-<VERSION>/kbd75hs.hex` (from `scripts/build_all.sh --archive`) with
`dfu-programmer` (erase, flash, launch).
