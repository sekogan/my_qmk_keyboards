// Replaces exotic clipboard-related keycodes with conventional key combinations.

#pragma once

#include QMK_KEYBOARD_H

// Should be called from process_record_user.
bool process_clipboard_shortcuts(uint16_t keycode, keyrecord_t* record);
