// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// The display's view model: what the screen shows, with every display rule already applied.
// Plain C (no QMK headers), so the renderer and ds_resolve() also compile on the host
// (scripts/check-elora-display.sh).

#include <stdbool.h>
#include <stdint.h>

typedef enum { DS_LAYER_BASE, DS_LAYER_LOWER, DS_LAYER_RAISE, DS_LAYER_NUMPAD, DS_LAYER_EXTRA } ds_layer_t;
typedef enum { DS_OS_LINUX, DS_OS_WINDOWS, DS_OS_MAC, DS_OS_UNKNOWN } ds_os_t;

#define DS_BACKLIGHT_MAX 10 // the backlight level is 0..DS_BACKLIGHT_MAX (BACKLIGHT_LEVELS)

typedef struct {
    uint8_t layer, os, backlight; // ds_layer_t, ds_os_t, backlight 0..DS_BACKLIGHT_MAX
    bool    os_auto;              // autodetection on AND os != unknown
    bool    qwerty, layout_auto;  // actual layout; instant QWERTY active
    bool    secondary;            // language (pre-LOWER value while the stash is full)
    bool    lang_muted;           // the language does not apply: not BASE, or macOS / unknown OS
    bool    shift, ctrl, alt, gui; // held or one-shot, minus Caps Word's synthetic LSFT
    bool    caps, word, num, scrl;
    bool    overlay; // full-screen backlight popup visible
} display_state_t;

// 8-bit mod bits as in QMK (MOD_BIT(KC_LCTL) ...); display_state.c checks them against QMK.
#define DS_MOD_LCTL 0x01
#define DS_MOD_LSFT 0x02
#define DS_MOD_LALT 0x04
#define DS_MOD_LGUI 0x08
#define DS_MOD_RCTL 0x10
#define DS_MOD_RSFT 0x20
#define DS_MOD_RALT 0x40
#define DS_MOD_RGUI 0x80

// The firmware getters, read once per display_task().
typedef struct {
    uint8_t  layer;          // ds_layer_t of the highest active layer
    uint8_t  os;             // ds_os_t of get_os()
    bool     os_autodetect;  // is_os_autodetection_enabled()
    bool     qwerty;         // default layer is QWERTY
    bool     instant_qwerty; // is_instant_qwerty_activated()
    bool     secondary;      // get_language() == SECONDARY_LANGUAGE
    bool     stash_full;     // !is_language_stash_empty()
    uint8_t  mods;           // get_mods()
    uint8_t  oneshot_mods;   // get_oneshot_mods()
    bool     caps_word_shifting;
    bool     caps_word;
    bool     caps_lock, num_lock, scroll_lock;
    uint8_t  backlight; // get_backlight_level()
    uint32_t now;       // timer_read32(), read only while the popup is armed (0 otherwise)
} ds_raw_t;

// What the view model remembers between reads.
typedef struct {
    bool     overlay_armed;
    uint32_t bl_pressed_at;
    bool     pre_stash_secondary, pre_stash_qwerty, pre_stash_layout_auto;
} ds_ctx_t;

// Pure: the view model of raw. Disarms an armed popup when the highest layer is not EXTRA or
// SEK_DISPLAY_BL_OVERLAY_MS have passed since the last BL+/BL- press (raw->now).
void ds_resolve(const ds_raw_t *raw, ds_ctx_t *ctx, display_state_t *out);
// Pure: a BL+/BL- press at time now.
void ds_backlight_pressed(ds_ctx_t *ctx, uint32_t now);
// Pure: remember language and layout before LOWER or NUMPAD stashes the language.
void ds_snapshot_before_stash(ds_ctx_t *ctx, const ds_raw_t *raw);

// Firmware only: the view model from the live getters.
void display_state_read(display_state_t *out);
