// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// Host driver for the Elora display check (scripts/check-elora-display.sh).
//   render_states palette   P <index> <rgb565> per colour; fails unless QMK's hsv_to_rgb_nocie()
//                           (qmk_firmware/quantum/color.c) of skin_hsv[] truncates to skin_565[]
//   render_states fields    the <state> field names (DS_FIELDS), one line
//   render_states list      the five design test states (display_test_states.c) as <state> lines
//   render_states render    reads states from stdin, one per line:
//                             A <state>            skin_draw_all(state)
//                             C <state> <state>    skin_draw_changes(a, b) only (no skin_draw_all(a))
//                           <state> = the DS_FIELDS values in order, as integers
//   render_states model     ds_resolve() cases (popup timer and EXTRA rule, Caps Word Shift,
//                           pre-LOWER snapshot, language muting)
// Frames are "BEGIN <n>" ... primitives (host_gfx.c) ... "END".

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "color.h"
#include "display_palette.h"
#include "display_skin.h"
#include "display_state.h"

// display_test_states.c (host only)
extern const display_state_t display_test_states[5];

// Every display_state_t field, in the <state> order.
#define DS_FIELDS(X) X(layer) X(os) X(backlight) X(os_auto) X(qwerty) X(layout_auto) X(secondary) X(lang_muted) X(shift) X(ctrl) X(alt) X(gui) X(caps) X(word) X(num) X(scrl) X(overlay)

#define DS_FIELD_SIZE(f) +sizeof(((display_state_t *)0)->f)
_Static_assert(0 DS_FIELDS(DS_FIELD_SIZE) == sizeof(display_state_t), "DS_FIELDS lists every display_state_t field");

static int palette(void) {
    int fail = 0;
    for (int i = 0; i < SKIN_NUM_COLORS; ++i) {
        const rgb_t    rgb = hsv_to_rgb_nocie((hsv_t){.h = skin_hsv[i][0], .s = skin_hsv[i][1], .v = skin_hsv[i][2]});
        const uint16_t c   = (uint16_t)((rgb.r >> 3) << 11 | (rgb.g >> 2) << 5 | (rgb.b >> 3));
        if (c != skin_565[i]) {
            fprintf(stderr, "palette %d: HSV %u,%u,%u gives 0x%04X, want 0x%04X\n", i, skin_hsv[i][0], skin_hsv[i][1], skin_hsv[i][2], c, skin_565[i]);
            fail = 1;
        }
        printf("P %d %u\n", i, skin_565[i]);
    }
    return fail;
}

static int parse_state(char **cur, display_state_t *s) {
    char *end;
    long  v;
#define DS_PARSE(f)                  \
    v = strtol(*cur, &end, 10);      \
    if (end == *cur) return 0;       \
    *cur = end;                      \
    s->f = v;
    DS_FIELDS(DS_PARSE)
#undef DS_PARSE
    return 1;
}

static int fields(void) {
    const char *sep = "";
#define DS_NAME(f) printf("%s%s", sep, #f), sep = " ";
    DS_FIELDS(DS_NAME)
#undef DS_NAME
    printf("\n");
    return 0;
}

static int render(void) {
    char line[512];
    int  n = 0;
    while (fgets(line, sizeof line, stdin)) {
        char           *cur = line + 1;
        display_state_t a, b;
        if (line[0] == 'A' && parse_state(&cur, &a)) {
            printf("BEGIN %d\n", n++);
            skin_draw_all(&a);
            printf("END\n");
        } else if (line[0] == 'C' && parse_state(&cur, &a) && parse_state(&cur, &b)) {
            printf("BEGIN %d\n", n++);
            skin_draw_changes(&a, &b);
            printf("END\n");
        } else {
            fprintf(stderr, "bad input line: %s", line);
            return 1;
        }
    }
    return 0;
}

static int list(void) {
    for (int i = 0; i < 5; ++i) {
        const display_state_t *s   = &display_test_states[i];
        const char            *sep = "";
#define DS_PRINT(f) printf("%s%d", sep, (int)s->f), sep = " ";
        DS_FIELDS(DS_PRINT)
#undef DS_PRINT
        printf("\n");
    }
    return 0;
}

// ---------------------------------------------------------------- ds_resolve() cases

static int cases = 0, failures = 0;

