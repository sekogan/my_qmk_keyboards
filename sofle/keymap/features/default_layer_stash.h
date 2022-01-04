// Temporarily activates the first default layer saving currently selected default layer to
// the stash.

#pragma once

#include QMK_KEYBOARD_H

// Makes sure the first default layer is selected.
// If it is not the case, puts current default layer to the stash (overwriting existing value
// if the stash is not empty) and switches to the first default layer.
void stash_current_default_layer(void);

// Switches to the stashed default layer and clears the stash if it is not empty.
// Does nothing otherwise.
void restore_stashed_default_layer(void);

// Returns true if the stash is not empty.
bool is_default_layer_stash_empty(void);

// Clears the stash if it is not empty. Does nothing otherwise.
void clear_default_layer_stash(void);

// Returns true if the feature is enabled.
bool is_default_layer_stash_enabled(void);

// Enables or disables the feature. The stash is considered empty and all
// the commands do nothing if the feature is disabled.
void enable_default_layer_stash(bool enable);

// Should be called from process_record_user.
bool process_default_layer_stash(
    uint16_t keycode, keyrecord_t *record, uint16_t toggle_feature_keycode
);
