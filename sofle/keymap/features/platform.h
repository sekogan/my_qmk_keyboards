// Adds user selectable platform.
// Used by other features.

#pragma once

#include QMK_KEYBOARD_H

typedef enum {
    LINUX_PLATFORM,
    WINDOWS_PLATFORM,
    MAC_PLATFORM,
    UNKNOWN_PLATFORM,
} platform_t;

// Returns currently selected platform.
platform_t get_platform(void);

// Sets a platform as currently selected.
void set_platform(platform_t platform);

// Should be called from process_record_user for each platform_keycode that is supposed to set
// the given platform as currently selected.
bool process_platform(
    uint16_t keycode, keyrecord_t* record, uint16_t platform_keycode, platform_t platform
);

// Implement this function to get notified when currently selected platform is set.
void platform_set_user(void);
