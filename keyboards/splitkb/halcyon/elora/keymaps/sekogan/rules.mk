AUDIO_ENABLE = no
BOOTMAGIC_ENABLE = no
COMMAND_ENABLE = no
CONSOLE_ENABLE = no
ENCODER_ENABLE = no
EXTRAKEY_ENABLE = yes
KEY_OVERRIDE_ENABLE = no
MOUSEKEY_ENABLE = no
NKRO_ENABLE = no
OS_DETECTION_ENABLE = yes
RGB_MATRIX_ENABLE = no
RGBLIGHT_ENABLE = no
SLEEP_LED_ENABLE = no
SWAP_HANDS_ENABLE = no
TAP_DANCE_ENABLE = no
TRI_LAYER_ENABLE = yes

BACKLIGHT_ENABLE = yes
BACKLIGHT_DRIVER = pwm
QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += st7789_spi surface

SEK_FEATURES = yes

SRC += display.c
SRC += graphics/spleen12x24.qff.c

# Temporary hardware preview of display mockups (display_preview.c).
SRC += display_preview.c
SRC += $(foreach p,01 02 03 04 05 06 07 08 09 10,graphics/preview/preview_t$(p).qgf.c)
