#include "fast_keycode.h"

bool process_fast_keycode(
    uint16_t keycode, keyrecord_t* record,
    uint16_t fast_keycode, uint16_t sent_keycode, uint8_t event_count
) {
    if (keycode != fast_keycode)
        return true;

    if (record->event.pressed) {
        for (uint8_t i = 1; i < event_count; ++i)
            tap_code(sent_keycode);
        register_code(sent_keycode);
    } else {
        unregister_code(sent_keycode);
    }
    return false;
}
