#include "platform.h"

static platform_t _current_platform = UNKNOWN_PLATFORM;

platform_t get_platform(void) {
    return _current_platform;
}

void set_platform(platform_t platform) {
    _current_platform = platform;
    platform_set_user();
}

bool process_platform_selector(
    uint16_t keycode, keyrecord_t* record, uint16_t selector_keycode, platform_t platform
) {
    if (keycode == selector_keycode) {
        if (record->event.pressed)
            set_platform(platform);
        return false;
    }
    return true;
}

__attribute__((weak)) void platform_set_user(void) {}
