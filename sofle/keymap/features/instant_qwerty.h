// Temporarily activates the first default layer (assuming it's Qwerty) saving currently selected
// default layer to the stash.

#pragma once

#include QMK_KEYBOARD_H


void add_instant_qwerty_activation_reasons(uint8_t reasons);

void del_instant_qwerty_activation_reasons(uint8_t reasons);

// Returns true if the Qwerty layout is activated.
bool is_instant_qwerty_activated(void);

// Returns true if the feature is enabled.
bool is_instant_qwerty_enabled(void);

// Enables or disables the feature. The stash is considered empty and all
// the commands do nothing if the feature is disabled.
void enable_instant_qwerty(bool enable);

// Should be called from process_record_user.
bool process_record_instant_qwerty(
    uint16_t keycode, keyrecord_t *record, uint16_t toggle_feature_keycode
);
