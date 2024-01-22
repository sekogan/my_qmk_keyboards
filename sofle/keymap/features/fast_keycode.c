#include "fast_keycode.h"

bool process_fast_keycodes(
    uint16_t keycode, keyrecord_t* record,
    const struct fast_keycode* begin, const struct fast_keycode* end
) {
    for (const struct fast_keycode* it = begin; it != end; ++it) {
        if (keycode != it->fast_keycode)
            continue;

        keycode = it->real_keycode;
        if (record->event.pressed) {
            for (uint8_t i = 1; i < it->real_count; ++i)
                tap_code(keycode);
            register_code(keycode);
        } else {
            unregister_code(keycode);
        }
        return false;
    }
    return true;
}
