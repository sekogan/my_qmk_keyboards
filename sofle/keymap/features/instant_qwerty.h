// Temporarily activates the first default layer (assuming it's Qwerty) saving currently selected
// default layer to the stash.

#pragma once

#include QMK_KEYBOARD_H


// Add reasons to activate Qwerty layout. reasons is a bitmap allowing up to 8 reasons to be used.
void add_instant_qwerty_activation_reasons(uint8_t reasons);

// Remove given reasons to activate Qwerty layout.
void del_instant_qwerty_activation_reasons(uint8_t reasons);

// Returns true if the Qwerty layout is activated.
bool is_instant_qwerty_activated(void);

// Returns true if the feature is enabled.
bool is_instant_qwerty_enabled(void);

// Should be called from keyboard_post_init_user.
void init_instant_qwerty(uint16_t toggle_feature_keycode);

// Should be called from process_record_user.
bool process_record_instant_qwerty(uint16_t keycode, keyrecord_t *record);
