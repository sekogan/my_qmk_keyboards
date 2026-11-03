#include "text_editing.h"

typedef enum {
    LEFT_BRACKET_REGISTERED_FLAG = 1,
    RIGHT_BRACKET_REGISTERED_FLAG = 2,
} word_navigation_flags_t;

static uint8_t _word_navigation_state = 0;

static bool _process_word_navigation_shortcuts(
    uint16_t keycode, keyrecord_t* record,
    uint16_t trigger_keycode,
    uint16_t word_jump_keycode, uint16_t bracket_keycode,
    word_navigation_flags_t bracket_registered_flag
) {
    if (keycode == trigger_keycode) {
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
            const uint8_t all_mods = mods | get_oneshot_mods();
#else
            const uint8_t all_mods = mods;
#endif  // NO_ACTION_ONESHOT
            if (all_mods & MOD_MASK_CTRL) {
                register_code(bracket_keycode);
                _word_navigation_state |= bracket_registered_flag;
            } else {
                if (keymap_config.swap_lctl_lgui)
                    register_mods(mod_config(MOD_BIT(KC_LALT)));
                else
                    register_mods(mod_config(MOD_BIT(KC_LCTL)));
                register_code(word_jump_keycode);
            }
        } else {
            if (_word_navigation_state & bracket_registered_flag) {
                _word_navigation_state &= ~bracket_registered_flag;
                unregister_code(bracket_keycode);
            } else {
                if (keymap_config.swap_lctl_lgui)
                    unregister_mods(mod_config(MOD_BIT(KC_LALT)));
                else
                    unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
                unregister_code(word_jump_keycode);
            }
        }
        return false;
    }
    return true;
}

bool process_text_navigation_shortcuts(
    uint16_t keycode, keyrecord_t* record,
    uint16_t prev_word_keycode, uint16_t next_word_keycode,
    uint16_t line_start_keycode, uint16_t line_end_keycode
) {
    if (!_process_word_navigation_shortcuts(
        keycode, record, prev_word_keycode, KC_LEFT, KC_LBRC, LEFT_BRACKET_REGISTERED_FLAG
    ))
        return false;
    if (!_process_word_navigation_shortcuts(
        keycode, record, next_word_keycode, KC_RIGHT, KC_RBRC, RIGHT_BRACKET_REGISTERED_FLAG
    ))
        return false;

    if (keycode == line_start_keycode) {
        if (record->event.pressed) {
            if (keymap_config.swap_lctl_lgui) {
                    // CMD-arrow on Mac, but we have CTL and GUI swapped
                register_mods(mod_config(MOD_BIT(KC_LCTL)));
                register_code(KC_LEFT);
            } else {
                register_code(KC_HOME);
            }
        } else {
            if (keymap_config.swap_lctl_lgui) {
                unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
                unregister_code(KC_LEFT);
            } else {
                unregister_code(KC_HOME);
            }
        }
        return false;
    }
    if (keycode == line_end_keycode) {
        if (record->event.pressed) {
            if (keymap_config.swap_lctl_lgui) {
                // CMD-arrow on Mac, but we have CTL and GUI swapped
                register_mods(mod_config(MOD_BIT(KC_LCTL)));
                register_code(KC_RIGHT);
            } else {
                register_code(KC_END);
            }
        } else {
            if (keymap_config.swap_lctl_lgui) {
                unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
                unregister_code(KC_RIGHT);
            } else {
                unregister_code(KC_END);
            }
        }
        return false;
    }
    return true;
}

bool process_editing_history_shortcuts(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case KC_UNDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_BIT(KC_LCTL)));
                register_code(KC_Z);
            } else {
                unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
                unregister_code(KC_Z);
            }
            return false;
    }
    return true;
}

bool process_text_deleting_macros(
    uint16_t keycode, keyrecord_t* record,
    uint16_t delete_forward_keycode, uint16_t delete_backward_keycode
) {
    if (keycode == delete_forward_keycode) {
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            if (mods & MOD_MASK_CTRL) {
                unregister_mods(mods);
                SEND_STRING(SS_LSFT(SS_TAP(X_END)) SS_TAP(X_DEL));
                register_mods(mods);
            } else {
                SEND_STRING(SS_LCTL(SS_TAP(X_DEL)));
            }
        }
        return false;
    } else if (keycode == delete_backward_keycode) {
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            if (mods & MOD_MASK_CTRL) {
                unregister_mods(mods);
                SEND_STRING(SS_LSFT(SS_TAP(X_HOME)SS_TAP(X_HOME)) SS_TAP(X_DEL));
                register_mods(mods);
            } else {
                SEND_STRING(SS_LCTL(SS_TAP(X_BSPC)));
            }
        }
        return false;
    }
    return true;
}
