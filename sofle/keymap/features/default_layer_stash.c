#include "default_layer_stash.h"

static bool _feature_enabled = true;
static bool _stash_empty = true;
static layer_state_t _stashed_default_layer_state = 0;

enum {
    _FIRST_DEFAULT_LAYER
};

void stash_current_default_layer(void) {
    if (!_feature_enabled)
        return;
    if (get_highest_layer(default_layer_state) != _FIRST_DEFAULT_LAYER) {
        _stashed_default_layer_state = default_layer_state;
        default_layer_set(1 << _FIRST_DEFAULT_LAYER);
        _stash_empty = false;
    }
}

void restore_stashed_default_layer(void) {
    if (!_stash_empty) {
        if (default_layer_state != _stashed_default_layer_state)
            default_layer_set(_stashed_default_layer_state);
        _stash_empty = true;
    }
}

bool is_default_layer_stash_empty(void) {
    return _stash_empty;
}

void clear_default_layer_stash(void) {
    _stash_empty = true;
}

bool is_default_layer_stash_enabled(void) {
    return _feature_enabled;
}

void enable_default_layer_stash(bool enable) {
    _feature_enabled = enable;
}

bool process_default_layer_stash(
    uint16_t keycode, keyrecord_t *record, uint16_t toggle_feature_keycode
) {
    if (keycode == toggle_feature_keycode) {
        if (record->event.pressed) {
            enable_default_layer_stash(!is_default_layer_stash_enabled());
            clear_default_layer_stash();
        }
        return false;
    }
    return true;
}
