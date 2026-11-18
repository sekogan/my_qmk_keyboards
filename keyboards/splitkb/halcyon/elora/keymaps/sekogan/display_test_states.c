// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// The six design test states (the natives in scripts/elora_display_test/reference/natives/),
// field for field as skin.STATES (sketchlib's five, then skin.py's "version"), in STATE_ORDER;
// lang_muted as ds_resolve() gives it. compare.py gives the version popup skin.TEST_VERSIONS.
// Used by the host check only (scripts/check-elora-display.sh); not linked into the firmware.

#include "display_state.h"

const display_state_t display_test_states[6] = {
    // 1 idle
    {.layer = DS_LAYER_BASE, .os = DS_OS_LINUX, .os_auto = true, .backlight = 6, .num = true},
    // 2 lower, secondary: one-shot Shift, Caps Lock, Windows not detected, instant QWERTY
    {.layer = DS_LAYER_LOWER, .os = DS_OS_WINDOWS, .backlight = 6, .qwerty = true, .layout_auto = true, .secondary = true, .lang_muted = true, .shift = true, .caps = true, .num = true},
    // 3 extra, no OS: Caps Word
    {.layer = DS_LAYER_EXTRA, .os = DS_OS_UNKNOWN, .backlight = 9, .lang_muted = true, .word = true, .num = true},
    // 4 numpad shortcut: Ctrl+Alt+Cmd held, macOS detected, QWERTY, Scroll Lock, no Num Lock
    {.layer = DS_LAYER_NUMPAD, .os = DS_OS_MAC, .os_auto = true, .backlight = 6, .qwerty = true, .lang_muted = true, .ctrl = true, .alt = true, .gui = true, .scrl = true},
    // 5 backlight adjust: the popup at 10/10
    {.layer = DS_LAYER_EXTRA, .os = DS_OS_LINUX, .os_auto = true, .backlight = 10, .lang_muted = true, .caps = true, .word = true, .num = true, .popup = DS_POPUP_BACKLIGHT},
    // 6 version: the version popup (KEYMAP 1.5.1, QMK 0.34.7)
    {.layer = DS_LAYER_EXTRA, .os = DS_OS_LINUX, .os_auto = true, .backlight = 6, .lang_muted = true, .num = true, .popup = DS_POPUP_VERSION},
};
