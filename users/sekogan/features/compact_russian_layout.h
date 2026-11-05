// Copyright 2022-2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// Makes Russian layout more compact by placing rarely used letters ("yo" and "hard sign")
// in a separate layer activated by Grave Alt. Letters are located under more frequently
// used similar letters: "e" and "soft sign" respectively.

#pragma once

#include "quantum.h"

// Should be called from process_record_user.
bool process_compact_russian_layout(uint16_t keycode, keyrecord_t* record);

// Enables or disables the feature.
void enable_compact_russian_layout(bool enable);
