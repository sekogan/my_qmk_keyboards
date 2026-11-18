// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// Keys that only work at the position of a given letter in the base layout the user selected.
// Depends on "instant qwerty" feature.

#pragma once

#include "quantum.h"

// A keycode placed under a letter of several base layouts: it works only where the base layout the
// user selected has that letter (not the Qwerty layout activated by "instant qwerty").
struct selected_layout_key {
    uint16_t keycode;
    uint16_t letter;
};

// Should be called first in process_record_user. Returns false (ignore the event) for a press or
// release of a listed keycode where the selected layout doesn't have its letter.
bool process_selected_layout_keys(uint16_t keycode, keyrecord_t *record, const struct selected_layout_key *begin, const struct selected_layout_key *end);
