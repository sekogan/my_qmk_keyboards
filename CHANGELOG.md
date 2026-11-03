# Changelog

One entry per firmware version (`VERSION`, [Semantic Versioning](https://semver.org); rules in
the [README](README.md#firmware-version)), newest first. The version covers all three
keyboards. "Sofle" is the sha256 of `sofle.hex` (built with `SKIP_VERSION=yes`).
Versions 1.0.0–1.0.3 were assigned retroactively; `VERSION` itself was introduced at 1.0.3.

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
