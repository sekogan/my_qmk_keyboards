// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// display_gfx.h with Quantum Painter on the rgb565 surface. Colours go in as the HSV888 of
// display_palette.h; the surface turns them into the sketch's RGB565 exactly
// (hsv_to_rgb_nocie + truncation). The 1 bpp fonts and icons are recoloured with 2 steps, so
// ink is exactly fg and the rest of the cell exactly bg.

#include QMK_KEYBOARD_H
#include "qp.h"
#include "qp_surface.h"
#include "display_gfx.h"
#include "display_gfx_qp.h"
#include "graphics/font_val28_caps.qff.h"
#include "graphics/font_l24_caps.qff.h"
#include "graphics/font_l18_caps.qff.h"
#include "graphics/icon_shift.qgf.h"
#include "graphics/icon_ctrl.qgf.h"
#include "graphics/icon_alt.qgf.h"
#include "graphics/icon_super.qgf.h"
#include "graphics/icon_win.qgf.h"
#include "graphics/icon_cmd.qgf.h"

static painter_device_t       surface, lcd;
static painter_font_handle_t  fonts[3];
static painter_image_handle_t icons[6];

#define HSV(c) skin_hsv[c][0], skin_hsv[c][1], skin_hsv[c][2]

bool gfx_qp_init(painter_device_t surface_, painter_device_t lcd_) {
    static const uint8_t *const font_data[3] = {
        [GFX_VAL28] = font_font_val28_caps,
        [GFX_L24]   = font_font_l24_caps,
        [GFX_L18]   = font_font_l18_caps,
    };
    static const uint8_t *const icon_data[6] = {
        [GFX_ICON_SHIFT] = gfx_icon_shift, [GFX_ICON_CTRL] = gfx_icon_ctrl, [GFX_ICON_ALT] = gfx_icon_alt,
        [GFX_ICON_SUPER] = gfx_icon_super, [GFX_ICON_WIN] = gfx_icon_win,   [GFX_ICON_CMD] = gfx_icon_cmd,
    };
    surface = surface_;
    lcd     = lcd_;
    for (uint8_t i = 0; i < 3; ++i)
        if ((fonts[i] = qp_load_font_mem(font_data[i])) == NULL) return false;
    for (uint8_t i = 0; i < 6; ++i)
        if ((icons[i] = qp_load_image_mem(icon_data[i])) == NULL) return false;
    return true;
}

void gfx_fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
    qp_rect(surface, x, y, x + w - 1, y + h - 1, HSV(color), true);
}

void gfx_text(uint8_t x, uint8_t y, uint8_t font, const char *s, uint8_t fg, uint8_t bg) {
    qp_drawtext_recolor(surface, x, y, fonts[font], s, HSV(fg), HSV(bg));
}

void gfx_icon(uint8_t x, uint8_t y, uint8_t icon, uint8_t fg, uint8_t bg) {
    qp_drawimage_recolor(surface, x, y, icons[icon], HSV(fg), HSV(bg));
}

void gfx_flush(void) {
    qp_surface_draw(surface, lcd, 0, 0, false);
}
