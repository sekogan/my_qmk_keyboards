#include QMK_KEYBOARD_H
#include "layers.h"
#include "features/instant_qwerty.h"
#include "features/language_stash.h"
#include "features/language.h"
#include "features/os.h"


#ifdef OLED_ENABLE

static void _print_status_master(void) {
    if (is_os_autodetection_enabled())
        oled_write_ln_P(PSTR("OS  a"), false);
    else
        oled_write_ln_P(PSTR("OS\n"), false);

    switch (get_os()) {
    case WINDOWS_OS:
        oled_write_ln_P(PSTR("Win"), false);
        break;
    case LINUX_OS:
        oled_write_P(PSTR("Linux"), false);
        break;
    case MAC_OS:
        oled_write_ln_P(PSTR("Mac"), false);
        break;
    default:
        oled_write_ln_P(PSTR("?"), false);
    }

    oled_write_ln_P(PSTR("\n\nMODE\n"), false);
    switch (get_highest_layer(default_layer_state)) {
    case QWERTY:
        oled_write_P(is_instant_qwerty_activated() ? PSTR("Qwrt*") : PSTR("Qwrty"), false);
        break;
    case COLEMAK:
        oled_write_P(PSTR("Colmk"), false);
        break;
    default:
        oled_write_ln_P(PSTR("?"), false);
    }

    oled_write_ln_P(PSTR("\n\nLANG\n"), false);
    switch (get_language()) {
    case PRIMARY_LANGUAGE:
        oled_write_ln_P(is_language_stash_empty() ? PSTR("Pri") : PSTR("Pri*"), false);
        break;
    case SECONDARY_LANGUAGE:
        oled_write_ln_P(PSTR("Alt"), false);
        break;
    default:
        oled_write_ln_P(PSTR("?"), false);
    }

    oled_write_ln_P(PSTR("\n"), false);
    const uint8_t mods = get_mods()
#ifndef NO_ACTION_ONESHOT
        | get_oneshot_mods()
#endif // NO_ACTION_ONESHOT
        ;
    oled_write_P(mods & MOD_MASK_SHIFT ? PSTR("S") : PSTR(" "), false);
    oled_write_P(mods & MOD_MASK_GUI ? PSTR("G") : PSTR(" "), false);
    oled_write_P(mods & MOD_MASK_ALT ? PSTR("A") : PSTR(" "), false);
    oled_write_P(mods & MOD_MASK_CTRL ? PSTR("C") : PSTR(" "), false);
}

static void _print_status_slave(void) {
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state)) {
        case COLEMAK:
        case QWERTY:
            oled_write_ln_P(PSTR("Base"), false);
            break;
        case RAISE:
            oled_write_P(PSTR("Raise"), false);
            break;
        case LOWER:
            oled_write_P(PSTR("Lower"), false);
            break;
        case NUMPAD:
            oled_write_ln_P(PSTR("Num"), false);
            break;
        case EXTRA:
            oled_write_P(PSTR("Extra"), false);
            break;
        default:
            oled_write_ln_P(PSTR("?"), false);
    }

    oled_write_ln_P(PSTR("\n"), false);
    const led_t led_usb_state = host_keyboard_led_state();
    oled_write_ln_P(led_usb_state.caps_lock ? PSTR("CAPS") : PSTR(""), false);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    if (is_keyboard_master())
        _print_status_master();
    else
        _print_status_slave();
    return false;
}

#endif // OLED_ENABLE

void suspend_power_down_oled(void) {
#ifdef OLED_ENABLE
    oled_off();
#endif // OLED_ENABLE
}
