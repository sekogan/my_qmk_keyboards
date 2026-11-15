// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// The skin status screen (the oracle is
// scripts/elora_display_test/reference/sketch/skin.py). Pixel-exact with the sketch's natives:
// scripts/check-elora-display.sh.
//
// Every group paints its box completely (fill, then text or icon whose cells paint their own
// background), so a group can be redrawn on its own over whatever was there.

#include <stdbool.h>
#include "display_skin.h"
#include "display_gfx.h"
#include "display_skin_layout.h"

// A lamp: lit = full fill + BLACK content; off = fill x0.22 + content x0.45.
typedef struct {
    uint8_t on, off_fill, off_txt;
} lamp_colors_t;

// A meter segment: lit = full colour, off = colour x0.22.
typedef struct {
    uint8_t on, off;
} seg_colors_t;

#define LAMP(name) {SKIN_##name, SKIN_##name##_OFF_FILL, SKIN_##name##_OFF_TXT}
#define SEG(name) {SKIN_##name, SKIN_##name##_OFF_FILL}

static const lamp_colors_t AMBER          = LAMP(AMBER);
static const lamp_colors_t MOD_COLORS[4]  = {LAMP(SHIFT), LAMP(CTRL), LAMP(ALT), LAMP(GUI)};
static const lamp_colors_t LOCK_COLORS[4] = {LAMP(LAMP_G), LAMP(AMBER), LAMP(LAMP_G), LAMP(LAMP_G)}; // CAPS, WORD, NUM, SCRL
static const seg_colors_t  SEG_COLOR[10]  = {
    SEG(LAMP_G), SEG(LAMP_G), SEG(LAMP_G), SEG(LAMP_G), SEG(LAMP_G), SEG(LAMP_G), SEG(FL_AMBER), SEG(FL_AMBER), SEG(ALERT), SEG(ALERT),
};

static const char *const LAYER_NAME[5] = {"BASE", "LOWER", "RAISE", "NUMPAD", "EXTRA"};
static const uint8_t     TALLY_FILL[5] = {SKIN_BASE_FILL, SKIN_TALLY_LOWER, SKIN_TALLY_RAISE, SKIN_TALLY_NUMPAD, SKIN_TALLY_EXTRA};
static const uint8_t     TALLY_TXT[5]  = {SKIN_BASE_TXT, SKIN_BLACK, SKIN_BLACK, SKIN_BLACK, SKIN_BLACK};
static const char *const OS_NAME[4]    = {"LINUX", "WINDOWS", "MACOS", "UNKNOWN"};
static const uint8_t     GUI_ICON[4]   = {GFX_ICON_SUPER, GFX_ICON_WIN, GFX_ICON_CMD, GFX_ICON_SUPER}; // by ds_os_t
static const char *const LOCK_NAME[4]  = {"CAPS", "WORD", "NUM", "SCRL"};
static const uint8_t     LOCK_BAR_X[4] = {SKIN_LOCK_X1, SKIN_LOCK_X2, SKIN_LOCK_X1, SKIN_LOCK_X2};
static const uint8_t     LOCK_BAR_Y[4] = {SKIN_LOCK_Y1, SKIN_LOCK_Y1, SKIN_LOCK_Y2, SKIN_LOCK_Y2};

static uint8_t lamp_fill(const lamp_colors_t *c, bool lit) {
    return lit ? c->on : c->off_fill;
}

static uint8_t lamp_ink(const lamp_colors_t *c, bool lit) {
    return lit ? SKIN_BLACK : c->off_txt;
}

static void lamp(uint8_t bx, uint8_t by, uint8_t bw, uint8_t bh, uint8_t tx, uint8_t ty, const char *label, uint8_t font, const lamp_colors_t *c, bool lit) {
    const uint8_t fill = lamp_fill(c, lit);
    gfx_fill(bx, by, bw, bh, fill);
    gfx_text(tx, ty, font, label, lamp_ink(c, lit), fill);
}

static bool mod_lit(const display_state_t *s, uint8_t k) {
    switch (k) {
        case 0:
            return s->shift;
        case 1:
            return s->ctrl;
        case 2:
            return s->alt;
        default:
            return s->gui;
    }
}

// Mod k's icon: GFX_ICON_SHIFT, _CTRL, _ALT, and the GUI icon of the OS.
static uint8_t mod_icon(const display_state_t *s, uint8_t k) {
    return k == 3 ? GUI_ICON[s->os] : k;
}

