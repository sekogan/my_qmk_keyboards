// Repeats a keycode given number of times on each key press, creating
// "fast" version of that keycode.

#pragma once

#include QMK_KEYBOARD_H

struct fast_keycode {
    uint16_t fast_keycode;
    uint16_t real_keycode;
    uint8_t real_count;
};

// Sends multiple events instead of a single event.
// Should be called from process_record_user.
bool process_fast_keycodes(
    uint16_t keycode, keyrecord_t* record,
    const struct fast_keycode* begin, const struct fast_keycode* end
);
