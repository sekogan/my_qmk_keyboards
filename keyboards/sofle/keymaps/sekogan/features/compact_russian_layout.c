#include "compact_russian_layout.h"

static bool _feature_enabled = false;
static uint8_t _registered_keycodes = 0;
static bool _ralt_pressed = false;

enum {
    _GRV_BIT = 1,
    _RBRC_BIT = 2,
};

void enable_compact_russian_layout(bool enable) {
    _feature_enabled = enable;
}

static bool _process_keycode(
    uint16_t keycode, keyrecord_t* record,
    uint16_t replaced_keycode, uint16_t replacement_keycode, uint8_t replacement_keycode_bit
) {
    if (keycode != replaced_keycode) return true;
    if (record->event.pressed) {
        if (_feature_enabled && _ralt_pressed) {
            if (get_mods() & MOD_BIT(KC_RALT)) {
                unregister_code(KC_RALT);

                // Tapped Alt activates window menu in some cases. Tap it again to dismiss.
                tap_code(KC_RALT);
            }

            register_code(replacement_keycode);
            _registered_keycodes |= replacement_keycode_bit;

            return false;
        }
    } else {
        if (_registered_keycodes & replacement_keycode_bit) {
            unregister_code(replacement_keycode);
            _registered_keycodes &= ~replacement_keycode_bit;
            return false;
        }
    }
    return true;
}

bool process_compact_russian_layout(uint16_t keycode, keyrecord_t* record) {
    if (keycode == KC_RALT) {
        _ralt_pressed = record->event.pressed;
        if (_registered_keycodes)
            return false;
        return true;
    }
    if (!_process_keycode(keycode, record, KC_T, KC_GRV, _GRV_BIT)) return false;
    if (!_process_keycode(keycode, record, KC_M, KC_RBRC, _RBRC_BIT)) return false;
    return true;
}
