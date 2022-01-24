#include "instant_qwerty.h"

static bool _stash_empty = true;
static layer_state_t _stashed_default_layer_state = 0;
static uint8_t _activation_reasons = 0;
static uint8_t _activation_reasons_mask = 0xff;

enum {
    _FIRST_DEFAULT_LAYER
};

static void _stash_current_default_layer(void) {
    if (get_highest_layer(default_layer_state) != _FIRST_DEFAULT_LAYER) {
        _stashed_default_layer_state = default_layer_state;
        default_layer_set(1 << _FIRST_DEFAULT_LAYER);
        _stash_empty = false;
    }
}

static void _restore_stashed_default_layer(void) {
    if (!_stash_empty) {
        if (default_layer_state != _stashed_default_layer_state)
            default_layer_set(_stashed_default_layer_state);
        _stash_empty = true;
    }
}

static uint8_t _get_effective_activation_reasons(void) {
    return _activation_reasons & _activation_reasons_mask;
}

static void _process_activation_reason_change(uint8_t prev_reasons) {
    const uint8_t current_reasons = _get_effective_activation_reasons();
    if (prev_reasons && !current_reasons)
        _restore_stashed_default_layer();
    else if (!prev_reasons && current_reasons)
        _stash_current_default_layer();
}

void add_instant_qwerty_activation_reasons(uint8_t reasons) {
    const uint8_t prev_reasons = _get_effective_activation_reasons();
    _activation_reasons |= reasons;
    _process_activation_reason_change(prev_reasons);
}

void del_instant_qwerty_activation_reasons(uint8_t reasons) {
    const uint8_t prev_reasons = _get_effective_activation_reasons();
    _activation_reasons &= ~reasons;
    _process_activation_reason_change(prev_reasons);
}

void mask_instant_qwerty_activation_reasons(uint8_t reasons) {
    const uint8_t prev_reasons = _get_effective_activation_reasons();
    _activation_reasons_mask &= ~reasons;
    _process_activation_reason_change(prev_reasons);
}

void unmask_instant_qwerty_activation_reasons(uint8_t reasons) {
    const uint8_t prev_reasons = _get_effective_activation_reasons();
    _activation_reasons_mask |= reasons;
    _process_activation_reason_change(prev_reasons);
}

bool is_instant_qwerty_activated(void) {
    return !_stash_empty;
}
