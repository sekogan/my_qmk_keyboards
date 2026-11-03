#include "clipboard_shortcuts.h"

bool process_clipboard_shortcuts(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case KC_COPY:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_BIT(KC_LCTL)));
                register_code(KC_C);
            } else {
                unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
                unregister_code(KC_C);
            }
            return false;
        case KC_PASTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_BIT(KC_LCTL)));
                register_code(KC_V);
            } else {
                unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
                unregister_code(KC_V);
            }
            return false;
        case KC_CUT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_BIT(KC_LCTL)));
                register_code(KC_X);
            } else {
                unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
                unregister_code(KC_X);
            }
            return false;
    }
    return true;
}
