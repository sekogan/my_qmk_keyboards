#include QMK_KEYBOARD_H
#include "features/caps_word.h"
#include "features/compact_russian.h"
#include "features/language.h"
#include "features/language_stash.h"
#include "features/platform.h"
#include "features/select_word.h"

enum layers {
    _QWERTY,
    _COLEMAK,
    _LOWER,
    _RAISE,
    _EXTRA,
};

enum custom_keycodes {
    KC_QWRT = SAFE_RANGE,   // Qwerty
    KC_CLMK,                // Colemak

    KC_LIN,
    KC_WIN,
    KC_MAC,

    KC_LOWER,
    KC_RAISE,

    KC_LNGST,               // Toggle language stash feature

    KC_WSEL,                // Select word

    KC_WPREV,               // Previous word
    KC_WNEXT,               // Next word
    KC_LSTRT,               // Line start
    KC_LEND,                // Line end

    KC_FUP,                 // Fast up
    KC_FDOWN,               // Fast down

    KC_LKSH                 // Locked Shift
};


#define MT_LGUI     LGUI_T(KC_LNG1)
#define MT_RGUI     RGUI_T(KC_LNG2)

#define KC_LDEL     C(S(KC_K))
#define KC_WBSPC    C(KC_BSPC)
#define KC_WDEL     C(KC_DEL)