static bool lock_lit(const display_state_t *s, uint8_t k) {
    switch (k) {
        case 0:
            return s->caps;
        case 1:
            return s->word;
        case 2:
            return s->num;
        default:
            return s->scrl;
    }
}

// "{n}/10" without printf.
static void level_text(char buf[6], uint8_t n) {
    uint8_t i = 0;
    if (n >= 10) buf[i++] = (char)('0' + n / 10);
    buf[i++] = (char)('0' + n % 10);
    buf[i++] = '/';
    buf[i++] = '1';
    buf[i++] = '0';
    buf[i]   = '\0';
}

// ---------------------------------------------------------------- main screen
// Each group is drawn by draw(s, k) and shows exactly what key(s, k) encodes, so it needs a
// redraw when its key changes.

static void draw_tally(const display_state_t *s, uint8_t k) {
    (void)k;
    const uint8_t fill = TALLY_FILL[s->layer];
    gfx_fill(SKIN_X0, SKIN_TALLY_Y, SKIN_CW, SKIN_TALLY_H, fill);
    gfx_text(SKIN_TALLY_X[s->layer], SKIN_TALLY_TEXT_Y, GFX_L24, LAYER_NAME[s->layer], TALLY_TXT[s->layer], fill);
}

static uint8_t tally_key(const display_state_t *s, uint8_t k) {
    (void)k;
    return s->layer;
}

static void draw_mod(const display_state_t *s, uint8_t k) {
    const uint8_t        x    = SKIN_X0 + SKIN_MOD_PITCH * k;
    const bool           lit  = mod_lit(s, k);
    const lamp_colors_t *c    = &MOD_COLORS[k];
    const uint8_t        fill = lamp_fill(c, lit);
    gfx_fill(x, SKIN_MOD_Y, SKIN_MOD_SIZE, SKIN_MOD_SIZE, fill);
    gfx_icon(x + SKIN_ICON_INSET, SKIN_MOD_Y + SKIN_ICON_INSET, mod_icon(s, k), lamp_ink(c, lit), fill);
}

static uint8_t mod_key(const display_state_t *s, uint8_t k) {
    return (uint8_t)(mod_icon(s, k) << 1 | mod_lit(s, k));
}

static void draw_lock(const display_state_t *s, uint8_t k) {
    lamp(LOCK_BAR_X[k], LOCK_BAR_Y[k], SKIN_LOCK_W, SKIN_LOCK_H, SKIN_LOCK_X[k], LOCK_BAR_Y[k] + SKIN_LOCK_TEXT_DY, LOCK_NAME[k], GFX_L24, &LOCK_COLORS[k], lock_lit(s, k));
}

static uint8_t lock_key(const display_state_t *s, uint8_t k) {
    return lock_lit(s, k);
}

static void draw_tag(uint8_t legend_y, bool lit) {
    lamp(SKIN_TAG_X, legend_y - SKIN_TAG_DY, SKIN_TAG_W, SKIN_TAG_H, SKIN_TAG_TEXT_X, legend_y, "AUTO", GFX_L18, &AMBER, lit);
}

static void draw_value(uint8_t y, const char *s, uint8_t color) {
    gfx_fill(SKIN_VALUE_X, y, SKIN_VALUE_W, SKIN_VAL28_H, SKIN_BLACK);
    gfx_text(SKIN_VALUE_X, y, GFX_VAL28, s, color, SKIN_BLACK);
}

static void draw_os_zone(const display_state_t *s, uint8_t k) {
    (void)k;
    gfx_text(SKIN_LEGEND_X, SKIN_OS_LEGEND_Y, GFX_L18, "OS", SKIN_PANEL, SKIN_BLACK);
    draw_tag(SKIN_OS_LEGEND_Y, s->os_auto);
    draw_value(SKIN_OS_VALUE_Y, OS_NAME[s->os], s->os == DS_OS_UNKNOWN ? SKIN_ALERT : SKIN_VFD);
}

static uint8_t os_key(const display_state_t *s, uint8_t k) {
    (void)k;
    return (uint8_t)(s->os << 1 | s->os_auto);
}

static void draw_layout_zone(const display_state_t *s, uint8_t k) {
    (void)k;
    gfx_text(SKIN_LEGEND_X, SKIN_LAYOUT_LEGEND_Y, GFX_L18, "LAYOUT", SKIN_PANEL, SKIN_BLACK);
    draw_tag(SKIN_LAYOUT_LEGEND_Y, s->layout_auto);
    draw_value(SKIN_LAYOUT_VALUE_Y, s->qwerty ? "QWERTY" : "COLEMAK", SKIN_VFD);
}

