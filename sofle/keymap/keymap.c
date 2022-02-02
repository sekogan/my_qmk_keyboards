#include QMK_KEYBOARD_H
#include "instant_qwerty_reasons.h"
#include "layers.h"
#include "oled.h"
#include "features/caps_word.h"
#include "features/clipboard_shortcuts.h"
#include "features/compact_russian_layout.h"
#include "features/fast_keycode.h"
#include "features/instant_qwerty.h"
#include "features/language_stash.h"
#include "features/language.h"
#include "features/non_qwerty_shortcuts.h"
#include "features/platform.h"
#include "features/select_word.h"
#include "features/text_editing.h"


enum custom_keycodes {
    KC_QWRT = SAFE_RANGE,   // Qwerty
    KC_CLMK,                // Colemak DHm

    KC_LIN,
    KC_WIN,
    KC_MAC,

    KC_LOWER,
    KC_RAISE,

    KC_WSEL,                // Select word

    KC_WPREV,               // Previous word
    KC_WNEXT,               // Next word
    KC_LSTRT,               // Line start
    KC_LEND,                // Line end

    KC_WBSPC,               // Delete from the cursor to the beginning of the word
    KC_WDEL,                // Delete from the cursor to the end of the word

    KC_FUP,                 // Fast up
    KC_FDOWN,               // Fast down
};


#define MT_LGUI     LGUI_T(KC_LNG1)
#define MT_RGUI     RGUI_T(KC_LNG2)

#define KC_LDEL     C(S(KC_K))

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
[QWERTY] = LAYOUT( \
KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,                         KC_7,     KC_8,     KC_9,     KC_0,     KC_LBRC,  XXXXXXX, \
KC_ESC,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,                         KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSPC, \
KC_TAB,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,                         KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT, \
OSM_LSFT, KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_MPLY,  XXXXXXX,  KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  OSM_RSFT, \
                    MT_LGUI,  KC_LALT,  KC_LCTRL, KC_LOWER, KC_SPC,   KC_ENT,   KC_RAISE, KC_RCTRL, KC_RALT,  MT_RGUI \
),

/* ,-----------------------------------------.                    ,-----------------------------------------.
 * |  1   |  2   |  3   |  4   |  5   |  6   |                    |  7   |  8   |  9   |  0   |  [   |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |  Q   |  W   |  F   |  P   |  B   |                    |  J   |  L   |  U   |  Y   | ; :  | BSPC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |  A   |  R   |  S   |  T   |  G   |,------.    ,------.|  M   |  N   |  E   |  I   |  O   | ' "  |
 * |------+------+------+------+------+------|| MPLY |    |      ||------+------+------+------+------+------|
 * |Shift |  Z   |  X   |  C   |  D   |  V   |`------'    `------'|  K   |  H   | , <  | . >  | / ?  |Shift |
 * `-------------+------+------+------+-.------------.    ,------------.-+------+------+------+-------------'
 *               |Super | LALT |LCTRL |/LOWER / SPC  /    \ ENT  \RAISE \|RCTRL | RALT |Super |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[COLEMAK] = LAYOUT( \
KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,                         KC_7,     KC_8,     KC_9,     KC_0,     KC_LBRC,  XXXXXXX, \
KC_ESC,   KC_Q,     KC_W,     KC_F,     KC_P,     KC_B,                         KC_J,     KC_L,     KC_U,     KC_Y,     KC_SCLN,  KC_BSPC, \
KC_TAB,   KC_A,     KC_R,     KC_S,     KC_T,     KC_G,                         KC_M,     KC_N,     KC_E,     KC_I,     KC_O,     KC_QUOT, \
OSM_LSFT, KC_Z,     KC_X,     KC_C,     KC_D,     KC_V,     KC_MPLY,  XXXXXXX,  KC_K,     KC_H,     KC_COMM,  KC_DOT,   KC_SLSH,  OSM_RSFT, \
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
 *               | LGUI | ___  | ___  |/ ___  / ___  /    \ ___  \ ___  \| ___  | ___  | RGUI |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[LOWER] = LAYOUT( \
KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,                        KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12, \
KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_CIRC,                      KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,  KC_BSLS,  KC_BSPC, \
KC_GRV,   KC_TILD,  KC_PLUS,  KC_EQL,   KC_MINS,  KC_UNDS,                      KC_QUOT,  KC_DQUO,  KC_LBRC,  KC_RBRC,  KC_COLN,  KC_PIPE, \
_______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_SCLN,  KC_SLSH,  _______,  _______,  KC_LCBR,  KC_RCBR,  KC_LT,    KC_GT,    KC_QUES,  _______, \
                    KC_LGUI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_RGUI \
),

/* ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      | CAPS |                    |      |      | FUP  | PSCR | INS  | PAUS |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  | MEH  | HYPR |      |      | ` ~  |                    | PGUP |WPREV |  UP  |WNEXT | DEL  | BSPC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  | LSFT | LALT | LCTL | TAB  |      |,------.    ,------.| PGDN | LEFT | DOWN | RGHT | WDEL |WBSPC |
 * |------+------+------+------+------+------|| ___  |    | ___  ||------+------+------+------+------+------|
 * | ___  | UNDO | CUT  | COPY |PASTE |PASTE |`------'    `------'| WSEL |LSTRT |FDOWN | LEND | LDEL | ___  |
 * `-------------+------+------+------+-.------------.    ,------------.-+------+------+------+-------------'
 *               | LGUI | ___  | ___  |/ ___  / ___  /    \ ___  \ ___  \| ___  | ___  | RGUI |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[RAISE] = LAYOUT( \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_CAPS,                      XXXXXXX,  XXXXXXX,  KC_FUP,   KC_PSCR,  KC_INS,    KC_PAUS, \
KC_ESC,   KC_MEH,   KC_HYPR,  XXXXXXX,  XXXXXXX,  KC_GRV,                       KC_PGUP,  KC_WPREV, KC_UP,    KC_WNEXT, KC_DEL,    KC_BSPC, \
KC_TAB,   KC_LSFT,  KC_LALT,  KC_LCTL,  KC_TAB,   XXXXXXX,                      KC_PGDN,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_WDEL,   KC_WBSPC, \
_______,  KC_UNDO,  KC_CUT,   KC_COPY,  KC_PASTE, KC_PASTE, _______,  _______,  KC_WSEL,  KC_LSTRT, KC_FDOWN, KC_LEND,  KC_LDEL,   _______, \
                    KC_LGUI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_RGUI \
),

/* ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      | QWRT |                    | LIN  |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      | CLMK |,------.    ,------.| WIN  |      |      |      |      |      |
 * |------+------+------+------+------+------||      |    |      ||------+------+------+------+------+------|
 * | ___  |      |      |      |      |      |`------'    `------'| MAC  |      |      |      |      | ___  |
 * `-------------+------+------+------+-.------------.    ,------------.-+------+------+------+-------------'
 *               | ___  | ___  | ___  |/ ___  / ___  /    \ ___  \ ___  \| ___  | ___  | ___  |
 *               |      |      |      /      /      /      \      \      \      |      |      |
 *               `---------------------------------'        `---------------------------------'
 *                                                                                   generated by [keymapviz] */
