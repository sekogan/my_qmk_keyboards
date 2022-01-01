AUDIO_ENABLE = no
BACKLIGHT_ENABLE = no
COMMAND_ENABLE = no
CONSOLE_ENABLE = no
ENCODER_ENABLE = yes
EXTRAFLAGS += -flto
EXTRAKEY_ENABLE = yes
KEY_OVERRIDE_ENABLE = no
MOUSEKEY_ENABLE = no
OLED_DRIVER = SSD1306
OLED_ENABLE = yes
RGBLIGHT_ENABLE = no
SLEEP_LED_ENABLE = no
SWAP_HANDS_ENABLE = no
TAP_DANCE_ENABLE = no

SRC += features/caps_word.c
SRC += features/clipboard_shortcuts.c
SRC += features/compact_russian_layout.c
SRC += features/fast_keycode.c
SRC += features/language.c
SRC += features/language_stash.c
SRC += features/platform.c
SRC += features/select_word.c
