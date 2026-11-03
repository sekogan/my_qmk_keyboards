#include "language_stash.h"
#include "language.h"

static bool _stash_empty = true;
static language_t _stashed_language = UNKNOWN_LANGUAGE;

void stash_current_language(void) {
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
