// Temporarily cancels "instant qwerty" feature if shortcut mods are pressed.
// Depends on "instant qwerty" feature.

#pragma once

#include QMK_KEYBOARD_H


// Should be called from post_process_record_user.
void post_process_record_non_qwerty_shortcuts(uint16_t keycode, keyrecord_t *record);
