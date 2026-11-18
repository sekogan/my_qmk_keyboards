// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// The skin renderer over display_gfx.h. No QMK dependencies: it also runs on the host.

#include <stdint.h>
#include "display_state.h"

// Clear to black, then draw the main screen or the popup. The caller flushes: every pixel is
// drawn, so one gfx_flush() sends the whole screen.
void skin_draw_all(const display_state_t *s);
// Redraw the groups that differ between old (on the screen) and s, flushing after each.
// The fields are in range (ds_resolve()).
void skin_draw_changes(const display_state_t *old, const display_state_t *s);

#ifdef SEK_DISPLAY_HOST
// Host check only: the versions the version popup draws (the firmware draws the build's
// SEK_KEYMAP_VERSION and SEK_QMK_VERSION). Digits and '.', 1..SKIN_VER_MAX_GLYPHS glyphs.
void skin_set_versions(const char *keymap, const char *qmk);
#endif