#define OSM_LSFT    OSM(MOD_LSFT)
#define OSM_RSFT    OSM(MOD_RSFT)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* ,-----------------------------------------.                    ,-----------------------------------------.
 * |  1   |  2   |  3   |  4   |  5   |  6   |                    |  7   |  8   |  9   |  0   |  [   |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |  Q   |  W   |  E   |  R   |  T   |                    |  Y   |  U   |  I   |  O   |  P   | BSPC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |  A   |  S   |  D   |  F   |  G   |,------.    ,------.|  H   |  J   |  K   |  L   | ; :  | ' "  |
 * |------+------+------+------+------+------|| MPLY |    |      ||------+------+------+------+------+------|
 * |Shift |  Z   |  X   |  C   |  V   |  B   |`------'    `------'|  N   |  M   | , <  | . >  | / ?  |Shift |
 * `-------------+------+------+------+-.------------.    ,------------.-+------+------+------+-------------'
 *               |Super | LALT |LCTRL |/LOWER / SPC  /    \ ENT  \RAISE \|RCTRL | RALT |Super |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[_QWERTY] = LAYOUT( \
KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,                         KC_7,     KC_8,     KC_9,     KC_0,     KC_LBRC,  XXXXXXX, \
KC_ESC,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,                         KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSPC, \
KC_TAB,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,                         KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT, \
OSM_LSFT, KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_MPLY,  XXXXXXX,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  OSM_RSFT, \
                    MT_LGUI,  KC_LALT,  KC_LCTRL, KC_LOWER, KC_SPC,   KC_ENT,   KC_RAISE, KC_RCTRL, KC_RALT,  MT_RGUI \
),

/* ,-----------------------------------------.                    ,-----------------------------------------.
 * |  1   |  2   |  3   |  4   |  5   |  6   |                    |  7   |  8   |  9   |  0   |  [   |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |  Q   |  W   |  F   |  P   |  G   |                    |  J   |  L   |  U   |  Y   | ; :  | BSPC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |  A   |  R   |  S   |  T   |  D   |,------.    ,------.|  H   |  N   |  E   |  I   |  O   | ' "  |
 * |------+------+------+------+------+------|| MPLY |    |      ||------+------+------+------+------+------|
 * |Shift |  Z   |  X   |  C   |  V   |  B   |`------'    `------'|  K   |  M   | , <  | . >  | / ?  |Shift |
 * `-------------+------+------+------+-.------------.    ,------------.-+------+------+------+-------------'
 *               |Super | LALT |LCTRL |/LOWER / SPC  /    \ ENT  \RAISE \|RCTRL | RALT |Super |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[_COLEMAK] = LAYOUT( \
KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,                         KC_7,     KC_8,     KC_9,     KC_0,     KC_LBRC,  XXXXXXX, \
KC_ESC,   KC_Q,     KC_W,     KC_F,     KC_P,     KC_G,                         KC_J,     KC_L,     KC_U,     KC_Y,     KC_SCLN,  KC_BSPC, \
KC_TAB,   KC_A,     KC_R,     KC_S,     KC_T,     KC_D,                         KC_H,     KC_N,     KC_E,     KC_I,     KC_O,     KC_QUOT, \
OSM_LSFT, KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_MPLY,  XXXXXXX,  KC_K,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  OSM_RSFT, \
                    MT_LGUI,  KC_LALT,  KC_LCTRL, KC_LOWER, KC_SPC,   KC_ENT,   KC_RAISE, KC_RCTRL, KC_RALT,  MT_RGUI \
),

/* ,-----------------------------------------.                    ,-----------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  | F10  | F11  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  !   |  @   |  #   |  $   |  %   |  ^   |                    |  &   |  *   |  (   |  )   | \ |  | BSPC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ` ~  |  ~   |  +   | = +  | - _  |  _   |,------.    ,------.| ' "  |  "   |  [   |  ]   |  :   |  |   |
 * |------+------+------+------+------+------|| ___  |    | ___  ||------+------+------+------+------+------|
 * | ___  |      |      |      | ; :  | / ?  |`------'    `------'|  {   |  }   |  <   |  >   |  ?   | ___  |
 * `-------------+------+------+------+-.------------.    ,------------.-+------+------+------+-------------'
 *               | LGUI | ___  | ___  |/ ___  / ___  /    \ ___  \ ___  \| ___  | ___  | ___  |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[_LOWER] = LAYOUT( \
KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,                        KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12, \
KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_CIRC,                      KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,  KC_BSLS,  KC_BSPC, \
KC_GRV,   KC_TILD,  KC_PLUS,  KC_EQL,   KC_MINS,  KC_UNDS,                      KC_QUOT,  KC_DQUO,  KC_LBRC,  KC_RBRC,  KC_COLN,  KC_PIPE, \
_______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_SCLN,  KC_SLSH,  _______,  _______,  KC_LCBR,  KC_RCBR,  KC_LT,    KC_GT,    KC_QUES,  _______, \
                    KC_LGUI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, _______\
),

/* ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      | CAPS |                    |      |      | FUP  | PSCR | INS  | PAUS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  | MEH  | HYPR |      | LKSH | ` ~  |                    | PGUP |WPREV |  UP  |WNEXT | DEL  | BSPC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  | LSFT | LALT | LCTL | TAB  |      |,------.    ,------.| PGDN | LEFT | DOWN | RGHT | WDEL |WBSPC |
 * |------+------+------+------+------+------|| ___  |    | ___  ||------+------+------+------+------+------|
 * | ___  | UNDO | CUT  | COPY |PASTE |      |`------'    `------'| WSEL |LSTRT |FDOWN | LEND | LDEL | ___  |
 * `-------------+------+------+------+-.------------.    ,------------.-+------+------+------+-------------'
 *               | ___  | ___  | ___  |/ ___  / ___  /    \ ___  \ ___  \| ___  | ___  | ___  |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[_RAISE] = LAYOUT( \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_CAPS,                      XXXXXXX,  XXXXXXX,  KC_FUP,   KC_PSCR,  KC_INS,    KC_PAUS, \
KC_ESC,   KC_MEH,   KC_HYPR,  XXXXXXX,  KC_LKSH,  KC_GRV,                       KC_PGUP,  KC_WPREV, KC_UP,    KC_WNEXT, KC_DEL,    KC_BSPC, \
KC_TAB,   KC_LSFT,  KC_LALT,  KC_LCTL,  KC_TAB,   XXXXXXX,                      KC_PGDN,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_WDEL,   KC_WBSPC, \
_______,  KC_UNDO,  KC_CUT,   KC_COPY,  KC_PASTE, XXXXXXX,  _______,  _______,  KC_WSEL,  KC_LSTRT, KC_FDOWN, KC_LEND,  KC_LDEL,   _______, \
                    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______ \
),

/* ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      | QWRT |                    | LIN  |      |      |      |      |RESET |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      | CLMK |,------.    ,------.| WIN  |      |      |LNGST |      |      |
 * |------+------+------+------+------+------||      |    |      ||------+------+------+------+------+------|
 * | ___  |      |      |      |      |      |`------'    `------'| MAC  |      |      |      |      | ___  |
 * `-------------+------+------+------+-.------------.    ,------------.-+------+------+------+-------------'
 *               | ___  | ___  | ___  |/ ___  / ___  /    \ ___  \ ___  \| ___  | ___  | ___  |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[_EXTRA] = LAYOUT( \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                      XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_QWRT,                      KC_LIN,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  RESET, \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_CLMK,                      KC_WIN,   XXXXXXX,  XXXXXXX,  KC_LNGST, XXXXXXX,  XXXXXXX, \
_______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MAC,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______, \
                    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______ \
)

};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode != KC_LOWER && keycode != KC_RAISE)
        if (!process_caps_word(keycode, record)) return false;
    if (!process_select_word(keycode, record, KC_WSEL)) return false;
    if (!process_compact_russian(keycode, record)) return false;
    if (!process_language(keycode, record)) return false;
    if (!process_language_stash(keycode, record, KC_LNGST)) return false;
    if (!process_platform_selector(keycode, record, KC_LIN, LINUX_PLATFORM)) return false;
    if (!process_platform_selector(keycode, record, KC_WIN, WINDOWS_PLATFORM)) return false;
    if (!process_platform_selector(keycode, record, KC_MAC, MAC_PLATFORM)) return false;

    switch (keycode) {
        case KC_QWRT:
            if (record->event.pressed)
                set_single_persistent_default_layer(_QWERTY);
            return false;
        case KC_CLMK:
            if (record->event.pressed)
                set_single_persistent_default_layer(_COLEMAK);
            return false;
        case KC_LOWER:
            if (record->event.pressed)
                layer_on(_LOWER);
            else
                layer_off(_LOWER);
            update_tri_layer(_LOWER, _RAISE, _EXTRA);
            return false;
        case KC_RAISE:
            if (record->event.pressed)
                layer_on(_RAISE);
            else
                layer_off(_RAISE);
            update_tri_layer(_LOWER, _RAISE, _EXTRA);
            return false;
        case KC_FUP:
            if (record->event.pressed) {
                tap_code(KC_UP);
                tap_code(KC_UP);
                tap_code(KC_UP);
            }
            return false;
        case KC_FDOWN:
            if (record->event.pressed) {
                tap_code(KC_DOWN);
                tap_code(KC_DOWN);
                tap_code(KC_DOWN);
            }
            return false;
        case KC_WPREV:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_BIT(KC_LALT)));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_BIT(KC_LCTL)));
                    register_code(KC_LEFT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_BIT(KC_LALT)));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
                    unregister_code(KC_LEFT);
                }
            }
            break;
        case KC_WNEXT:
             if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_BIT(KC_LALT)));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_BIT(KC_LCTL)));
                    register_code(KC_RIGHT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_BIT(KC_LALT)));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
                    unregister_code(KC_RIGHT);
                }
            }
            break;
        case KC_LSTRT:
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
            break;
        case KC_LEND:
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
            break;
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
            break;
        case KC_UNDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_BIT(KC_LCTL)));
                register_code(KC_Z);
            } else {
                unregister_mods(mod_config(MOD_BIT(KC_LCTL)));
                unregister_code(KC_Z);
            }
            return false;
        case KC_LKSH:
            if (record->event.pressed) {
                if ((get_mods() & MOD_BIT(KC_LSFT)) == MOD_BIT(KC_LSFT))
                    unregister_mods(MOD_BIT(KC_LSFT));
                else
                    register_mods(MOD_BIT(KC_LSFT));
            }
            return false;
        case MT_LGUI:
            if (record->tap.count == 1 && record->event.pressed)
                return process_record_user(KC_LNG1, record);
            break;
        case MT_RGUI:
            if (record->tap.count == 1 && record->event.pressed)
                return process_record_user(KC_LNG2, record);
            break;
    }
    return true;
}


layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _LOWER:
        stash_current_language();
        break;
    default:
        restore_stashed_language();
    }
    return state;
}


void platform_set_user(void) {
    reset_language();
    clear_language_stash();
}


void language_set_user(void) {
    clear_language_stash();
    enable_compact_russian(get_language() == SECONDARY_LANGUAGE);
}


void suspend_power_down_user(void) {
    oled_off();
}


#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t mods = get_mods();
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
                tap_code(KC_PGDOWN);
            else
                tap_code(KC_PGUP);
        }
    }
    return true;
}

#endif // ENCODER_ENABLE


// The "led_set_user" is called in initialization and any time the "lock" state is changed for any of the states.
// It checks the NUM LOCK state, and if it's disabled, sends the "numlock" key press to enable it.
void led_set_user(uint8_t usb_led) {
    if (!IS_LED_ON(usb_led, USB_LED_NUM_LOCK))
        tap_code(KC_NUMLOCK);
}


#ifdef OLED_ENABLE

static void print_status_master(void) {
    oled_write_ln_P(PSTR("OS\n"), false);
    switch (get_platform()) {
        case WINDOWS_PLATFORM:
            oled_write_ln_P(PSTR("Win"), false);
            break;
        case LINUX_PLATFORM:
            oled_write_P(PSTR("Linux"), false);
            break;
        case MAC_PLATFORM:
            oled_write_ln_P(PSTR("Mac"), false);
            break;
        default:
            oled_write_ln_P(PSTR("?"), false);
    }

    oled_write_ln_P(PSTR("\n\nMODE\n"), false);
    switch (get_highest_layer(default_layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("Qwrty"), false);
            break;
        case _COLEMAK:
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
            oled_write_ln_P(is_language_stash_enabled() ? PSTR("Alt*") : PSTR("Alt"), false);
            break;
        default:
            oled_write_ln_P(PSTR("?"), false);
    }

    oled_write_ln_P(PSTR("\n"), false);
    uint8_t mods = get_mods()
#ifndef NO_ACTION_ONESHOT
        | get_oneshot_mods()
#endif // NO_ACTION_ONESHOT
        ;
    oled_write_P(mods & MOD_MASK_SHIFT ? PSTR("S") : PSTR(" "), false);
    oled_write_P(mods & MOD_MASK_GUI ? PSTR("G") : PSTR(" "), false);
    oled_write_P(mods & MOD_MASK_ALT ? PSTR("A") : PSTR(" "), false);
    oled_write_P(mods & MOD_MASK_CTRL ? PSTR("C") : PSTR(" "), false);
}

static void print_status_slave(void) {
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state)) {
        case _COLEMAK:
        case _QWERTY:
            oled_write_ln_P(PSTR("Base"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("Lower"), false);
            break;
        case _EXTRA:
            oled_write_P(PSTR("Extra"), false);
            break;
        default:
            oled_write_ln_P(PSTR("?"), false);
    }

    oled_write_ln_P(PSTR("\n"), false);
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_ln_P(led_usb_state.caps_lock ? PSTR("CAPS") : PSTR(""), false);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    if (is_keyboard_master())
        print_status_master();
    else
        print_status_slave();
    return false;
}

#endif // OLED_ENABLE
