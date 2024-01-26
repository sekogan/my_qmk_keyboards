// OS independent (for user) non-modal input language switcher.
// Depends on "OS" feature.

#pragma once

#include QMK_KEYBOARD_H

typedef enum {
    PRIMARY_LANGUAGE,
    SECONDARY_LANGUAGE,
    UNKNOWN_LANGUAGE = 100,
} language_t;

// Returns currently selected language.
language_t get_language(void);

// Sets currently selected language to its default value, which is UNKNOWN_LANGUAGE.
void reset_language(void);

// Switches to the given language by sending a keystroke to the host
// and sets it as currently selected.
void switch_language(language_t language);

// Should be called from process_record_user.
bool process_language(uint16_t keycode, keyrecord_t* record);

// Implement this function to get notified when language is set.
void language_set_user(void);
