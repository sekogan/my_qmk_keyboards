# Changelog

One entry per firmware version (`VERSION`, [Semantic Versioning](https://semver.org); rules in
the [README](README.md#firmware-version)), newest first. The version covers all three
keyboards. "Sofle" is the sha256 of `sofle.hex` (built with `SKIP_VERSION=yes`).
Versions 1.0.0–1.0.3 were assigned retroactively; `VERSION` itself was introduced at 1.0.3.

## 1.0.17 — QMK 0.34.7

- Source: the commit that adds this entry (`12.5`, no tag); the release `README` names it.
- Sofle: `a1603f1e64b5dfb243f7e771bdc4fe685491818b98e45b315735ca0b5e628b07`
- Sofle: the LOWER and RAISE keys use QMK's core Tri Layer (`TL_LOWR`/`TL_UPPR`) instead
  of custom keycodes with `update_tri_layer()`. Same layers (LOWER + RAISE = EXTRA), same
  keys; the language stash on LOWER and Caps Word are unchanged. No intended behaviour
  change.

## 1.0.16 — QMK 0.34.7

- Source: the commit that adds this entry (`12.4`, no tag); the release `README` names it.
- Sofle: `ba283c3af88d2a564dd8c6c684557bce46b4ea6e0f3c12843e1423e090b82a45`
- Sofle: the encoders use QMK's Encoder Map instead of the deprecated
  `encoder_update_user()`. Same functions (left: volume, Ctrl = next/previous track;
  right: Page Down/Up, Ctrl/Alt/GUI without Shift = Tab/Shift+Tab) on every layer; encoder
  turns are handled before every other feature, so Caps Word and word selection are not
  affected by them. One timing detail: a turn made while a Super (tap/hold) key is still
  undecided now waits for that decision and then takes the held modifier into account.

## 1.0.15 — QMK 0.34.7

- Source: the commit that adds this entry (`12.3`, no tag); the release `README` names it.
- Sofle: `6221bbfdc6977a3c256e49b8614f0a53c8af3d547edb6a15fa030799fbc4af1a`
- Sofle: OS auto-detection uses QMK's `process_detected_host_os_user()` callback instead
  of polling from the OLED code. Same rules as before: the first detected OS is applied
  once per entry into auto mode (power-up, or `OS` cycling back to auto); later
  re-detections (wake, re-plug through a KVM) are ignored; manual mode is untouched. No
  intended behaviour change.

## 1.0.14 — QMK 0.34.7

- Source: the commit that adds this entry (`12.2`, no tag); the release `README` names it.
- Sofle: `c313a66683a93002823158faeab17c25402f27819984383d69d3ec3b5fa72267`
- Sofle: Bootmagic and NKRO turned off (both were defaults of the upstream Sofle keyboard
  definition since QMK 0.24.0, not a choice of this keymap). Holding the top-left key while
  plugging in no longer jumps to the bootloader or clears EEPROM; use the reset button
  (double-tap) to flash. Bootmagic was not used, so this is a PATCH, not a removed feature.
  The keyboard reports 6KRO instead of NKRO; the USB descriptor of interface 1 shrinks
  (111 → 52 bytes), so re-plug once on each host. About 400 bytes smaller.

## 1.0.13 — QMK 0.34.7

- Source: the commit that adds this entry (`12.1`, no tag); the release `README` names it.
- Sofle: `ea97facded396c80fadf3bd917759345a151a822446dc7f9e8d3e6342cc8ae05`
- Sofle bug fix: NumLock forcing works again. The old check ran once at power-up, before
  the host had sent its LED state, so it had no effect. Now a check is armed when USB
  is configured (plug-in, resume) and whenever the host turns NumLock off, and about
  0.5 s later the left (USB) half taps NumLock once if it is still off.

## 1.0.12 — QMK 0.34.7

- Source: tag `qmk-0.34.7`.
- Sofle: `265efdcc08cfccb573339ef87dd192bd7886de843b29c769e1cf8cf5d9e5db5f`
- Bump QMK 0.34.0 → 0.34.7 (latest release; upstream fixes only: the System
  Control HID range is widened again so `KC_SLEP` works on Windows, which matters for the
  KBD75 Sleep key). No keymap changes; no intended behaviour change.

## 1.0.11 — QMK 0.34.0

- Source: tag `qmk-0.34.0`.
- Sofle: `3c281b4097f833f2f79aa2256cad35e468120951fcaaf50e925df33ef2502d81`
- Bump QMK 0.33.0 → 0.34.0 (upstream: the EXTRAKEY System Control HID range is
  restricted to 0x81–0x8F, so the USB descriptor changes: re-plug the keyboard once on
  each host). No keymap changes; no intended behaviour change.

## 1.0.10 — QMK 0.33.0

- Source: tag `qmk-0.33.0`.
- Sofle: `425b90b73a97fa0cc73c0ca0f74fe2788b072541ea571534fa70943713c79c4f`
- Bump QMK 0.32.0 → 0.33.0 (upstream: deprecated `isLeftHand`, `FORCE_NKRO` and
  `OLED_DISPLAY_128X32` removed). No keymap changes; no intended behaviour change.

## 1.0.9 — QMK 0.32.0

- Source: tag `qmk-0.32.0`.
- Sofle: `09eeba1a03aa7b442149a6415a1b2edd25b09392af44a2ae48d51eed8ec40c47`
- Bump QMK 0.31.0 → 0.32.0 (upstream: deprecated GPIO defines removed, core split
  code uses `is_keyboard_left()`, stricter lint). No keymap changes; no intended behaviour
  change.

## 1.0.8 — QMK 0.31.0

- Source: tag `qmk-0.31.0`.
- Sofle: `45f73c905f3dd0facb8d632de96142adfaed21cbcedb26d06d737414b881ee27`
- Bump QMK 0.30.0 → 0.31.0 (upstream: new default OLED font, tap-dance state
  refactor, Speculative Hold opt-in). No keymap changes. Behaviour change from upstream:
  the key that wakes a suspended PC is no longer typed (QMK #23389).

## 1.0.7 — QMK 0.30.0

- Source: tag `qmk-0.30.0`.
- Sofle: `3578ebcc233b0e66417621bb38e7e7c5eb6f50919a714731dcd9a89e56928ae7`
- Bump QMK 0.29.0 → 0.30.0 (upstream: deprecated `RGB_*` and mouse keycodes removed,
  `DEFAULT_FOLDER` removed, quantum keycode handling relocated; kbd75hs NKRO default moved
  from `FORCE_NKRO` to `NKRO_DEFAULT_ON`). No keymap changes; no intended behaviour change.

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
