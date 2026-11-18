// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// The display's view model. ds_resolve() and the other ds_* functions are pure and also compile
// on the host (SEK_DISPLAY_HOST, scripts/check-elora-display.sh); the rest reads the firmware.

#include "display_state.h"

#ifndef SEK_DISPLAY_POPUP_MS
#    error "SEK_DISPLAY_POPUP_MS (config.h) is not defined"
#endif

void ds_popup_pressed(ds_ctx_t *ctx, uint8_t popup, uint32_t now) {
    ctx->popup    = popup;
    ctx->popup_at = now;
}

void ds_snapshot_before_lower(ds_ctx_t *ctx, const ds_raw_t *raw) {
    ctx->pre_lower_secondary   = raw->secondary;
    ctx->pre_lower_qwerty      = raw->qwerty;
    ctx->pre_lower_layout_auto = raw->instant_qwerty;
}

void ds_resolve(const ds_raw_t *raw, ds_ctx_t *ctx, display_state_t *out) {
    // A popup hides SEK_DISPLAY_POPUP_MS after the press that armed it, or at once when EXTRA is left.
    // Disarmed, not masked: re-entering EXTRA does not bring it back.
    if (ctx->popup != DS_POPUP_NONE && (raw->layer != DS_LAYER_EXTRA || (uint32_t)(raw->now - ctx->popup_at) >= SEK_DISPLAY_POPUP_MS)) ctx->popup = DS_POPUP_NONE;

    // Caps Word's own Left Shift is not the user's; a one-shot Shift still is.
    uint8_t mods = raw->mods;
    if (raw->caps_word_shifting) mods &= (uint8_t)~DS_MOD_LSFT;
    mods |= raw->oneshot_mods;

    // LOWER stashes the language (and the host may switch back to primary): while the stash is
    // full, show the language and layout from before LOWER.
    const bool stashed = raw->stash_full;

    *out = (display_state_t){
        .layer       = raw->layer,
        .os          = raw->os,
        .backlight   = raw->backlight <= DS_BACKLIGHT_MAX ? raw->backlight : DS_BACKLIGHT_MAX,
        .os_auto     = raw->os_autodetect && raw->os != DS_OS_UNKNOWN,
        .qwerty      = stashed ? ctx->pre_lower_qwerty : raw->qwerty,
        .layout_auto = stashed ? ctx->pre_lower_layout_auto : raw->instant_qwerty,
        .secondary   = stashed ? ctx->pre_lower_secondary : raw->secondary,
        // The language only matters on BASE with Linux or Windows: elsewhere it is shown muted.
        .lang_muted  = raw->layer != DS_LAYER_BASE || raw->os == DS_OS_MAC || raw->os == DS_OS_UNKNOWN,
        .shift       = (mods & (DS_MOD_LSFT | DS_MOD_RSFT)) != 0,
        .ctrl        = (mods & (DS_MOD_LCTL | DS_MOD_RCTL)) != 0,
        .alt         = (mods & (DS_MOD_LALT | DS_MOD_RALT)) != 0,
        .gui         = (mods & (DS_MOD_LGUI | DS_MOD_RGUI)) != 0,
        .caps        = raw->caps_lock,
        .word        = raw->caps_word,
        .num         = raw->num_lock,
        .scrl        = raw->scroll_lock,
        .popup       = ctx->popup,
    };
}

#ifndef SEK_DISPLAY_HOST

#    include QMK_KEYBOARD_H
#    include "display.h"
#    include "layers.h"
#    include "features/caps_word.h"
#    include "features/instant_qwerty.h"
#    include "features/language_stash.h"
#    include "features/language.h"
#    include "features/os.h"

_Static_assert(DS_MOD_LCTL == MOD_BIT(KC_LCTL) && DS_MOD_LSFT == MOD_BIT(KC_LSFT) && DS_MOD_LALT == MOD_BIT(KC_LALT) && DS_MOD_LGUI == MOD_BIT(KC_LGUI), "DS_MOD_* must match QMK");
_Static_assert(DS_BACKLIGHT_MAX == BACKLIGHT_LEVELS, "DS_BACKLIGHT_MAX must match BACKLIGHT_LEVELS");
_Static_assert(DS_MOD_RCTL == MOD_BIT(KC_RCTL) && DS_MOD_RSFT == MOD_BIT(KC_RSFT) && DS_MOD_RALT == MOD_BIT(KC_RALT) && DS_MOD_RGUI == MOD_BIT(KC_RGUI), "DS_MOD_* must match QMK");

static ds_ctx_t ctx;

static uint8_t ds_layer(uint8_t layer) {
    switch (layer) {
        case LOWER:
            return DS_LAYER_LOWER;
        case RAISE:
            return DS_LAYER_RAISE;
        case NUMPAD:
            return DS_LAYER_NUMPAD;
        case EXTRA:
            return DS_LAYER_EXTRA;
        default: // QWERTY, COLEMAK
            return DS_LAYER_BASE;
    }
}

static uint8_t ds_os(os_t os) {
    switch (os) {
        case LINUX_OS:
            return DS_OS_LINUX;
        case WINDOWS_OS:
            return DS_OS_WINDOWS;
        case MAC_OS:
            return DS_OS_MAC;
        default:
            return DS_OS_UNKNOWN;
    }
}

static void read_raw(ds_raw_t *raw) {
    const led_t leds = host_keyboard_led_state();
    *raw             = (ds_raw_t){
                    .layer              = ds_layer(get_highest_layer(layer_state)),
                    .os                 = ds_os(get_os()),
                    .os_autodetect      = is_os_autodetection_enabled(),
                    .qwerty             = get_highest_layer(default_layer_state) == QWERTY,
                    .instant_qwerty     = is_instant_qwerty_activated(),
                    .secondary          = get_language() == SECONDARY_LANGUAGE,
                    .stash_full         = !is_language_stash_empty(),
                    .mods               = get_mods(),
                    .oneshot_mods       = get_oneshot_mods(),
                    .caps_word_shifting = is_caps_word_both_shifts_shifting(),
                    .caps_word          = is_caps_word_both_shifts_on(),
                    .caps_lock          = leds.caps_lock,
                    .num_lock           = leds.num_lock,
                    .scroll_lock        = leds.scroll_lock,
                    .backlight          = get_backlight_level(),
    };
}

void display_state_read(display_state_t *out) {
    ds_raw_t raw;
    read_raw(&raw);
    if (ctx.popup != DS_POPUP_NONE) raw.now = timer_read32(); // only the popup expiry needs the time
    ds_resolve(&raw, &ctx, out);
}

void display_popup_pressed(uint8_t popup) {
    ds_popup_pressed(&ctx, popup, timer_read32());
}

void display_snapshot_before_lower(void) {
    if (!is_language_stash_empty()) return; // LOWER again over a full stash: keep the first snapshot
    ds_raw_t raw;
    read_raw(&raw);
    ds_snapshot_before_lower(&ctx, &raw);
}

#endif // SEK_DISPLAY_HOST
