// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <string.h>
#include "qp.h"
#include "qp_surface.h"
#include "display.h"
#include "display_preview.h"
#include "layers.h"
#include "features/instant_qwerty.h"
#include "features/language_stash.h"
#include "features/language.h"
#include "features/os.h"
#include "graphics/spleen12x24.qff.h"

#define MARGIN_X 4
#define ROW_HEIGHT 24
#define COLOR_LABEL 0, 0, 128
#define COLOR_VALUE 0, 0, 255
#define COLOR_BG 0, 0, 0

static painter_device_t      lcd;
static painter_device_t      surface;
static painter_font_handle_t font;
// Aligned: the rgb565 surface does 16-bit stores into it, and the M0+ faults on misaligned access.
static uint8_t               framebuffer[SURFACE_REQUIRED_BUFFER_BYTE_SIZE(LCD_WIDTH, LCD_HEIGHT, 16)] __attribute__((aligned(4)));
static bool                  ready    = false;
static bool                  panel_on = false;
static bool                  resend   = false; // the panel came back on: the preview sends the whole frame again

typedef struct {
    bool    os_auto, instant_qwerty, language_stash_empty, caps;
    uint8_t os, mode, language, layer, mods;
} status_t;

static status_t last_status;
static bool     has_last_status = false;
static char     shown[10][6]; // text on each row, so that only the rows that changed are redrawn

static void draw_row(uint8_t row, const char *text, bool is_label) {
    if (strcmp(shown[row], text) == 0) return;
    strncpy(shown[row], text, sizeof(shown[row]) - 1);
    const uint16_t y = row * ROW_HEIGHT;
    qp_rect(surface, 0, y, LCD_WIDTH - 1, y + ROW_HEIGHT - 1, COLOR_BG, true);
    if (is_label)
        qp_drawtext_recolor(surface, MARGIN_X, y, font, text, COLOR_LABEL, COLOR_BG);
    else
        qp_drawtext_recolor(surface, MARGIN_X, y, font, text, COLOR_VALUE, COLOR_BG);
}

static const char *os_text(uint8_t os) {
    switch (os) {
        case WINDOWS_OS:
            return "Win";
        case LINUX_OS:
            return "Linux";
        case MAC_OS:
            return "Mac";
        default:
            return "?";
    }
}

static const char *mode_text(const status_t *s) {
    switch (s->mode) {
        case QWERTY:
            return s->instant_qwerty ? "Qwrt*" : "Qwrty";
        case COLEMAK:
            return "Colmk";
        default:
            return "?";
    }
}

static const char *language_text(const status_t *s) {
    switch (s->language) {
        case PRIMARY_LANGUAGE:
            return s->language_stash_empty ? "Pri" : "Pri*";
        case SECONDARY_LANGUAGE:
            return "Alt";
        default:
            return "?";
    }
}

static const char *layer_text(uint8_t layer) {
    switch (layer) {
        case COLEMAK:
        case QWERTY:
            return "Base";
        case RAISE:
            return "Raise";
        case LOWER:
            return "Lower";
        case NUMPAD:
            return "Num";
        case EXTRA:
            return "Extra";
        default:
            return "?";
    }
}

static void draw_status(const status_t *s) {
    const char mods[] = {
        s->mods & MOD_MASK_SHIFT ? 'S' : ' ', s->mods & MOD_MASK_GUI ? 'G' : ' ', s->mods & MOD_MASK_ALT ? 'A' : ' ', s->mods & MOD_MASK_CTRL ? 'C' : ' ', '\0',
    };
    draw_row(0, s->os_auto ? "OS  a" : "OS", true);
    draw_row(1, os_text(s->os), false);
    draw_row(2, "MODE", true);
    draw_row(3, mode_text(s), false);
    draw_row(4, "LANG", true);
    draw_row(5, language_text(s), false);
    draw_row(6, "LAYER", true);
    draw_row(7, layer_text(s->layer), false);
    draw_row(8, mods, false);
    draw_row(9, s->caps ? "CAPS" : "", false);
}

void display_init(void) {
    if (!is_keyboard_master()) return;
    lcd     = qp_st7789_make_spi_device(LCD_WIDTH, LCD_HEIGHT, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, LCD_SPI_DIVISOR, LCD_SPI_MODE);
    surface = qp_make_rgb565_surface(LCD_WIDTH, LCD_HEIGHT, framebuffer);
    if (!qp_init(lcd, QP_ROTATION_0) || !qp_init(surface, QP_ROTATION_0)) return;
    qp_set_viewport_offsets(lcd, LCD_OFFSET_X, LCD_OFFSET_Y);
    font = qp_load_font_mem(font_spleen12x24);
    if (font == NULL) return;
    qp_rect(surface, 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, COLOR_BG, true);
    qp_power(lcd, true);
    ready = panel_on = true;
}

void display_task(void) {
    if (!ready) return;

    const bool idle = last_input_activity_elapsed() > SEK_DISPLAY_TIMEOUT;
    if (idle == panel_on) {
        panel_on = !idle;
        resend   = panel_on;
        qp_power(lcd, panel_on);
        if (panel_on)
            backlight_enable_noeeprom();
        else
            backlight_disable_noeeprom();
    }

    if (display_preview_active()) {
        if (display_preview_render(surface) || resend) qp_surface_draw(surface, lcd, 0, 0, true);
        resend = false;
        return;
    }

    const status_t status = {
        .os_auto              = is_os_autodetection_enabled(),
        .os                   = get_os(),
        .mode                 = get_highest_layer(default_layer_state),
        .instant_qwerty       = is_instant_qwerty_activated(),
        .language             = get_language(),
        .language_stash_empty = is_language_stash_empty(),
        .layer                = get_highest_layer(layer_state),
        .mods                 = get_mods() | get_oneshot_mods(),
        .caps                 = host_keyboard_led_state().caps_lock,
    };
    if (has_last_status && memcmp(&status, &last_status, sizeof(status)) == 0) return;
    last_status     = status;
    has_last_status = true;
    draw_status(&status);
    qp_surface_draw(surface, lcd, 0, 0, false);
}

// The suspend loop calls this repeatedly; the core has already set the backlight to 0.
void display_suspend(void) {
    if (ready && panel_on) {
        qp_power(lcd, false);
        panel_on = false;
    }
}

// The core has restored the backlight (backlight_init). The panel comes back on here;
// display_task() then turns both off again if the keyboard is still idle.
void display_wakeup(void) {
    if (ready && !panel_on) {
        qp_power(lcd, true);
        panel_on = resend = true;
    }
}
