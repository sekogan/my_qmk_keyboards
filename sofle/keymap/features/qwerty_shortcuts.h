// Temporarily cancels "instant qwerty" feature if shortcut mods are pressed.
// Depends on "instant qwerty" feature.

#pragma once

#include QMK_KEYBOARD_H

// Should be called from keyboard_post_init_user.
void init_qwerty_shortcuts(uint8_t instant_qwerty_reason);

// Should be called from process_record_user.
bool process_qwerty_shortcuts(
    uint16_t keycode, keyrecord_t *record, uint16_t toggle_feature_keycode
);

// Should be called from post_process_record_user.
void post_process_record_qwerty_shortcuts(uint16_t keycode, keyrecord_t *record);

// Returns true if the feature is enabled.
bool is_qwerty_shortcuts_enabled(void);

// Enables or disables the feature.
void enable_qwerty_shortcuts(bool enable);
