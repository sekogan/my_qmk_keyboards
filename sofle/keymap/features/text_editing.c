#include "text_editing.h"

bool process_text_navigation_shortcuts(
    uint16_t keycode, keyrecord_t* record,
    uint16_t prev_word_keycode, uint16_t next_word_keycode,
    uint16_t line_start_keycode, uint16_t line_end_keycode
) {
    if (keycode == prev_word_keycode) {
        if (record->event.pressed) {
            if (keymap_config.swap_lctl_lgui)
                register_mods(mod_config(MOD_BIT(KC_LALT)));
            else
                register_mods(mod_config(MOD_BIT(KC_LCTL)));
            register_code(KC_LEFT);
        } else {
            if (keymap_config.swap_lctl_lgui)
                unregister_mods(mod_config(MOD_BIT(KC_LALT)));
            else
                unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
            unregister_code(KC_LEFT);
        }
        return false;
    }
    if (keycode == next_word_keycode) {
        if (record->event.pressed) {
            if (keymap_config.swap_lctl_lgui)
                register_mods(mod_config(MOD_BIT(KC_LALT)));
            else
                register_mods(mod_config(MOD_BIT(KC_LCTL)));
            register_code(KC_RIGHT);
        } else {
            if (keymap_config.swap_lctl_lgui)
                unregister_mods(mod_config(MOD_BIT(KC_LALT)));
            else
                unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
            unregister_code(KC_RIGHT);
        }
        return false;
    }
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
