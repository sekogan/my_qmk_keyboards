#include "qwerty_shortcuts.h"
#include "instant_qwerty.h"

static bool _feature_enabled = true;
static uint8_t _mods = 0;
static uint8_t _instant_qwerty_reason = 0;
static uint16_t _toggle_feature_keycode = KC_NO;

inline bool _is_shortcut_mods(uint8_t mods) {
    return mods & MOD_MASK_CAG;
}

void _enable_qwerty_shortcuts(bool enable) {
    if (_feature_enabled == enable)
        return;

    _feature_enabled = enable;
    if (_feature_enabled) {
        if (_is_shortcut_mods(_mods))
            add_instant_qwerty_activation_reasons(_instant_qwerty_reason);
    } else {
        if (_is_shortcut_mods(_mods))
            del_instant_qwerty_activation_reasons(_instant_qwerty_reason);
    }
}

bool is_qwerty_shortcuts_enabled(void) {
    return _feature_enabled;
}

void init_qwerty_shortcuts(uint16_t toggle_feature_keycode, uint8_t instant_qwerty_reason) {
    _toggle_feature_keycode = toggle_feature_keycode;
    _instant_qwerty_reason = instant_qwerty_reason;
}

bool process_record_qwerty_shortcuts(uint16_t keycode, keyrecord_t *record) {
    if (keycode == _toggle_feature_keycode) {
        if (record->event.pressed)
            _enable_qwerty_shortcuts(!is_qwerty_shortcuts_enabled());
        return false;
    }
    return true;
}

void post_process_record_qwerty_shortcuts(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mods = get_mods() | get_oneshot_mods();
    if (mods == _mods)
        return;

    const bool were_shortcut_mods = _is_shortcut_mods(_mods);
    _mods = mods;
    if (were_shortcut_mods != _is_shortcut_mods(_mods)) {
        if (were_shortcut_mods)
            del_instant_qwerty_activation_reasons(_instant_qwerty_reason);
        else
            add_instant_qwerty_activation_reasons(_instant_qwerty_reason);
    }
}
