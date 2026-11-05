// Copyright 2024-2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// Adds user selectable OS.
// Used by other features.

#pragma once

#include "quantum.h"

typedef enum {
    LINUX_OS,
    WINDOWS_OS,
    MAC_OS,
    UNKNOWN_OS,
} os_t;

// Returns currently selected/autodetected OS.
os_t get_os(void);

// Enables autodetection mode. This is the default.
void enable_os_autodetect(void);

bool is_os_autodetection_enabled(void);

// Sets an OS as currently selected. Disables autodetection.
void set_os(os_t os);

// Should be called from process_record_user.
bool process_os_selector(uint16_t keycode, keyrecord_t* record, uint16_t selector_keycode);

// Implement this function to get notified when currently selected OS is set.
void os_set_user(void);
