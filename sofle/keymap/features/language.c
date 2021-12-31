#include "language.h"
#include "platform.h"

static language_t _current_language = UNKNOWN_LANGUAGE;

uint8_t _swap_mods(uint8_t desired_mods) {
    uint8_t before_mods = get_mods();
    uint8_t missing_mods = ~before_mods & desired_mods;
    uint8_t undesired_mods = before_mods & ~desired_mods;
    if (undesired_mods)
        unregister_mods(undesired_mods);
    if (missing_mods)
        register_mods(missing_mods);
    return before_mods;
}

language_t get_language(void) {
    return _current_language;
}

void reset_language(void) {
    _current_language = UNKNOWN_LANGUAGE;
}

void switch_language(language_t language) {
    uint8_t mods = get_mods();
    switch (get_platform()) {
        case LINUX_PLATFORM:
            switch (language) {
                case PRIMARY_LANGUAGE:
                    _swap_mods(0);
                    break;
                case SECONDARY_LANGUAGE:
                    _swap_mods(MOD_BIT(KC_LSFT));
                    break;
                default:
                    return;
            }
            tap_code(KC_CAPS);
            _swap_mods(mods);
            break;
        case WINDOWS_PLATFORM:
            _swap_mods(MOD_BIT(KC_LALT)|MOD_BIT(KC_LSFT));
            tap_code(KC_1 + language);
            _swap_mods(mods);
            break;
        default:
            return;
    }
    _current_language = language;
    language_set_user();
}

bool process_language(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case KC_LNG1:
            if (record->event.pressed)
                switch_language(PRIMARY_LANGUAGE);
            return false;
        case KC_LNG2:
            if (record->event.pressed)
                switch_language(SECONDARY_LANGUAGE);
            return false;
    }
    return true;
}

__attribute__((weak)) void language_set_user(void) {}
