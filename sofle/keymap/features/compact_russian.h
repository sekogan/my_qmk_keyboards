// Makes Russian layout more compact by placing rarely used letters ("yo" and "hard sign")
// in a separate layer activated by Grave Alt. Letters are located under more frequently
// used similar letters: "e" and "soft sign" respectively.

#pragma once

#include QMK_KEYBOARD_H

// Enables or disables the feature.
void enable_compact_russian(bool enable);

// Should be called from process_record_user.
bool process_compact_russian(uint16_t keycode, keyrecord_t* record);
