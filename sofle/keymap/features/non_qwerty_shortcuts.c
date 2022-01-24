#include "non_qwerty_shortcuts.h"
#include "instant_qwerty.h"

static uint8_t _mods = 0;

static bool _are_shortcut_mods(uint8_t mods) {
    return (mods & MOD_MASK_CAG) && mods != MOD_BIT(KC_RALT);
}

static void _process_mods_change(uint8_t prev_mods) {
    const bool were_shortcut_mods = _are_shortcut_mods(prev_mods);
    const bool are_shortcut_mods = _are_shortcut_mods(_mods);
    if (!were_shortcut_mods && are_shortcut_mods)
        mask_instant_qwerty_activation_reasons(0xff);
    else if (were_shortcut_mods && !are_shortcut_mods)
        unmask_instant_qwerty_activation_reasons(0xff);
}

void post_process_record_non_qwerty_shortcuts(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mods = get_mods() | get_oneshot_mods();
    if (mods == _mods)
        return;

    const uint8_t prev_mods = _mods;
    _mods = mods;
    _process_mods_change(prev_mods);
}
