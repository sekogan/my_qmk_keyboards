// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// skin layout, measured from the sketch (scripts/elora_display_test/reference/sketch/skin.py)
// and its natives. Boxes are x, y, w, h (last pixel x + w - 1). Text origins are firmware
// origins: the sketch's text() cell with y + 4, because the fonts are cropped to cell rows 4..
// (VAL28 20 rows, L24 18, L18 11; advances 14, 12, 9). The centered x tables are generated:
// display_skin_metrics.h.

#include "display_skin_metrics.h"

#define SKIN_W 135
#define SKIN_H 240

// Font cells after the crop.
#define SKIN_VAL28_ADV 14
#define SKIN_VAL28_H 20
#define SKIN_L24_ADV 12
#define SKIN_L24_H 18
#define SKIN_L18_ADV 9
#define SKIN_L18_H 11
// Cap rows inside the cropped cells (display_skin.md section 2): L24 caps are cell rows 2..15,
// VAL28 caps rows 0..17.
#define SKIN_L24_CAP_DY 2
#define SKIN_L24_CAP_H 14
#define SKIN_VAL28_CAP_H 18

// Content block: x 4..130.
#define SKIN_X0 4
#define SKIN_CW 127

// E1 tally: box 4,4,127,20; L24 at (SKIN_TALLY_X[layer], 5).
#define SKIN_TALLY_Y 4
#define SKIN_TALLY_H 20
#define SKIN_TALLY_TEXT_Y 5

// E2-E5 mod tiles: 28x28 at x 4 + 33k, y 29; the icon at tile + 4.
#define SKIN_MOD_Y 29
#define SKIN_MOD_SIZE 28
#define SKIN_MOD_PITCH 33
#define SKIN_ICON_INSET 4
#define SKIN_ICON_SIZE 20

// E6-E9 lock bars: 61x20; L24 label at (SKIN_LOCK_X[k], bar y + 1).
#define SKIN_LOCK_W 61
#define SKIN_LOCK_H 20
#define SKIN_LOCK_X1 4  // CAPS, NUM
#define SKIN_LOCK_X2 70 // WORD, SCRL
#define SKIN_LOCK_Y1 62 // CAPS, WORD
#define SKIN_LOCK_Y2 87 // NUM, SCRL
#define SKIN_LOCK_TEXT_DY 1

// Sections (no lines): L18 legend at x 3, an AUTO tag box 92,y,39,15 with its L18 text at
// (93, legend y), the VAL28 value at x 3 over a cleared 128x20 rect.
#define SKIN_LEGEND_X 3
#define SKIN_TAG_X 92
#define SKIN_TAG_W 39
#define SKIN_TAG_H 15
#define SKIN_TAG_TEXT_X 93
#define SKIN_TAG_DY 2 // tag box y = legend y - 2
#define SKIN_VALUE_X 3
#define SKIN_VALUE_W 128
#define SKIN_OS_LEGEND_Y 117     // E11, E12
#define SKIN_OS_VALUE_Y 132      // E13
#define SKIN_LAYOUT_LEGEND_Y 160 // E15, E16
#define SKIN_LAYOUT_VALUE_Y 175  // E17
#define SKIN_LANG_LEGEND_Y 203   // E19
#define SKIN_LANG_VALUE_Y 218    // E20

// Popup (full screen, nothing else drawn).
#define SKIN_P1_X 25 // DISPLAY
#define SKIN_P1_Y 66
#define SKIN_P2_X 13 // BACKLIGHT
#define SKIN_P2_Y 85
#define SKIN_READOUT_Y 111 // O2: VAL28 at (SKIN_READOUT_X[n], 111) over a cleared 4,111,127,20
#define SKIN_SEG_X0 4      // O3: k = 0..9 at (4 + 13k, 139), 10x16
#define SKIN_SEG_PITCH 13
#define SKIN_SEG_Y 139
#define SKIN_SEG_W 10
#define SKIN_SEG_H 16
#define SKIN_SCALE_Y 161 // O5: L18 "0" "5" "10"
#define SKIN_SCALE_X0 3
#define SKIN_SCALE_X5 62
#define SKIN_SCALE_X10 113

