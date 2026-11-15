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
SRC += display_state.c
SRC += display_skin.c
SRC += display_gfx_qp.c
SRC += graphics/font_val28_caps.qff.c
SRC += graphics/font_l24_caps.qff.c
SRC += graphics/font_l18_caps.qff.c
SRC += graphics/icon_shift.qgf.c
SRC += graphics/icon_ctrl.qgf.c
SRC += graphics/icon_alt.qgf.c
SRC += graphics/icon_super.qgf.c
SRC += graphics/icon_win.qgf.c
SRC += graphics/icon_cmd.qgf.c
