// NOTE: this version was modified. For the original version go to the original author's site.
//
// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//
// For full documentation, see
// https://getreuer.info/posts/keyboards/select-word

#include "select_word.h"

enum { STATE_NONE, STATE_WORD, STATE_LINE, STATE_SELECTED };

bool process_select_word(
    uint16_t keycode, keyrecord_t* record,
    uint16_t sel_keycode
) {
    static uint8_t state = STATE_NONE;

    if (keycode == KC_LSFT || keycode == KC_RSFT) { return true; }

    if (keycode == sel_keycode && record->event.pressed) {  // On key press.
        const uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
        const uint8_t all_mods = mods | get_oneshot_mods();
#else
        const uint8_t all_mods = mods;
#endif  // NO_ACTION_ONESHOT
        if ((all_mods & MOD_MASK_SHIFT) == 0) {  // Select word.
            if (state == STATE_NONE)
                tap_code(KC_RIGHT);

            register_code(KC_LCTL);

            if (state == STATE_NONE)
                tap_code(KC_LEFT);

            register_code(KC_LSFT);
            register_code(KC_RIGHT);
            state = STATE_WORD;
        } else {  // Select line.
            if (state == STATE_NONE) {
                clear_mods();
#ifndef NO_ACTION_ONESHOT
                clear_oneshot_mods();
#endif  // NO_ACTION_ONESHOT
                send_keyboard_report();
                SEND_STRING(SS_TAP(X_END) SS_TAP(X_HOME) SS_TAP(X_HOME));
                register_mods(mods);
            }

            register_code(KC_DOWN);
            state = STATE_LINE;
        }
        return false;
    }

    // `sel_keycode` was released, or another key was pressed.
    switch (state) {
        case STATE_WORD:
            unregister_code(KC_RIGHT);
            unregister_code(KC_LSFT);
            unregister_code(KC_LCTL);
            state = STATE_SELECTED;
            break;

        case STATE_LINE:
            unregister_code(KC_DOWN);
            state = STATE_SELECTED;
            break;

        case STATE_SELECTED:
            if (keycode == KC_ESC) {
                tap_code(KC_RIGHT);
                state = STATE_NONE;
                return false;
            }
            // Fallthrough.
        default:
            state = STATE_NONE;
    }

    return true;
}