// Version popup (full screen, nothing else drawn): L24 labels at (SKIN_VER_X_<label>, y), the VAL28
// versions centered at run time (display_skin.c version_x()). Ink rows 74-87, 94-111, 128-141,
// 148-165: label caps, 6 rows, value caps, 16 rows, label caps, 6 rows, value caps; 74 black
// rows above and below.
#define SKIN_VER_KEYMAP_Y 72   // KEYMAP
#define SKIN_VER_KEYMAP_V_Y 94 // the keymap version (VERSION)
#define SKIN_VER_QMK_Y 126     // QMK
#define SKIN_VER_QMK_V_Y 148   // the QMK version
#define SKIN_VER_LABEL_GAP 6   // label caps -> its value caps
#define SKIN_VER_GROUP_GAP 16  // keymap version caps -> QMK caps
#define SKIN_VER_MAX_GLYPHS (SKIN_CW / SKIN_VAL28_ADV) // 9: a longer version would not fit the content width

// The geometry adds up.
_Static_assert(4 * SKIN_MOD_SIZE + 3 * (SKIN_MOD_PITCH - SKIN_MOD_SIZE) == SKIN_CW, "mod tiles span the content width");
_Static_assert(SKIN_LOCK_W + (SKIN_LOCK_X2 - SKIN_LOCK_X1 - SKIN_LOCK_W) + SKIN_LOCK_W == SKIN_CW, "lock bars span the content width");
_Static_assert(10 * SKIN_SEG_W + 9 * (SKIN_SEG_PITCH - SKIN_SEG_W) == SKIN_CW, "segments span the content width");
_Static_assert(SKIN_X0 + SKIN_CW + SKIN_X0 == SKIN_W, "even side margins");
_Static_assert(2 * SKIN_ICON_INSET + SKIN_ICON_SIZE == SKIN_MOD_SIZE, "icons centered in their tiles");

