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

# The version popup (V on EXTRA) shows the keymap version (VERSION at the repo root) and the QMK
# version (the qmk_firmware submodule's release tag): the two parts of the release name
# build/firmware/qmk-<QMK version>-<VERSION>/ (scripts/build_all.sh). Both come from the commit,
# so the same commit still gives the same firmware (SKIP_VERSION=yes keeps QMK's own git stamp
# out). They go in bare (-DSEK_KEYMAP_VERSION=1.5.1, a C preprocessing number; display_skin.c
# stringifies them and asserts that they fit the screen), so only MAJOR.MINOR.PATCH passes.
SEK_KEYMAP_VERSION := $(strip $(file <$(QMK_USERSPACE)/VERSION))
SEK_QMK_VERSION := $(shell git -C "$(QMK_USERSPACE)/qmk_firmware" describe --tags --exact-match 2>/dev/null)
# $(call sek_version_ok,<v>) is non-empty for MAJOR.MINOR.PATCH: one word of digits and dots, three
# dot-separated parts, none of them empty.
sek_not_version_glyphs = $(subst .,,$(subst 0,,$(subst 1,,$(subst 2,,$(subst 3,,$(subst 4,,$(subst 5,,$(subst 6,,$(subst 7,,$(subst 8,,$(subst 9,,$(1))))))))))))
sek_version_ok = $(and $(filter 1,$(words $(1))),$(if $(call sek_not_version_glyphs,$(1)),,ok),$(filter 3,$(words $(subst ., ,$(1)))),$(if $(findstring ..,$(1))$(filter .% %.,$(1)),,ok))
ifeq ($(call sek_version_ok,$(SEK_KEYMAP_VERSION)),)
    $(error VERSION '$(SEK_KEYMAP_VERSION)' is not MAJOR.MINOR.PATCH, which the Elora version popup needs)
endif
ifeq ($(call sek_version_ok,$(SEK_QMK_VERSION)),)
    $(error qmk_firmware is not on a MAJOR.MINOR.PATCH release tag ('$(SEK_QMK_VERSION)'), which the Elora version popup needs)
endif
OPT_DEFS += -DSEK_KEYMAP_VERSION=$(SEK_KEYMAP_VERSION) -DSEK_QMK_VERSION=$(SEK_QMK_VERSION)
