# Firmware for Sofle keyboard

Derived from the [default keymap](https://github.com/qmk/qmk_firmware/tree/master/keyboards/sofle/keymaps/default).

![My Sofle!](sofle1000.jpg)

## Build and flash

See the [root README](../../../../README.md): `make release`, then `make flash` once for
**each half** (double-tap that half's reset button).

The keyboard has no handedness configuration: the half with the USB cable acts as the
left half, so always connect USB to the **left** half.

## History

- The language feature broke somewhere between QMK 0.15.11 and 0.16.9; 0.21.3 and 0.23.6
  were fine again. The firmware in daily use since June 2024 was built from QMK 0.25.8.
- November 2026: moved to a QMK External Userspace and upgraded release by release from
  QMK 0.25.8 to 0.34.7 (firmware 1.0.12) without keymap changes; see the
  [CHANGELOG](../../../../CHANGELOG.md) for upstream effects (EEPROM reset at 0.29.0, the
  key that wakes a suspended PC is no longer typed from 0.31.0).
