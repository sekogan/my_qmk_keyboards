// Copyright 2022-2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// Replaces exotic clipboard-related keycodes with conventional key combinations.

#pragma once

#include "quantum.h"

// Should be called from process_record_user.
bool process_clipboard_shortcuts(uint16_t keycode, keyrecord_t* record);