// Every cropped text cell lies inside its fill, or on black with nothing else on its pixels.
#define SKIN_INSIDE(cx, cy, cw, ch, bx, by, bw, bh) ((cx) >= (bx) && (cy) >= (by) && (cx) + (cw) <= (bx) + (bw) && (cy) + (ch) <= (by) + (bh))
_Static_assert(SKIN_INSIDE(SKIN_LOCK_X_CAPS, SKIN_LOCK_Y1 + SKIN_LOCK_TEXT_DY, 4 * SKIN_L24_ADV, SKIN_L24_H, SKIN_LOCK_X1, SKIN_LOCK_Y1, SKIN_LOCK_W, SKIN_LOCK_H), "CAPS cell in its bar");
_Static_assert(SKIN_INSIDE(SKIN_LOCK_X_WORD, SKIN_LOCK_Y1 + SKIN_LOCK_TEXT_DY, 4 * SKIN_L24_ADV, SKIN_L24_H, SKIN_LOCK_X2, SKIN_LOCK_Y1, SKIN_LOCK_W, SKIN_LOCK_H), "WORD cell in its bar");
_Static_assert(SKIN_INSIDE(SKIN_LOCK_X_NUM, SKIN_LOCK_Y2 + SKIN_LOCK_TEXT_DY, 3 * SKIN_L24_ADV, SKIN_L24_H, SKIN_LOCK_X1, SKIN_LOCK_Y2, SKIN_LOCK_W, SKIN_LOCK_H), "NUM cell in its bar");
_Static_assert(SKIN_INSIDE(SKIN_LOCK_X_SCRL, SKIN_LOCK_Y2 + SKIN_LOCK_TEXT_DY, 4 * SKIN_L24_ADV, SKIN_L24_H, SKIN_LOCK_X2, SKIN_LOCK_Y2, SKIN_LOCK_W, SKIN_LOCK_H), "SCRL cell in its bar");
_Static_assert(SKIN_INSIDE(SKIN_TALLY_X_NUMPAD, SKIN_TALLY_TEXT_Y, 6 * SKIN_L24_ADV, SKIN_L24_H, SKIN_X0, SKIN_TALLY_Y, SKIN_CW, SKIN_TALLY_H), "widest tally cell (NUMPAD) in the box");
_Static_assert(SKIN_INSIDE(SKIN_TAG_TEXT_X, SKIN_OS_LEGEND_Y, 4 * SKIN_L18_ADV, SKIN_L18_H, SKIN_TAG_X, SKIN_OS_LEGEND_Y - SKIN_TAG_DY, SKIN_TAG_W, SKIN_TAG_H), "AUTO cell in its tag");
_Static_assert(SKIN_LEGEND_X + 8 * SKIN_L18_ADV <= SKIN_TAG_X, "the widest legend cell (LANGUAGE) ends left of the tags");
_Static_assert(9 * SKIN_VAL28_ADV <= SKIN_VALUE_W, "widest value cell (SECONDARY) inside the cleared value rect");
_Static_assert(SKIN_OS_LEGEND_Y - SKIN_TAG_DY + SKIN_TAG_H <= SKIN_OS_VALUE_Y, "OS value below its strip");
_Static_assert(SKIN_OS_VALUE_Y + SKIN_VAL28_H <= SKIN_LAYOUT_LEGEND_Y - SKIN_TAG_DY, "OS value above the layout strip");
_Static_assert(SKIN_LAYOUT_LEGEND_Y - SKIN_TAG_DY + SKIN_TAG_H <= SKIN_LAYOUT_VALUE_Y, "layout value below its strip");
_Static_assert(SKIN_LAYOUT_VALUE_Y + SKIN_VAL28_H <= SKIN_LANG_LEGEND_Y, "E17 cell (Q tail) ends before the language legend");
_Static_assert(SKIN_LANG_LEGEND_Y + SKIN_L18_H <= SKIN_LANG_VALUE_Y, "language value below its legend");
_Static_assert(SKIN_LANG_VALUE_Y + SKIN_VAL28_H <= SKIN_H, "language value on the screen");
_Static_assert(SKIN_LOCK_Y2 + SKIN_LOCK_H <= SKIN_OS_LEGEND_Y - SKIN_TAG_DY, "locks above the OS strip");
_Static_assert(SKIN_P1_Y + SKIN_L24_H <= SKIN_P2_Y, "P1 and P2 cells do not overlap");
_Static_assert(SKIN_P2_Y + SKIN_L24_H <= SKIN_READOUT_Y, "BACKLIGHT above the readout");
_Static_assert(SKIN_READOUT_Y + SKIN_VAL28_H <= SKIN_SEG_Y, "readout above the segments");
_Static_assert(SKIN_SEG_Y + SKIN_SEG_H <= SKIN_SCALE_Y, "segments above the scale");
_Static_assert(SKIN_SCALE_X10 + 2 * SKIN_L18_ADV <= SKIN_W, "scale on the screen");
_Static_assert(SKIN_VER_KEYMAP_Y + SKIN_L24_CAP_DY + SKIN_L24_CAP_H + SKIN_VER_LABEL_GAP == SKIN_VER_KEYMAP_V_Y, "KEYMAP caps 6 rows above its version");
_Static_assert(SKIN_VER_KEYMAP_V_Y + SKIN_VAL28_CAP_H + SKIN_VER_GROUP_GAP == SKIN_VER_QMK_Y + SKIN_L24_CAP_DY, "16 rows from the keymap version to the QMK caps");
_Static_assert(SKIN_VER_QMK_Y + SKIN_L24_CAP_DY + SKIN_L24_CAP_H + SKIN_VER_LABEL_GAP == SKIN_VER_QMK_V_Y, "QMK caps 6 rows above its version");
_Static_assert(SKIN_VER_KEYMAP_Y + SKIN_L24_CAP_DY == SKIN_H - (SKIN_VER_QMK_V_Y + SKIN_VAL28_CAP_H), "the version group centered vertically");
_Static_assert(SKIN_VER_KEYMAP_Y + SKIN_L24_H <= SKIN_VER_KEYMAP_V_Y && SKIN_VER_KEYMAP_V_Y + SKIN_VAL28_H <= SKIN_VER_QMK_Y && SKIN_VER_QMK_Y + SKIN_L24_H <= SKIN_VER_QMK_V_Y, "version popup cells do not overlap");
_Static_assert(SKIN_VER_QMK_V_Y + SKIN_VAL28_H <= SKIN_H, "QMK version on the screen");
_Static_assert(SKIN_VER_X_KEYMAP >= 0 && SKIN_VER_X_KEYMAP + 6 * SKIN_L24_ADV <= SKIN_W && SKIN_VER_X_QMK + 3 * SKIN_L24_ADV <= SKIN_W, "version labels on the screen");
