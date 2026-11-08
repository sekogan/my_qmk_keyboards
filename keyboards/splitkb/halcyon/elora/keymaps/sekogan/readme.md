# Firmware for Halcyon Elora rev2

QMK target `splitkb/halcyon/elora/rev2`, keymap `sekogan` (shared code in
[`users/sekogan/`](../../../../../../users/sekogan/)).

## Hardware

- Two Elora rev2 halves (RP2040). The display half carries a Halcyon TFT module (ST7789,
  135x240); the other half has no modules. No other module configuration is handled.
- Always connect USB to the **display half**: the half with USB is the master, and only the
  master draws on the display.
- Left and right are set in hardware (GP23), so both halves run the same firmware.
- The RGB LEDs and encoders are not used. The keys L19, L20, R19 and R20 do nothing on any
  layer for now.

## Build and flash

See the [root README](../../../../../../README.md): `make release`, then `make flash-elora`
once for **each half**, with the same `elora.uf2` (double-tap that half's reset button; the
bootloader appears as the `RPI-RP2` drive).

## Font

The display uses [Spleen](https://github.com/fcambus/spleen) 12x24 2.2.0 by Frederic Cambus,
BSD-2-Clause. The font, its licence and the generated QMK font data are in `graphics/`.