[EXTRA] = LAYOUT( \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                      XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_QWRT,                      KC_LIN,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_CLMK,                      KC_WIN,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
_______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MAC,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______, \
                    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______ \
)

};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_text_navigation_shortcuts(keycode, record, KC_WPREV, KC_WNEXT, KC_LSTRT, KC_LEND))
        return false;
    if (!process_fast_keycode(keycode, record, KC_FUP, KC_UP, 5)) return false;
    if (!process_fast_keycode(keycode, record, KC_FDOWN, KC_DOWN, 5)) return false;
    if (!process_clipboard_shortcuts(keycode, record)) return false;
    if (!process_editing_history_shortcuts(keycode, record)) return false;
    if (!process_text_deleting_macros(keycode, record, KC_WDEL, KC_WBSPC)) return false;
    if (keycode != KC_LOWER && keycode != KC_RAISE)
        if (!process_caps_word(keycode, record)) return false;
    if (!process_select_word(keycode, record, KC_WSEL)) return false;
    if (!process_compact_russian_layout(keycode, record)) return false;
    if (!process_language(keycode, record)) return false;
    if (!process_platform_selector(keycode, record, KC_LIN, LINUX_PLATFORM)) return false;
    if (!process_platform_selector(keycode, record, KC_WIN, WINDOWS_PLATFORM)) return false;
    if (!process_platform_selector(keycode, record, KC_MAC, MAC_PLATFORM)) return false;

    switch (keycode) {
        case KC_QWRT:
            if (record->event.pressed)
                set_single_persistent_default_layer(QWERTY);
            return false;
        case KC_CLMK:
            if (record->event.pressed)
                set_single_persistent_default_layer(COLEMAK);
            return false;
        case KC_LOWER:
            if (record->event.pressed)
                layer_on(LOWER);
            else
                layer_off(LOWER);
            update_tri_layer(LOWER, RAISE, EXTRA);
            return false;
        case KC_RAISE:
            if (record->event.pressed)
                layer_on(RAISE);
            else
                layer_off(RAISE);
            update_tri_layer(LOWER, RAISE, EXTRA);
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


void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    post_process_record_non_qwerty_shortcuts(keycode, record);
}


layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case LOWER:
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
    const bool is_secondary = get_language() == SECONDARY_LANGUAGE;
    enable_compact_russian_layout(is_secondary);

    if (get_language() == PRIMARY_LANGUAGE)
        del_instant_qwerty_activation_reasons(NON_ENGLISH_LANGUAGE);
    else
        add_instant_qwerty_activation_reasons(NON_ENGLISH_LANGUAGE);
}


void suspend_power_down_user(void) {
    suspend_power_down_oled();
}


// The "led_set_user" is called in initialization and any time the "lock" state is changed for any of the states.
// It checks the NUM LOCK state, and if it's disabled, sends the "numlock" key press to enable it.
void led_set_user(uint8_t usb_led) {
    if (!IS_LED_ON(usb_led, USB_LED_NUM_LOCK))
        tap_code(KC_NUMLOCK);
}
