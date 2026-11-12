# Firmware for Halcyon Elora rev2

QMK target `splitkb/halcyon/elora/rev2`, keymap `sekogan` (shared code in
[`users/sekogan/`](../../../../../../users/sekogan/)).

## Hardware

- Two Elora rev2 halves (RP2040). The display half carries a Halcyon TFT module (ST7789,
  135x240); the other half has no modules. No other module configuration is handled.
- Always connect USB to the **display half**: the half with USB is the master, and only the
  master draws on the display.
- Left and right are set in hardware (GP23), so both halves run the same firmware.
- The RGB LEDs and encoders are not used. On the QWERTY and COLEMAK layers L20 is Left Shift,
  L19 selects the primary language, R19 the alternative one and R20 holds the numpad layer; on
  every other layer those four keys do nothing.
- Other differences from the Sofle layers:
  - QWERTY, COLEMAK: the outer thumb keys are plain Left and Right Super; tapping them does
    not select a language.
  - EXTRA: display brightness down and up on the COLEMAK R and W positions (QWERTY S and W);
    the OS selector is on G, under BASE, so all keyboard controls are on the left half.
  - QWERTY, COLEMAK: the inner right thumb key is Enter (the numpad layer is on R20).
  - RAISE: the outer right key of the bottom row is transparent (Right Shift) instead of Enter.

## Build and flash

See the [root README](../../../../../../README.md): `make release`, then `make flash-elora`
once for **each half**, with the same `elora.uf2` (double-tap that half's reset button; the
bootloader appears as the `RPI-RP2` drive).

## Font

The display uses [Spleen](https://github.com/fcambus/spleen) 12x24 2.2.0 by Frederic Cambus,
BSD-2-Clause. The font, its licence and the generated QMK font data are in `graphics/`.
