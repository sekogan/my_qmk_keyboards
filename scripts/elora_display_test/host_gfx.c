// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// display_gfx.h on the host: one line per primitive on stdout, rasterised by compare.py.
//   F x y w h color
//   T x y font fg bg text
//   I x y icon fg bg
//   X                      (flush)

#include <stdio.h>
#include "display_gfx.h"

void gfx_fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
    printf("F %u %u %u %u %u\n", x, y, w, h, color);
}

void gfx_text(uint8_t x, uint8_t y, uint8_t font, const char *s, uint8_t fg, uint8_t bg) {
    printf("T %u %u %u %u %u %s\n", x, y, font, fg, bg, s);
}

void gfx_icon(uint8_t x, uint8_t y, uint8_t icon, uint8_t fg, uint8_t bg) {
    printf("I %u %u %u %u %u\n", x, y, icon, fg, bg);
}

void gfx_flush(void) {
    printf("X\n");
}
