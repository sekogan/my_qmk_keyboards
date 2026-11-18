// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#include "selected_layout.h"
#include "instant_qwerty.h"

static bool _is_under_letter(keyrecord_t *record, uint16_t letter) {
    if (!IS_KEYEVENT(record->event)) return true; // a combo, an encoder
    return keymap_key_to_keycode(get_selected_default_layer(), record->event.key) == letter;
}

bool process_selected_layout_keys(uint16_t keycode, keyrecord_t *record, const struct selected_layout_key *begin, const struct selected_layout_key *end) {
    for (const struct selected_layout_key *k = begin; k != end; ++k) {
        if (k->keycode == keycode) return _is_under_letter(record, k->letter);
    }
    return true;
}
