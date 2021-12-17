#include QMK_KEYBOARD_H

enum layers {
    _QWERTY,
    _COLEMAK,
    _LOWER,
    _RAISE,
    _ADJUST,
};

enum custom_keycodes {
    KC_QWRT = SAFE_RANGE,   // Qwerty
    KC_CLMK,                // Colemak

    KC_LIN,
    KC_WIN,
    KC_MAC,

    KC_LOWER,
    KC_RAISE,

    KC_TALNG,               // Toggle auto language adjustment

    KC_WPREV,               // Previous word
    KC_WNEXT,               // Next word
    KC_LSTRT,               // Line start
    KC_LEND,                // Line end
    KC_REDO,

    KC_FUP,                 // Fast up
    KC_FDOWN,               // Fast down
};


#ifdef TAP_DANCE_ENABLE

enum tap_dances {
    TAP_DANCE_LSFT,
    TAP_DANCE_RSFT,
};

#define TD_LSFT     TD(TAP_DANCE_LSFT)
#define TD_RSFT     TD(TAP_DANCE_RSFT)

#else // TAP_DANCE_ENABLE

#define TD_LSFT     KC_LSFT
#define TD_RSFT     KC_RSFT

#endif // TAP_DANCE_ENABLE


#define KC_LDEL     C(S(KC_K))
#define KC_WBSPC    C(KC_BSPC)
#define KC_WDEL     C(KC_DEL)
#define OSM_LSFT    OSM(MOD_LSFT)


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
 *               | LGUI | LALT |LCTRL |/LOWER / SPC  /    \ ENT  \RAISE \|RCTRL | RALT | RGUI |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[_QWERTY] = LAYOUT( \
KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,                         KC_7,     KC_8,     KC_9,     KC_0,     KC_LBRC,  XXXXXXX, \
KC_ESC,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,                         KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSPC, \
KC_TAB,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,                         KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT, \
TD_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_MPLY,  XXXXXXX,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  TD_RSFT, \
                    KC_LGUI,  KC_LALT,  KC_LCTRL, KC_LOWER, KC_SPC,   KC_ENT,   KC_RAISE, KC_RCTRL, KC_RALT,  KC_RGUI \
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
 *               | LGUI | LALT |LCTRL |/LOWER / SPC  /    \ ENT  \RAISE \|RCTRL | RALT | RGUI |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[_COLEMAK] = LAYOUT( \
KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,                         KC_7,     KC_8,     KC_9,     KC_0,     KC_LBRC,  XXXXXXX, \
KC_ESC,   KC_Q,     KC_W,     KC_F,     KC_P,     KC_G,                         KC_J,     KC_L,     KC_U,     KC_Y,     KC_SCLN,  KC_BSPC, \
KC_TAB,   KC_A,     KC_R,     KC_S,     KC_T,     KC_D,                         KC_H,     KC_N,     KC_E,     KC_I,     KC_O,     KC_QUOT, \
TD_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_MPLY,  XXXXXXX,  KC_K,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  TD_RSFT, \
                    KC_LGUI,  KC_LALT,  KC_LCTRL, KC_LOWER, KC_SPC,   KC_ENT,   KC_RAISE, KC_RCTRL, KC_RALT,  KC_RGUI \
),

/* ,-----------------------------------------.                    ,-----------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  | F10  | F11  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  !   |  @   |  #   |  $   |  %   |  ^   |                    |  &   |  *   |  (   |  )   | \ |  | BSPC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ` ~  |  ~   |  +   | = +  | - _  |  _   |,------.    ,------.| ' "  |  "   |  [   |  ]   |  :   |  |   |
 * |------+------+------+------+------+------|| ___  |    | ___  ||------+------+------+------+------+------|
 * | ___  |OSSft |      |      | ; :  | / ?  |`------'    `------'|  {   |  }   |  <   |  >   |  ?   | ___  |
 * `-------------+------+------+------+-.------------.    ,------------.-+------+------+------+-------------'
 *               | ___  | ___  | ___  |/ ___  / ___  /    \ ___  \ ___  \| ___  | ___  | ___  |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[_LOWER] = LAYOUT( \
KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,                        KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12, \
KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_CIRC,                      KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,  KC_BSLS,  KC_BSPC, \
KC_GRV,   KC_TILD,  KC_PLUS,  KC_EQL,   KC_MINS,  KC_UNDS,                      KC_QUOT,  KC_DQUO,  KC_LBRC,  KC_RBRC,  KC_COLN,  KC_PIPE, \
_______,  OSM_LSFT, XXXXXXX,  XXXXXXX,  KC_SCLN,  KC_SLSH,  _______,  _______,  KC_LCBR,  KC_RCBR,  KC_LT,    KC_GT,    KC_QUES,  _______, \
                    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, _______\
),

/* ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      | FUP  | PSCR | INS  | PAUS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  | MEH  | HYPR |      | CAPS | ` ~  |                    | PGUP |WPREV |  UP  |WNEXT | DEL  | BSPC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  | LSFT | LALT | LCTL |      | TAB  |,------.    ,------.| PGDN | LEFT | DOWN | RGHT | WDEL |WBSPC |
 * |------+------+------+------+------+------|| ___  |    | ___  ||------+------+------+------+------+------|
 * | ___  | UNDO | CUT  | COPY |PASTE | REDO |`------'    `------'|      |LSTRT |FDOWN | LEND | LDEL | ___  |
 * `-------------+------+------+------+-.------------.    ,------------.-+------+------+------+-------------'
 *               | ___  | ___  | ___  |/ ___  / ___  /    \ ___  \ ___  \| ___  | ___  | ___  |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[_RAISE] = LAYOUT( \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                      XXXXXXX,  XXXXXXX,  KC_FUP,   KC_PSCR,  KC_INS,    KC_PAUS, \
KC_ESC,   KC_MEH,   KC_HYPR,  XXXXXXX,  KC_CAPS,  KC_GRV,                       KC_PGUP,  KC_WPREV, KC_UP,    KC_WNEXT, KC_DEL,    KC_BSPC, \
KC_TAB,   KC_LSFT,  KC_LALT,  KC_LCTL,  XXXXXXX,  KC_TAB,                       KC_PGDN,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_WDEL,   KC_WBSPC, \
_______,  KC_UNDO,  KC_CUT,   KC_COPY,  KC_PASTE, KC_REDO,  _______,  _______,  XXXXXXX,  KC_LSTRT, KC_FDOWN, KC_LEND,  KC_LDEL,   _______, \
                    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______ \
),

/* ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      | QWRT |                    | LIN  |      |      |      |      |RESET |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      | CLMK |,------.    ,------.| WIN  |      |      |TALNG |      |      |
 * |------+------+------+------+------+------||      |    |      ||------+------+------+------+------+------|
 * | ___  |      |      |      |      |      |`------'    `------'| MAC  |      |      |      |      | ___  |
 * `-------------+------+------+------+-.------------.    ,------------.-+------+------+------+-------------'
 *               | ___  | ___  | ___  |/ ___  / ___  /    \ ___  \ ___  \| ___  | ___  | ___  |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[_ADJUST] = LAYOUT( \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                      XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_QWRT,                      KC_LIN,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  RESET, \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_CLMK,                      KC_WIN,   XXXXXXX,  XXXXXXX,  KC_TALNG, XXXXXXX,  XXXXXXX, \
_______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MAC,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______, \
                    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______ \
)

};

void language_reset(void);
void toggle_auto_language_adjustment(void);
void adjust_language(void);
void restore_language_if_adjusted(void);


enum platforms {
    LINUX_PLATFORM,
    WINDOWS_PLATFORM,
    MAC_PLATFORM,
    UNKNOWN_PLATFORM,
};

enum platforms current_platform = UNKNOWN_PLATFORM;

void platform_set(enum platforms platform) {
    current_platform = platform;
    language_reset();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
            return false;
        case KC_RAISE:
            if (record->event.pressed)
                layer_on(_RAISE);
            else
                layer_off(_RAISE);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
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
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                }
            }
            break;
        case KC_WNEXT:
             if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
            }
            break;
        case KC_LSTRT:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                     // CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                } else {
                    register_code(KC_HOME);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
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
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                } else {
                    register_code(KC_END);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_code(KC_END);
                }
            }
            break;
        case KC_COPY:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_C);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_C);
            }
            return false;
        case KC_PASTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_V);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_V);
            }
            return false;
        case KC_CUT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_X);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_X);
            }
            return false;
            break;
        case KC_UNDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Z);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Z);
            }
            return false;
        case KC_REDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Y);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Y);
            }
            return false;
        case KC_LIN:
            if (record->event.pressed)
                platform_set(LINUX_PLATFORM);
            return false;
        case KC_WIN:
            if (record->event.pressed)
                platform_set(WINDOWS_PLATFORM);
            return false;
        case KC_MAC:
            if (record->event.pressed)
                platform_set(MAC_PLATFORM);
            return false;
        case KC_TALNG:
            if (record->event.pressed)
                toggle_auto_language_adjustment();
            return false;
    }
    return true;
}


layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _LOWER:
        adjust_language();
        break;
    default:
        restore_language_if_adjusted();
    }
    return state;
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


enum languages {
    PRIMARY_LANGUAGE,
    SECONDARY_LANGUAGE,
    UNKNOWN_LANGUAGE = 100,
};

enum languages current_language = UNKNOWN_LANGUAGE;

uint8_t swap_mods(uint8_t desired_mods) {
    uint8_t before_mods = get_mods();
    uint8_t missing_mods = (~before_mods) & desired_mods;
    uint8_t undesired_mods = before_mods & (~desired_mods);
    if (undesired_mods)
        unregister_mods(undesired_mods);
    if (missing_mods)
        register_mods(missing_mods);
    return before_mods;
}

void language_reset(void) {
    current_language = UNKNOWN_LANGUAGE;
}

void language_set(enum languages language) {
    uint8_t mods = get_mods();
    switch (current_platform) {
        case LINUX_PLATFORM:
            switch (language) {
                case PRIMARY_LANGUAGE:
                    swap_mods(0);
                    break;
                case SECONDARY_LANGUAGE:
                    swap_mods(MOD_LSFT);
                    break;
                default:
                    return;
            }
            tap_code(KC_CAPS);
            swap_mods(mods);
            break;
        case WINDOWS_PLATFORM:
            swap_mods(MOD_LALT|MOD_LSFT);
            tap_code(KC_1 + language);
            swap_mods(mods);
            break;
        default:
            return;
    }
    current_language = language;
}


bool auto_language_adjustment_enabled = true;
bool current_language_adjusted = false;
enum languages language_before_adjustment = UNKNOWN_LANGUAGE;

void toggle_auto_language_adjustment(void) {
    auto_language_adjustment_enabled = !auto_language_adjustment_enabled;
}

void adjust_language(void) {
    if (!auto_language_adjustment_enabled || current_language == UNKNOWN_LANGUAGE)
        return;
    if (current_language != PRIMARY_LANGUAGE) {
        language_before_adjustment = current_language;
        current_language_adjusted = true;
        language_set(PRIMARY_LANGUAGE);
    }
}

void restore_language_if_adjusted(void) {
    if (current_language_adjusted) {
        if (current_language != language_before_adjustment && current_language != UNKNOWN_LANGUAGE)
            language_set(language_before_adjustment);
        current_language_adjusted = false;
    }
}


#ifdef TAP_DANCE_ENABLE

void dance_language_mod_finished(
  qk_tap_dance_state_t *state, enum languages language, uint8_t mod
) {
    if (state->pressed)
        register_mods(mod);
    else if (state->count == 1)
        language_set(language);
}

void dance_language_mod_reset(qk_tap_dance_state_t *state, uint8_t mod) {
    if (get_mods() & mod)
        unregister_mods(mod);
}

void dance_lsft_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_language_mod_finished(state, PRIMARY_LANGUAGE, MOD_LSFT);
}

void dance_lsft_reset(qk_tap_dance_state_t *state, void *user_data) {
    dance_language_mod_reset(state, MOD_LSFT);
}

void dance_rsft_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_language_mod_finished(state, SECONDARY_LANGUAGE, MOD_BIT(KC_RSFT));
}

void dance_rsft_reset(qk_tap_dance_state_t *state, void *user_data) {
    dance_language_mod_reset(state, MOD_BIT(KC_RSFT));
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TAP_DANCE_LSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_lsft_finished, dance_lsft_reset),
    [TAP_DANCE_RSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_rsft_finished, dance_rsft_reset),
};

#endif // TAP_DANCE_ENABLE


#ifdef KEY_OVERRIDE_ENABLE

#define MOD_MASK_RALT MOD_BIT(KC_RALT)
const key_override_t russian_e_override = ko_make_basic(MOD_MASK_RALT, KC_T, KC_GRV);
const key_override_t russian_hard_sign_override = ko_make_basic(MOD_MASK_RALT, KC_M, KC_RBRC);

const key_override_t **key_overrides = (const key_override_t *[]){
    &russian_e_override,
    &russian_hard_sign_override,
    NULL
};

#endif // KEY_OVERRIDE_ENABLE


#ifdef OLED_ENABLE

static void print_status_master(void) {
    oled_write_ln_P(PSTR("OS\n"), false);
    switch (current_platform) {
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
    switch (current_language) {
        case PRIMARY_LANGUAGE:
            oled_write_ln_P(PSTR("Pri"), false);
            break;
        case SECONDARY_LANGUAGE:
            oled_write_ln_P(auto_language_adjustment_enabled ? PSTR("Alt*") : PSTR("Alt"), false);
            break;
        default:
            oled_write_ln_P(PSTR("?"), false);
    }

    oled_write_ln_P(PSTR("\n"), false);
    uint8_t mods = get_mods() | get_oneshot_mods();
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
            oled_write_P(PSTR("RAISE"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("lower"), false);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("*"), false);
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

void oled_task_user(void) {
    if (is_keyboard_master())
        print_status_master();
    else
        print_status_slave();
}

#endif // OLED_ENABLE
