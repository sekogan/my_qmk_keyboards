// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "display_preview.h"
#include "graphics/preview/preview_t01.qgf.h"
#include "graphics/preview/preview_t02.qgf.h"
#include "graphics/preview/preview_t03.qgf.h"
#include "graphics/preview/preview_t04.qgf.h"
#include "graphics/preview/preview_t05.qgf.h"
#include "graphics/preview/preview_t06.qgf.h"
#include "graphics/preview/preview_t07.qgf.h"
#include "graphics/preview/preview_t08.qgf.h"
#include "graphics/preview/preview_t09.qgf.h"
#include "graphics/preview/preview_t10.qgf.h"

// Sony i12.2 and i14, interleaved per state (i12.2, then i14);
// see display-sketches/hw-test/designs-sony-i12.2-i14.md.
static const uint8_t *const pictures[] = {
    gfx_preview_t01, gfx_preview_t02, // idle
    gfx_preview_t03, gfx_preview_t04, // lower-secondary
    gfx_preview_t05, gfx_preview_t06, // extra-no-os
    gfx_preview_t07, gfx_preview_t08, // numpad-shortcut
    gfx_preview_t09, gfx_preview_t10, // backlight-adjust
};
#define PICTURE_COUNT ((int8_t)ARRAY_SIZE(pictures))

static int8_t current = -1; // -1: preview mode is off
static bool   pending = false;

void display_preview_step(int8_t direction) {
    if (current < 0)
        current = direction > 0 ? 0 : PICTURE_COUNT - 1;
    else
        current = (current + (direction > 0 ? 1 : PICTURE_COUNT - 1)) % PICTURE_COUNT;
    pending = true;
}

bool display_preview_active(void) {
    return current >= 0;
}

bool display_preview_render(painter_device_t surface) {
    if (!pending) return false;
    pending = false;
    painter_image_handle_t image = qp_load_image_mem(pictures[current]);
    if (image == NULL) return false;
    qp_drawimage(surface, 0, 0, image);
    qp_close_image(image);
    return true;
}
