// Temporarily activates the first default layer (assuming it's Qwerty) saving currently selected
// default layer to the stash.

#pragma once

#include QMK_KEYBOARD_H

// Add reasons to activate Qwerty layout. reasons is a bitmap allowing up to 8 reasons to be used.
void add_instant_qwerty_activation_reasons(uint8_t reasons);

// Remove given reasons to activate Qwerty layout.
void del_instant_qwerty_activation_reasons(uint8_t reasons);

// Add given reasons to the mask making them not valid.
void mask_instant_qwerty_activation_reasons(uint8_t reasons);

// Remove given reasons from the mask.
void unmask_instant_qwerty_activation_reasons(uint8_t reasons);

// Returns true if the Qwerty layout is activated.
bool is_instant_qwerty_activated(void);
