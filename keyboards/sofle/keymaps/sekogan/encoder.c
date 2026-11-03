#include QMK_KEYBOARD_H


#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    const uint8_t mods = get_mods();
    if (index == 0) {
        if (mods & MOD_MASK_CTRL) {
            if (clockwise)
                tap_code(KC_MNXT);
            else
                tap_code(KC_MPRV);
        } else {
            if (clockwise)
                tap_code(KC_VOLU);
            else
                tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        if (mods & (MOD_MASK_CTRL|MOD_MASK_ALT|MOD_MASK_GUI) && !(mods & MOD_MASK_SHIFT)) {
            if (clockwise)
                tap_code(KC_TAB);
            else
                tap_code16(S(KC_TAB));
        } else {
            if (clockwise)
                tap_code(KC_PGDN);
            else
                tap_code(KC_PGUP);
        }
    }
    return false;
}

#endif // ENCODER_ENABLE
