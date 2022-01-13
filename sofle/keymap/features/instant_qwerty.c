#include "instant_qwerty.h"

static bool _feature_enabled = true;
static bool _stash_empty = true;
static layer_state_t _stashed_default_layer_state = 0;
static uint8_t _activation_reasons = 0;
static uint16_t _toggle_feature_keycode = KC_NO;

enum {
    _FIRST_DEFAULT_LAYER
};

void _stash_current_default_layer(void) {
    if (!_feature_enabled)
        return;
    if (get_highest_layer(default_layer_state) != _FIRST_DEFAULT_LAYER) {
        _stashed_default_layer_state = default_layer_state;
        default_layer_set(1 << _FIRST_DEFAULT_LAYER);
        _stash_empty = false;
    }
}

void _restore_stashed_default_layer(void) {
    if (!_stash_empty) {
        if (default_layer_state != _stashed_default_layer_state)
            default_layer_set(_stashed_default_layer_state);
        _stash_empty = true;
    }
}

void add_instant_qwerty_activation_reasons(uint8_t reasons) {
    const uint8_t prev_reasons = _activation_reasons;
    _activation_reasons |= reasons;
    if (!prev_reasons && _activation_reasons)
        _stash_current_default_layer();
}

void del_instant_qwerty_activation_reasons(uint8_t reasons) {
    const uint8_t prev_reasons = _activation_reasons;
    _activation_reasons &= ~reasons;
    if (prev_reasons && !_activation_reasons)
        _restore_stashed_default_layer();
}

bool is_instant_qwerty_activated(void) {
    return !_stash_empty;
}

bool is_instant_qwerty_enabled(void) {
    return _feature_enabled;
}

void _enable_instant_qwerty(bool enable) {
    if (_feature_enabled == enable)
        return;

    _feature_enabled = enable;
    if (_feature_enabled) {
        if (_activation_reasons)
            _stash_current_default_layer();
    } else {
        if (_activation_reasons)
            _restore_stashed_default_layer();
    }
}

void init_instant_qwerty(uint16_t toggle_feature_keycode) {
    _toggle_feature_keycode = toggle_feature_keycode;
}

bool process_record_instant_qwerty(uint16_t keycode, keyrecord_t *record) {
    if (keycode && keycode == _toggle_feature_keycode) {
        if (record->event.pressed)
            _enable_instant_qwerty(!is_instant_qwerty_enabled());
        return false;
    }
    return true;
}
