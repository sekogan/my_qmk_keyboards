#include "language_stash.h"
#include "language.h"

static bool _feature_enabled = true;
static bool _stash_empty = true;
static language_t _stashed_language = UNKNOWN_LANGUAGE;

void stash_current_language(void) {
    if (!_feature_enabled)
        return;
    language_t language = get_language();
    if (language == UNKNOWN_LANGUAGE)
        return;
    if (language != PRIMARY_LANGUAGE) {
        _stashed_language = language;
        _stash_empty = false;
        switch_language(PRIMARY_LANGUAGE);
    }
}

void restore_stashed_language(void) {
    if (!_stash_empty) {
        language_t language = get_language();
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
