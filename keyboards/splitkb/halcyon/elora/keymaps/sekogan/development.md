# Halcyon Elora keymap: maintainer notes

Details behind the [readme](readme.md). The repo-wide rules (versioning, releases, checks)
are in the [root README](../../../../../../README.md).

## Hardware

- Two Elora rev2 halves (RP2040). The display half carries a Halcyon TFT module (ST7789,
  135x240); the other half has no modules. No other module configuration is handled.
- The half with USB is the master, and only the master draws on the display.
- Left and right are set in hardware (GP23), so both halves run the same firmware.
- The RGB LEDs and encoders are not used.

## Differences from the Sofle layers

The Elora `keymap.c` holds its own copy of the Sofle layers. `scripts/check-elora-layout.sh`
(part of `make check`) fails if they differ, except for the keys listed here (the list in the
script must match):

- QWERTY, COLEMAK: L20 is Left Shift, L19 selects the primary language, R19 the alternative
  one and R20 holds the numpad layer; on every other layer those four keys do nothing.
- QWERTY, COLEMAK: the outer thumb keys are plain Left and Right Super; tapping them does
  not select a language.
- QWERTY, COLEMAK: the inner right thumb key is Enter (the numpad layer is on R20).
- RAISE: the outer right key of the bottom row is transparent (Right Shift) instead of Enter.
- EXTRA: display brightness down and up on the COLEMAK R and W positions (QWERTY S and W);
  the OS selector is on G, under BASE, so all keyboard controls are on the left half.
- EXTRA, right half: media keys instead of the Sofle's left encoder, on the RAISE arrow
  positions (volume up/down on up/down, previous/next track on left/right), with mute under
  volume down and play/pause on the full stop.

The layer diagrams in `keymap.c` are generated: `make keymapviz` (legends in
`keymapviz.toml`).

## Display

Design rules for the default skin: [display_skin.md](display_skin.md). Black background,
top to bottom:

- **Layer tally**: the highest active layer (BASE, LOWER, RAISE, NUMPAD, EXTRA) in a
  full-width box (BASE dark grey, LOWER blue, RAISE orange, NUMPAD yellow, EXTRA violet).
- **Mod tiles**: Shift, Ctrl, Alt and GUI (Super, Windows or Command icon, following the OS),
  each in its own colour. A tile is lit while its modifier is held or one-shot; the Left Shift
  that Caps Word registers itself does not light Shift.
- **Lock bars**: CAPS and WORD (Caps Word) over NUM and SCRL. Lit bars are full colour
  (green, WORD amber) with black text; off bars are a dim version of the colour.
- **OS, LAYOUT and LANGUAGE sections**: a grey legend over a value. OS is LINUX, WINDOWS,
  MACOS or UNKNOWN (red); its AUTO tag is lit while autodetection is on and the OS is known.
  LAYOUT is QWERTY or COLEMAK; its AUTO tag is lit while instant QWERTY is active. LANGUAGE
  is PRIMARY or SECONDARY (amber); it is shown muted except on BASE with Linux or Windows.
  While LOWER holds the stashed language, the language and layout from before LOWER are shown.

**Backlight popup**: BL+ or BL- on EXTRA replaces the whole screen with DISPLAY BACKLIGHT,
the level as `n/10` and a 10-segment meter (green, then amber, then red). It hides 2.5 s
after the last BL+/BL- (`SEK_DISPLAY_BL_OVERLAY_MS`), or at once when EXTRA is left;
re-entering EXTRA does not bring it back.

**Idle-off**: after 120 s without key activity (`SEK_DISPLAY_TIMEOUT`) the panel and its
backlight turn off; the next key turns them on and the screen is redrawn. They are also off
while the host is suspended.

Only the parts of the screen that changed are redrawn and sent to the panel.

`scripts/check-elora-display.sh` (part of `make check`) renders every screen state on the
host and fails unless each one is pixel-exact with the reference renders in
`scripts/elora_display_test/`.

## Fonts

- Labels: Leggie 12x24 bold and 9x18 bold by Wiktor Kerr, CC BY 4.0. The glyphs were cropped
  to their inked rows and converted to QFF; no other changes. Licence: `graphics/LICENCE.leggie`.
- Values: a font derived from Terminus Font 14x28 bold by Dimitar Toshkov Zhekov, SIL OFL 1.1.
  The glyphs were cropped to their inked rows and converted to QFF. Because "Terminus Font" is
  a Reserved Font Name, the derivative is not called Terminus (it is `font_val28_caps`).
  Licence: `graphics/OFL.terminus.txt`.

The source BDFs, the licences and the generated QMK font data are in `graphics/`
(`scripts/elora-display-assets.sh` regenerates them).
