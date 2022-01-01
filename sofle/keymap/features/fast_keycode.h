// Repeats a keycode given number of times on each key press, creating
// "fast" version of that keycode.

#pragma once

#include QMK_KEYBOARD_H

// Sends event_count events instead of a single event.
// Should be called from process_record_user for each fast_keycode.
bool process_fast_keycode(
    uint16_t keycode, keyrecord_t* record,
    uint16_t fast_keycode, uint16_t sent_keycode, uint8_t event_count
);