static uint8_t layout_key(const display_state_t *s, uint8_t k) {
    (void)k;
    return (uint8_t)(s->qwerty << 1 | s->layout_auto);
}

static void draw_language_zone(const display_state_t *s, uint8_t k) {
    (void)k;
    gfx_text(SKIN_LEGEND_X, SKIN_LANG_LEGEND_Y, GFX_L18, "LANGUAGE", SKIN_PANEL, SKIN_BLACK);
    if (s->secondary)
        draw_value(SKIN_LANG_VALUE_Y, "SECONDARY", s->lang_muted ? SKIN_MUTED_AMBER : SKIN_FL_AMBER);
    else
        draw_value(SKIN_LANG_VALUE_Y, "PRIMARY", s->lang_muted ? SKIN_MUTED : SKIN_VFD);
}

static uint8_t language_key(const display_state_t *s, uint8_t k) {
    (void)k;
    return (uint8_t)(s->secondary << 1 | s->lang_muted);
}

typedef struct {
    void (*draw)(const display_state_t *s, uint8_t k);
    uint8_t (*key)(const display_state_t *s, uint8_t k);
    uint8_t k;
} group_t;

// In drawing order.
static const group_t GROUPS[] = {
    {draw_tally, tally_key, 0},
    {draw_mod, mod_key, 0},
    {draw_mod, mod_key, 1},
    {draw_mod, mod_key, 2},
    {draw_mod, mod_key, 3},
    {draw_lock, lock_key, 0},
    {draw_lock, lock_key, 1},
    {draw_lock, lock_key, 2},
    {draw_lock, lock_key, 3},
    {draw_os_zone, os_key, 0},
    {draw_layout_zone, layout_key, 0},
    {draw_language_zone, language_key, 0},
};

// ---------------------------------------------------------------- popup

static void draw_level(uint8_t n) {
    char buf[6];
    level_text(buf, n);
    gfx_fill(SKIN_X0, SKIN_READOUT_Y, SKIN_CW, SKIN_VAL28_H, SKIN_BLACK);
    gfx_text(SKIN_READOUT_X[n], SKIN_READOUT_Y, GFX_VAL28, buf, SKIN_VFD, SKIN_BLACK);
    for (uint8_t k = 0; k < 10; ++k) gfx_fill(SKIN_SEG_X0 + SKIN_SEG_PITCH * k, SKIN_SEG_Y, SKIN_SEG_W, SKIN_SEG_H, k + 1 <= n ? SEG_COLOR[k].on : SEG_COLOR[k].off);
}

static void draw_popup(const display_state_t *s) {
    gfx_text(SKIN_P1_X, SKIN_P1_Y, GFX_L24, "DISPLAY", SKIN_PANEL, SKIN_BLACK);
    gfx_text(SKIN_P2_X, SKIN_P2_Y, GFX_L24, "BACKLIGHT", SKIN_PANEL, SKIN_BLACK);
    draw_level(s->backlight);
    gfx_text(SKIN_SCALE_X0, SKIN_SCALE_Y, GFX_L18, "0", SKIN_PANEL, SKIN_BLACK);
    gfx_text(SKIN_SCALE_X5, SKIN_SCALE_Y, GFX_L18, "5", SKIN_PANEL, SKIN_BLACK);
    gfx_text(SKIN_SCALE_X10, SKIN_SCALE_Y, GFX_L18, "10", SKIN_PANEL, SKIN_BLACK);
}

// ---------------------------------------------------------------- entry points

void skin_draw_all(const display_state_t *s) {
    gfx_fill(0, 0, SKIN_W, SKIN_H, SKIN_BLACK);
    if (s->overlay) {
        draw_popup(s);
    } else {
        for (uint8_t g = 0; g < sizeof GROUPS / sizeof GROUPS[0]; ++g) GROUPS[g].draw(s, GROUPS[g].k);
    }
}

void skin_draw_changes(const display_state_t *old, const display_state_t *s) {
    if (old->overlay != s->overlay) { // popup <-> main: the screens share nothing
        skin_draw_all(s);
        gfx_flush();
    } else if (s->overlay) { // only the level can change in the popup
        if (old->backlight != s->backlight) {
            draw_level(s->backlight);
            gfx_flush();
        }
    } else {
        for (uint8_t g = 0; g < sizeof GROUPS / sizeof GROUPS[0]; ++g) {
            const group_t *grp = &GROUPS[g];
            if (grp->key(old, grp->k) != grp->key(s, grp->k)) {
                grp->draw(s, grp->k);
                gfx_flush();
            }
        }
    }
}
