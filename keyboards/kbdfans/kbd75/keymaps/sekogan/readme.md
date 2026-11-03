# Firmware for Kbd75 v2

This is a 83-key layout.
The goal is to make everyday coding and debugging simple and comfortable.

Features:

- Large **[Shift]**, right **[Control]** and **[Alt]** keys.
- Almost all TKL keys are on the base layer. In particular all keys from
  the navigation cluster.
- **[Delete]** is in the top right corner, easy to find.
- **[Fn]** is a second function of **[Caps Lock]** key. Doesn't have a dedicated **[Fn]** key.
- Keyboard settings moved to a special layer that is not so easy to access
  (**[Caps Lock + Escape]**). This was done on purpose to prevent accidental key
  presses. **[Caps Lock + Escape + F12]** switches the keyboard into
  bootloader mode.
- Bootmagic is enabled. Press **[Backspace]** while connecting
  the keyboard to USB to reset EEPROM and jump into bootloader.
  Note that **[Caps Lock + Escape + F12]** does not reset EEPROM.

Hardware documentation: [docs/kbd75v2](../../../../../docs/kbd75v2/).

## Status

**Builds; not hardware-tested since 2022.** The keymap was ported from QMK ~0.14 to the
current QMK version without access to the board, so the changes since then are only
verified by compiling.

## Build and flash

See the [root README](../../../../../README.md): `scripts/qmk compile -kb kbdfans/kbd75/rev2 -km sekogan`,
then put the board into the bootloader and flash `../qmk-data/qmk-artifacts/<git describe>/kbdfans_kbd75_rev2_sekogan.hex` (from `scripts/build_all.sh --archive`) with
`dfu-programmer` (erase, flash, launch).
