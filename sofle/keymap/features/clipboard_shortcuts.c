#include "clipboard.h"

bool process_clipboard_shortcuts(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case KC_COPY:
            if (record->event.pressed) {
                add_mods(mod_config(MOD_BIT(KC_LCTL)));
                register_code(KC_C);
            } else {
                del_mods(mod_config(MOD_BIT(KC_LCTL)));
                unregister_code(KC_C);
            }
            return false;
        case KC_PASTE:
            if (record->event.pressed) {
                add_mods(mod_config(MOD_BIT(KC_LCTL)));
                register_code(KC_V);
            } else {
                del_mods(mod_config(MOD_BIT(KC_LCTL)));
                unregister_code(KC_V);
            }
            return false;
        case KC_CUT:
            if (record->event.pressed) {
                add_mods(mod_config(MOD_BIT(KC_LCTL)));
                register_code(KC_X);
            } else {
                del_mods(mod_config(MOD_BIT(KC_LCTL)));
                unregister_code(KC_X);
            }
            return false;
    }
    return true;
}
