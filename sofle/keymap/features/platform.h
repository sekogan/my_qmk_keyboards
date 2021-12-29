#pragma once

#include QMK_KEYBOARD_H

typedef enum {
    LINUX_PLATFORM,
    WINDOWS_PLATFORM,
    MAC_PLATFORM,
    UNKNOWN_PLATFORM,
} platform_t;

platform_t get_platform(void);
void set_platform(platform_t platform);
bool process_platform(
    uint16_t keycode, keyrecord_t* record, uint16_t platform_keycode, platform_t platform
);
