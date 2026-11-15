// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// The five design test states (the natives in scripts/elora_display_test/reference/natives/),
// field for field as sketchlib.STATES, in STATE_ORDER.
// Used by the host check only (scripts/check-elora-display.sh); not linked into the firmware.

#include "display_state.h"

const display_state_t display_test_states[5] = {
    // 1 idle
    {.layer = DS_LAYER_BASE, .os = DS_OS_LINUX, .os_auto = true, .backlight = 6, .num = true},
    // 2 lower, secondary: one-shot Shift, Caps Lock, Windows not detected, instant QWERTY
    {.layer = DS_LAYER_LOWER, .os = DS_OS_WINDOWS, .backlight = 6, .qwerty = true, .layout_auto = true, .secondary = true, .shift = true, .caps = true, .num = true},
    // 3 extra, no OS: Caps Word
    {.layer = DS_LAYER_EXTRA, .os = DS_OS_UNKNOWN, .backlight = 9, .word = true, .num = true},
    // 4 numpad shortcut: Ctrl+Alt+Cmd held, macOS detected, QWERTY, Scroll Lock, no Num Lock
    {.layer = DS_LAYER_NUMPAD, .os = DS_OS_MAC, .os_auto = true, .backlight = 6, .qwerty = true, .ctrl = true, .alt = true, .gui = true, .scrl = true},
    // 5 backlight adjust: the popup at 10/10
    {.layer = DS_LAYER_EXTRA, .os = DS_OS_LINUX, .os_auto = true, .backlight = 10, .caps = true, .word = true, .num = true, .overlay = true},
};
