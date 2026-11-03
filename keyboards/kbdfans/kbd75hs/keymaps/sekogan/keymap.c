/* Copyright 2021 DZTECH <moyi4681@Live.cn>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum layer_id {
  BASE_LAYER,
  SECOND_LAYER,
  THIRD_LAYER
};

enum tap_dances {
  TAP_DANCE_LSFT,
  TAP_DANCE_RSFT,
};

#define CAPS_FN     LT(SECOND_LAYER, KC_CAPSLOCK)
#define ESC_FN2     LT(THIRD_LAYER, KC_ESCAPE)

#define LSFT_A      LSFT_T(KC_A)
#define LALT_S      LALT_T(KC_S)
#define LCTL_D      LCTL_T(KC_D)

#define RCTL_K      RCTL_T(KC_K)
#define RALT_L      RALT_T(KC_L)
#define RSFT_SCLN   RSFT_T(KC_SCLN)

#define TD_LSFT     TD(TAP_DANCE_LSFT)
#define TD_RSFT     TD(TAP_DANCE_RSFT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [BASE_LAYER] = LAYOUT_75_ansi(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_INS,   KC_DEL,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,             KC_BSPC,  KC_HOME,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_END,
    CAPS_FN,  LSFT_A,   LALT_S,   LCTL_D,   KC_F,     KC_G,     KC_H,     KC_J,     RCTL_K,   RALT_L,   RSFT_SCLN,KC_QUOT,                      KC_ENT,   KC_PGUP,
    TD_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            TD_RSFT,            KC_UP,    KC_PGDN,
    KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RGUI,  KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),

  [SECOND_LAYER] = LAYOUT_75_ansi(
    ESC_FN2,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_PAUS,  XXXXXXX,  XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,
    XXXXXXX,  KC_VOLU,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MPRV,  KC_MNXT,  KC_MPLY,            XXXXXXX,
    XXXXXXX,  KC_VOLD,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                      XXXXXXX,  XXXXXXX,
    _______,  _______,  KC_MUTE,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            _______,            XXXXXXX,  XXXXXXX,
    _______,  _______,  _______,                                XXXXXXX,                                _______,  _______,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX
  ),

  [THIRD_LAYER] = LAYOUT_75_ansi(
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  RESET,    XXXXXXX,  XXXXXXX,  KC_SLEP,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,  XXXXXXX,
    XXXXXXX,  RGB_TOG,  RGB_MOD,  RGB_HUD,  RGB_HUI,  RGB_SAD,  RGB_SAI,  RGB_VAD,  RGB_VAI,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,
    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                      XXXXXXX,  XXXXXXX,
    _______,  _______,  BL_TOGG,  BL_BRTG,  BL_DEC,   BL_INC,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            _______,            XXXXXXX,  XXXXXXX,
    _______,  _______,  _______,                                XXXXXXX,                                _______,  _______,  _______,  XXXXXXX,  XXXXXXX,  XXXXXXX
  ),
};

// The "led_set_user" is called in initialization and any time the "lock" state is changed for any of the states.
// It checks the NUM LOCK state, and if it's disabled, sends the "numlock" key press to enable it.
void led_set_user(uint8_t usb_led) {
  if (!IS_LED_ON(usb_led, USB_LED_NUM_LOCK)) {
    tap_code(KC_NUMLOCK);
  }
}

bool dance_lsft_unregister_pending = false;
bool dance_rsft_unregister_pending = false;

void dance_lsft_finished(qk_tap_dance_state_t *state, void *user_data) {
  if (state->pressed) {
    register_mods(MOD_LSFT);
    dance_lsft_unregister_pending = true;
  } else if (state->count == 1) {
    register_mods(MOD_LALT|MOD_LSFT);
    tap_code(KC_0);
    unregister_mods(MOD_LALT|MOD_LSFT);
  }
}

void dance_lsft_reset(qk_tap_dance_state_t *state, void *user_data) {
  if (dance_lsft_unregister_pending) {
    dance_lsft_unregister_pending = false;
    unregister_mods(MOD_LSFT);
  }
}

void dance_rsft_finished(qk_tap_dance_state_t *state, void *user_data) {
  if (state->pressed) {
    register_mods(MOD_BIT(KC_RSFT));
    dance_rsft_unregister_pending = true;
  } else if (state->count == 1) {
    register_mods(MOD_LALT|MOD_LSFT);
    tap_code(KC_7);
    unregister_mods(MOD_LALT|MOD_LSFT);
  }
}

void dance_rsft_reset(qk_tap_dance_state_t *state, void *user_data) {
  if (dance_rsft_unregister_pending) {
    dance_rsft_unregister_pending = false;
    unregister_mods(MOD_BIT(KC_RSFT));
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [TAP_DANCE_LSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_lsft_finished, dance_lsft_reset),
  [TAP_DANCE_RSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_rsft_finished, dance_rsft_reset),
};


#define MOD_MASK_RALT MOD_BIT(KC_RALT)
const key_override_t russian_e_override = ko_make_basic(MOD_MASK_RALT, KC_T, KC_GRV);
const key_override_t russian_hard_sign_override = ko_make_basic(MOD_MASK_RALT, KC_M, KC_RBRC);

const key_override_t **key_overrides = (const key_override_t *[]){
    &russian_e_override,
    &russian_hard_sign_override,
    NULL
};
