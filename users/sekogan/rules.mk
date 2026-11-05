# Reusable features (originally written for the Sofle keymap). QMK includes users/<keymap name>/
# in every keymap called "sekogan" (including the KBD75 ones), so the features are only built
# when a keymap opts in with `SEK_FEATURES = yes` in its rules.mk.
ifeq ($(strip $(SEK_FEATURES)), yes)
    SRC += features/caps_word.c
    SRC += features/clipboard_shortcuts.c
    SRC += features/compact_russian_layout.c
    SRC += features/fast_keycode.c
    SRC += features/instant_qwerty.c
    SRC += features/language_stash.c
    SRC += features/language.c
    SRC += features/non_qwerty_shortcuts.c
    SRC += features/os.c
    SRC += features/select_word.c
    SRC += features/text_editing.c
endif