#define EXPECT(cond)                                                     \
    do {                                                                 \
        ++cases;                                                         \
        if (!(cond)) {                                                   \
            fprintf(stderr, "model case failed, line %d: %s\n", __LINE__, #cond); \
            ++failures;                                                  \
        }                                                                \
    } while (0)

static display_state_t at(ds_ctx_t *ctx, uint8_t layer, uint32_t now) {
    const ds_raw_t  raw = {.layer = layer, .os = DS_OS_LINUX, .now = now};
    display_state_t out;
    ds_resolve(&raw, ctx, &out);
    return out;
}

static int model(void) {
    { // popup: SEK_DISPLAY_BL_OVERLAY_MS after the last press, on EXTRA
        ds_ctx_t ctx = {0};
        EXPECT(!at(&ctx, DS_LAYER_EXTRA, 1000).overlay);
        ds_backlight_pressed(&ctx, 1000);
        EXPECT(at(&ctx, DS_LAYER_EXTRA, 1000).overlay);
        EXPECT(at(&ctx, DS_LAYER_EXTRA, 1000 + SEK_DISPLAY_BL_OVERLAY_MS - 1).overlay);
        EXPECT(!at(&ctx, DS_LAYER_EXTRA, 1000 + SEK_DISPLAY_BL_OVERLAY_MS).overlay);
        EXPECT(!at(&ctx, DS_LAYER_EXTRA, 1000 + SEK_DISPLAY_BL_OVERLAY_MS - 1).overlay); // disarmed for good
    }
    { // hidden at once when EXTRA is left; re-entering EXTRA does not bring it back
        ds_ctx_t ctx = {0};
        ds_backlight_pressed(&ctx, 5000);
        EXPECT(at(&ctx, DS_LAYER_EXTRA, 5001).overlay);
        EXPECT(!at(&ctx, DS_LAYER_LOWER, 5002).overlay);
        EXPECT(!at(&ctx, DS_LAYER_EXTRA, 5003).overlay);
        ds_backlight_pressed(&ctx, 5004); // a new press does
        EXPECT(at(&ctx, DS_LAYER_EXTRA, 5004).overlay);
        EXPECT(!at(&ctx, DS_LAYER_BASE, 5005).overlay);
    }
    { // a new press restarts the time
        ds_ctx_t       ctx    = {0};
        const uint32_t second = 1000 + SEK_DISPLAY_BL_OVERLAY_MS - 1;
        ds_backlight_pressed(&ctx, 1000);
        EXPECT(at(&ctx, DS_LAYER_EXTRA, second).overlay);
        ds_backlight_pressed(&ctx, second);
        EXPECT(at(&ctx, DS_LAYER_EXTRA, 1000 + SEK_DISPLAY_BL_OVERLAY_MS).overlay);
        EXPECT(at(&ctx, DS_LAYER_EXTRA, second + SEK_DISPLAY_BL_OVERLAY_MS - 1).overlay);
        EXPECT(!at(&ctx, DS_LAYER_EXTRA, second + SEK_DISPLAY_BL_OVERLAY_MS).overlay);
    }
    { // across the 32-bit timer wrap
        ds_ctx_t       ctx   = {0};
        const uint32_t press = 0xFFFFFFFFu - SEK_DISPLAY_BL_OVERLAY_MS / 2;
        ds_backlight_pressed(&ctx, press);
        EXPECT(at(&ctx, DS_LAYER_EXTRA, press + SEK_DISPLAY_BL_OVERLAY_MS - 1).overlay); // wrapped
        EXPECT(!at(&ctx, DS_LAYER_EXTRA, press + SEK_DISPLAY_BL_OVERLAY_MS).overlay);
    }
    { // mods: Caps Word's own Left Shift is hidden, the user's Shift is not
        ds_ctx_t        ctx = {0};
        display_state_t out;
        ds_raw_t        raw = {.mods = DS_MOD_LSFT, .caps_word = true, .caps_word_shifting = true};
        ds_resolve(&raw, &ctx, &out);
        EXPECT(!out.shift && out.word);
        raw.oneshot_mods = DS_MOD_LSFT; // a one-shot Shift
        ds_resolve(&raw, &ctx, &out);
        EXPECT(out.shift);
        raw = (ds_raw_t){.mods = DS_MOD_LSFT | DS_MOD_RSFT, .caps_word_shifting = true}; // Right Shift held
        ds_resolve(&raw, &ctx, &out);
        EXPECT(out.shift);
        raw = (ds_raw_t){.mods = DS_MOD_LSFT}; // held, no Caps Word
        ds_resolve(&raw, &ctx, &out);
        EXPECT(out.shift && !out.ctrl && !out.alt && !out.gui);
        raw = (ds_raw_t){.mods = DS_MOD_RCTL, .oneshot_mods = DS_MOD_LALT | DS_MOD_RGUI};
        ds_resolve(&raw, &ctx, &out);
        EXPECT(!out.shift && out.ctrl && out.alt && out.gui);
    }
    { // a full stash on LOWER shows the language and layout from before LOWER
        ds_ctx_t        ctx    = {0};
        display_state_t out;
        const ds_raw_t  before = {.layer = DS_LAYER_BASE, .secondary = true, .qwerty = true, .instant_qwerty = true};
        ds_snapshot_before_stash(&ctx, &before);
        const ds_raw_t lower = {.layer = DS_LAYER_LOWER, .stash_full = true}; // host switched to PRIMARY, instant QWERTY ended
        ds_resolve(&lower, &ctx, &out);
        EXPECT(out.layer == DS_LAYER_LOWER && out.secondary && out.qwerty && out.layout_auto);
        const ds_raw_t live = {.layer = DS_LAYER_LOWER}; // stash empty: live values
        ds_resolve(&live, &ctx, &out);
        EXPECT(!out.secondary && !out.qwerty && !out.layout_auto);
    }
    { // OS AUTO only with a known OS
        ds_ctx_t        ctx = {0};
        display_state_t out;
        ds_raw_t        raw = {.os = DS_OS_UNKNOWN, .os_autodetect = true};
        ds_resolve(&raw, &ctx, &out);
        EXPECT(!out.os_auto && out.os == DS_OS_UNKNOWN);
        raw.os = DS_OS_MAC;
        ds_resolve(&raw, &ctx, &out);
        EXPECT(out.os_auto);
        raw.os_autodetect = false;
        ds_resolve(&raw, &ctx, &out);
        EXPECT(!out.os_auto);
    }
    { // the language is muted off BASE and with macOS or an unknown OS
        static const struct {
            uint8_t layer, os;
            bool    muted;
        } C[] = {
            {DS_LAYER_BASE, DS_OS_LINUX, false}, {DS_LAYER_BASE, DS_OS_WINDOWS, false}, {DS_LAYER_BASE, DS_OS_MAC, true}, {DS_LAYER_BASE, DS_OS_UNKNOWN, true}, {DS_LAYER_LOWER, DS_OS_LINUX, true}, {DS_LAYER_EXTRA, DS_OS_WINDOWS, true},
        };
        for (unsigned i = 0; i < sizeof C / sizeof C[0]; ++i) {
            ds_ctx_t        ctx = {0};
            display_state_t out;
            const ds_raw_t  raw = {.layer = C[i].layer, .os = C[i].os};
            ds_resolve(&raw, &ctx, &out);
            EXPECT(out.lang_muted == C[i].muted);
        }
    }
    { // the backlight level is clamped to DS_BACKLIGHT_MAX
        ds_ctx_t        ctx = {0};
        display_state_t out;
        const ds_raw_t  raw = {.backlight = DS_BACKLIGHT_MAX + 1};
        ds_resolve(&raw, &ctx, &out);
        EXPECT(out.backlight == DS_BACKLIGHT_MAX);
    }
    { // locks and level pass through
        ds_ctx_t        ctx = {0};
        display_state_t out;
        const ds_raw_t  raw = {.caps_lock = true, .scroll_lock = true, .backlight = 7};
        ds_resolve(&raw, &ctx, &out);
        EXPECT(out.caps && !out.num && out.scrl && !out.word && out.backlight == 7);
    }
    if (failures) return 1;
    printf("model OK: %d cases\n", cases);
    return 0;
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "palette") == 0) return palette();
    if (argc == 2 && strcmp(argv[1], "fields") == 0) return fields();
    if (argc == 2 && strcmp(argv[1], "list") == 0) return list();
    if (argc == 2 && strcmp(argv[1], "render") == 0) return render();
    if (argc == 2 && strcmp(argv[1], "model") == 0) return model();
    fprintf(stderr, "usage: %s palette|fields|list|render|model\n", argv[0]);
    return 2;
}
