// Copyright 2024-2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#include "os.h"
#include <os_detection.h>

static os_t _current_os    = UNKNOWN_OS;
static bool _autodetect_os = true;
static bool _detected      = false;

static void _apply_detected_os(os_variant_t detected) {
    os_t os;
    switch (detected) {
        case OS_LINUX:
            os = LINUX_OS;
            break;
        case OS_WINDOWS:
            os = WINDOWS_OS;
            break;
        case OS_MACOS:
            os = MAC_OS;
            break;
        case OS_UNSURE:
            return;
        default:
            os = UNKNOWN_OS;
    }
    _detected   = true;
    _current_os = os;
    os_set_user();
}

bool process_detected_host_os_user(os_variant_t detected) {
    if (!_detected) // the same condition the former polling get_os() used
        _apply_detected_os(detected);
    return true;
}

os_t get_os(void) {
    return _current_os;
}

void enable_os_autodetect(void) {
    _current_os    = UNKNOWN_OS;
    _autodetect_os = true;
    _detected      = false;
    os_set_user();
    _apply_detected_os(detected_host_os());
}

bool is_os_autodetection_enabled(void) {
    return _autodetect_os;
}

void set_os(os_t os) {
    _autodetect_os = false;
    _current_os    = os;
    os_set_user();
}

static void _cycle_through_modes(void) {
    if (_autodetect_os)
        set_os(0);
    else {
        const os_t new_os = _current_os + 1;
        if (new_os == UNKNOWN_OS)
            enable_os_autodetect();
        else
            set_os(new_os);
    }
}

bool process_os_selector(uint16_t keycode, keyrecord_t* record, uint16_t selector_keycode) {
    if (keycode == selector_keycode) {
        if (record->event.pressed) _cycle_through_modes();
        return false;
    }
    return true;
}

__attribute__((weak)) void os_set_user(void) {}
