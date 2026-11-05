// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

// Handles the four encoder keycodes from the encoder map (left/right encoder,
// counter-clockwise/clockwise). Acts on press only and returns false for them.
// Call it first in process_record_user, so no other feature sees encoder turns.
bool process_encoder_keycodes(uint16_t keycode, keyrecord_t* record, uint16_t l_ccw, uint16_t l_cw, uint16_t r_ccw, uint16_t r_cw);
