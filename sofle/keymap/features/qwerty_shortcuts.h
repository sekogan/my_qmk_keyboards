// Temporarily activates the first default layer if any mod except Shift is pressed.

#pragma once

#include QMK_KEYBOARD_H

// Returns true if the feature is enabled.
bool is_qwerty_shortcuts_enabled(void);

// Should be called from keyboard_post_init_user.
void init_qwerty_shortcuts(uint16_t toggle_feature_keycode, uint8_t instant_qwerty_reason);

// Should be called from process_record_user.
bool process_record_qwerty_shortcuts(uint16_t keycode, keyrecord_t *record);

// Should be called from post_process_record_user.
void post_process_record_qwerty_shortcuts(uint16_t keycode, keyrecord_t *record);
