# Changelog

One entry per firmware version (`VERSION`, [Semantic Versioning](https://semver.org); rules in
the [README](README.md#firmware-version)), newest first. The version covers all three
keyboards. "Sofle" is the sha256 of `sofle.hex` (built with `SKIP_VERSION=yes`).
Versions 1.0.0–1.0.3 were assigned retroactively; `VERSION` itself was introduced at 1.0.3.

## 1.0.6 — QMK 0.29.0

- Source: tag `qmk-0.29.0`.
- Sofle: `0e1dbd3d7dc057a3d3fbeb8fdfb9761838874c121a7e3b71e37de4a7891b26f6`
- Bump QMK 0.28.0 → 0.29.0 (upstream: non-volatile memory rework with a new EEPROM magic
  number, new compiler flags `-mlra`/`-mrelax` (about 1 KB smaller), Flow Tap opt-in,
  `encoder_update_user` deprecated). No keymap changes; no intended behaviour change.
  **One-time effect:** EEPROM is re-initialised on the first boot after flashing, so the
  saved base layer (Colemak) and similar settings return to their defaults; re-select them.

## 1.0.5 — QMK 0.28.0

- Source: tag `qmk-0.28.0`.
- Sofle: `0593c291b14a35ee0f0619743e266303d13559a179b76ed4c5837052c01a7727`
- Bump QMK 0.27.0 → 0.28.0 (upstream: retro-tapping rewrite, Chordal Hold and Community
  Modules as opt-in features). No keymap changes; no intended behaviour change.

## 1.0.4 — QMK 0.27.0

- Source: tag `qmk-0.27.0`.
- Sofle: `a17cf007578d197068cc18a2cdb776fe43abac8b28b0c3732e8a9c9946248f18`
- Bump QMK 0.26.0 → 0.27.0 (upstream: OS-detection and USB device state rework, single
  default layer API). No keymap changes; no intended behaviour change.

## 1.0.3 — QMK 0.26.0

- Source: commit `fb2a18e` (tag `qmk-0.26.0`); `VERSION` 1.0.3 added by the following
  commit with byte-identical firmware.
- Sofle: `70fb1804c9cefbf662bc8eb81b5d39677a5c64154c79191c363d4b8dec989474`
- Bump QMK 0.25.8 → 0.26.0. KBD75 v3.1: key overrides use the new array form required by
  QMK 0.26. No intended behaviour change.

## 1.0.2 — QMK 0.25.8

- Source: commit `1254d02` (tag `qmk-0.25.8-refactored`).
- Sofle: `cfafaf868abf61b2f8783c83012eab1fde04eb0cd1bea018617541a06115188c` (identical to 1.0.1)
- Repo moved to a QMK External Userspace with the QMK submodule and the pinned toolchain
  image; Sofle code clean-ups (custom keycodes renamed to `SEK_*`, formatting, license
  headers) without changing the Sofle firmware. KBD75 keymaps ported from QMK ~0.14 so they
  build again (renamed keycodes, tap-dance types, `led_update_user`, Bootmagic, `UG_*` RGB
  keycodes).

## 1.0.1 — QMK 0.25.8

- Source: tag `pre-modernization` (keymap `sofle/keymap`), rebuilt with the pinned
  toolchain image (avr-gcc 15.2.0).
- Sofle: `cfafaf868abf61b2f8783c83012eab1fde04eb0cd1bea018617541a06115188c`
- Same source as 1.0.0; only the compiler changed (avr-gcc 14.1 → 15.2).

## 1.0.0 — QMK 0.25.8

- Source: tag `pre-modernization` (keymap `sofle/keymap`), built 2024-06-15 with the
  then-current host toolchain (avr-gcc 14.1.0).
- Sofle: `6ba2670b348d20b4e6d2e89cbc0061927d19b4e4bcecc5e6002599dab7c8ea56`
- The firmware on the Sofle since June 2024 (Sofle only).
