#include "language_stash.h"
#include "language.h"

static bool _feature_enabled = true;
static bool _stash_empty = true;
static language_t _stashed_language = UNKNOWN_LANGUAGE;

void stash_current_language(void) {
    if (!_feature_enabled)
        return;
    const language_t language = get_language();
    if (language == UNKNOWN_LANGUAGE)
        return;
    if (language != PRIMARY_LANGUAGE) {
        switch_language(PRIMARY_LANGUAGE);
        _stashed_language = language;
        _stash_empty = false;
    }
}

void restore_stashed_language(void) {
    if (!_stash_empty) {
        const language_t language = get_language();
        if (language != _stashed_language && language != UNKNOWN_LANGUAGE)
            switch_language(_stashed_language);
        _stash_empty = true;
    }
}

bool is_language_stash_empty(void) {
    return _stash_empty;
}

void clear_language_stash(void) {
    _stash_empty = true;
}

bool is_language_stash_enabled(void) {
    return _feature_enabled;
}

void enable_language_stash(bool enable) {
    _feature_enabled = enable;
}

bool process_language_stash(
    uint16_t keycode, keyrecord_t *record, uint16_t toggle_feature_keycode
) {
    if (keycode == toggle_feature_keycode) {
        if (record->event.pressed) {
            enable_language_stash(!is_language_stash_enabled());
            clear_language_stash();
        }
        return false;
    }
    return true;
}
