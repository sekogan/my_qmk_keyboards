// Replaces exotic clipboard-related keycodes with conventional key combinations.

#pragma once

#include QMK_KEYBOARD_H

// Should be called from process_record_user.
bool process_text_navigation_shortcuts(
    uint16_t keycode, keyrecord_t* record,
    uint16_t prev_word_keycode, uint16_t next_word_keycode,
    uint16_t line_start_keycode, uint16_t line_end_keycode
);

// Should be called from process_record_user.
bool process_editing_history_shortcuts(uint16_t keycode, keyrecord_t* record);

// Should be called from process_record_user.
bool process_text_deleting_macros(
    uint16_t keycode, keyrecord_t* record,
    uint16_t delete_forward_keycode, uint16_t delete_backward_keycode
);
