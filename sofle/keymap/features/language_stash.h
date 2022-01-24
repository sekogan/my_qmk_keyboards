// Temporarily activates the primary language saving currently selected language to
// the stash.
// Depends on "language" feature.

#pragma once

#include QMK_KEYBOARD_H

// Makes sure the primary language is selected.
// If it is not the case, puts current language to the stash (overwriting existing value
// if the stash is not empty) and switches to the primary language.
void stash_current_language(void);

// Switches to the stashed language and clears the stash if it is not empty. Does nothing otherwise.
void restore_stashed_language(void);

// Returns true if the stash is not empty.
bool is_language_stash_empty(void);

// Clears the stash if it is not empty. Does nothing otherwise.
void clear_language_stash(void);
