#include "qwerty_shortcuts.h"
#include "instant_qwerty.h"

typedef enum {
    QWERTY_SHORTCUTS_MODE,
    SELECTED_LAYOUT_SHORTCUTS_MODE,
} working_mode_t;

static bool _feature_enabled = true;
static uint8_t _mods = 0;
static uint8_t _instant_qwerty_reason = 0;
static bool _selected_layout_shortcuts_mode = true;

static bool _are_shortcut_mods(uint8_t mods) {
    return (mods & MOD_MASK_CAG) && mods != MOD_BIT(KC_RALT);
}

static uint8_t _get_effective_mods(void) {
    return _feature_enabled ? _mods : 0;
}

static void _process_mods_change(uint8_t prev_mods) {
    const bool were_shortcut_mods = _are_shortcut_mods(prev_mods);
    const bool are_shortcut_mods = _are_shortcut_mods(_get_effective_mods());
    if (!were_shortcut_mods && are_shortcut_mods) {
        if (_selected_layout_shortcuts_mode)
            mask_instant_qwerty_activation_reasons(0xff);
        else
            add_instant_qwerty_activation_reasons(_instant_qwerty_reason);
    } else if (were_shortcut_mods && !are_shortcut_mods) {
        if (_selected_layout_shortcuts_mode)
            unmask_instant_qwerty_activation_reasons(0xff);
        else
            del_instant_qwerty_activation_reasons(_instant_qwerty_reason);
    }
}

void enable_qwerty_shortcuts(bool enable) {
    if (_feature_enabled == enable)
        return;

    const uint8_t prev_mods = _get_effective_mods();
    _feature_enabled = enable;
    _process_mods_change(prev_mods);
}

bool is_qwerty_shortcuts_enabled(void) {
    return _feature_enabled;
}

void init_qwerty_shortcuts(uint8_t instant_qwerty_reason) {
    _instant_qwerty_reason = instant_qwerty_reason;
}

bool process_qwerty_shortcuts(
    uint16_t keycode, keyrecord_t *record, uint16_t toggle_feature_keycode
) {
    if (keycode == toggle_feature_keycode) {
        if (record->event.pressed) {
            if (is_qwerty_shortcuts_enabled() && _selected_layout_shortcuts_mode) {
                _selected_layout_shortcuts_mode = false;
                return false;
            }
            _selected_layout_shortcuts_mode = true;
            enable_qwerty_shortcuts(!is_qwerty_shortcuts_enabled());
        }
        return false;
    }
    return true;
}

void post_process_record_qwerty_shortcuts(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mods = get_mods() | get_oneshot_mods();
    if (mods == _mods)
        return;

    const uint8_t prev_mods = _get_effective_mods();
    _mods = mods;
    _process_mods_change(prev_mods);
}
