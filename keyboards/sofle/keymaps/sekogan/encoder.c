// Copyright 2022-2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#include "encoder.h"

bool process_encoder_keycodes(uint16_t keycode, keyrecord_t* record, uint16_t l_ccw, uint16_t l_cw, uint16_t r_ccw, uint16_t r_cw) {
    const bool is_left  = keycode == l_ccw || keycode == l_cw;
    const bool is_right = keycode == r_ccw || keycode == r_cw;
    if (!is_left && !is_right) return true;
    if (!record->event.pressed) return false;

    const uint8_t mods      = get_mods();
    const bool    clockwise = keycode == l_cw || keycode == r_cw;
    if (is_left) {
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
    } else {
        if (mods & (MOD_MASK_CTRL | MOD_MASK_ALT | MOD_MASK_GUI) && !(mods & MOD_MASK_SHIFT)) {
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
