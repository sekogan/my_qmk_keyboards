"""Firmware enum orders shared by compare.py and scripts/elora-display-assets.py."""

LAYERS = ["BASE", "LOWER", "RAISE", "NUMPAD", "EXTRA"]  # ds_layer_t (display_state.h)
ICONS = ["shift", "ctrl", "alt", "super", "win", "cmd"]  # enum gfx_icon (display_gfx.h)
VERSION_GLYPHS = "0123456789."  # the glyphs of a version string, in SKIN_VERSION_INK_* order (display_skin_metrics.h)
VERSION_LABELS = ["KEYMAP", "QMK"]  # the version popup labels (SKIN_VER_X_*)
WIDE_VERSIONS = ("10.12.13", "0.34.10")  # a wide pair for the checks (the test pair is skin.TEST_VERSIONS)
