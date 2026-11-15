// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// The four drawing primitives the renderer (display_skin.c) uses. The firmware implements them
// with Quantum Painter on the rgb565 surface (display_gfx_qp.c); the host check
// (scripts/elora_display_test/host_gfx.c) records them. Coordinates are pixels on the 135x240
// panel; colours are enum skin_color (display_palette.h).

#include <stdint.h>
#include "display_palette.h"

// The fonts: cropped QFF cells (graphics/font_*_caps), one BDF cell wide, no gap between glyphs.
enum gfx_font {
    GFX_VAL28, // Terminus 14x28 bold derivative, rows 4..23 of the cell: 14 x 20
    GFX_L24,   // Leggie 12x24 bold, rows 4..21: 12 x 18
    GFX_L18,   // Leggie 9x18 bold, rows 4..14: 9 x 11
};

// The 20x20 mod icons (graphics/icon_*).
enum gfx_icon {
    GFX_ICON_SHIFT,
    GFX_ICON_CTRL,
    GFX_ICON_ALT,
    GFX_ICON_SUPER, // the diamond: Linux and unknown OS
    GFX_ICON_WIN,
    GFX_ICON_CMD,
};

// Fill the w x h rectangle at (x, y).
void gfx_fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
// Draw s with its first cell's top-left at (x, y): ink in fg, the rest of every cell in bg.
void gfx_text(uint8_t x, uint8_t y, uint8_t font, const char *s, uint8_t fg, uint8_t bg);
// Draw an icon with its top-left at (x, y): ink in fg, the rest of the 20x20 in bg.
void gfx_icon(uint8_t x, uint8_t y, uint8_t icon, uint8_t fg, uint8_t bg);
// Send what changed to the panel.
void gfx_flush(void);
