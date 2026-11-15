// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// The Halcyon TFT module: the skin status screen (display_skin.c) of the view model
// (display_state.c), drawn into an rgb565 surface and sent to the panel by dirty area.

#include QMK_KEYBOARD_H
#include <string.h>
#include "qp.h"
#include "qp_surface.h"
#include "display.h"
#include "display_gfx.h"
#include "display_gfx_qp.h"
#include "display_skin.h"
#include "display_state.h"

static painter_device_t lcd;
// Aligned: the rgb565 surface does 16-bit stores into it, and the M0+ faults on misaligned access.
static uint8_t         framebuffer[SURFACE_REQUIRED_BUFFER_BYTE_SIZE(LCD_WIDTH, LCD_HEIGHT, 16)] __attribute__((aligned(4)));
static bool            ready    = false;
static bool            panel_on = false;
static bool            resend   = false; // the next display_task() redraws and sends the whole screen
static display_state_t last;

void display_init(void) {
    if (!is_keyboard_master()) return;
    if (((uintptr_t)framebuffer & 3) != 0) return; // stay dark rather than HardFault (see above)
    lcd                      = qp_st7789_make_spi_device(LCD_WIDTH, LCD_HEIGHT, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, LCD_SPI_DIVISOR, LCD_SPI_MODE);
    painter_device_t surface = qp_make_rgb565_surface(LCD_WIDTH, LCD_HEIGHT, framebuffer);
    if (!qp_init(lcd, QP_ROTATION_0) || !qp_init(surface, QP_ROTATION_0)) return;
    qp_set_viewport_offsets(lcd, LCD_OFFSET_X, LCD_OFFSET_Y);
    if (!gfx_qp_init(surface, lcd)) return;
    qp_power(lcd, true);
    ready = panel_on = resend = true;
}

void display_task(void) {
    if (!ready) return;

    const bool idle = last_input_activity_elapsed() > SEK_DISPLAY_TIMEOUT;
    if (idle == panel_on) {
        panel_on = !idle;
        qp_power(lcd, panel_on);
        if (panel_on) {
            backlight_enable_noeeprom();
            resend = true;
        } else {
            backlight_disable_noeeprom();
        }
    }

    display_state_t s;
    display_state_read(&s);

    // While the panel is off nothing is drawn; turning it on resends the whole screen.
    if (panel_on) {
        if (resend) {
            skin_draw_all(&s);
            gfx_flush(); // skin_draw_all() painted every pixel: this sends the whole screen
            resend = false;
        } else if (memcmp(&s, &last, sizeof(s)) != 0) {
            skin_draw_changes(&last, &s);
        }
    }
    last = s;
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
        panel_on = true;
        resend   = true;
    }
}
