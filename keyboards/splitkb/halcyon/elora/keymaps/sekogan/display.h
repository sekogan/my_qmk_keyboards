// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// The Halcyon TFT module (ST7789, 135x240) on the master half. All functions are
// no-ops on the slave half.
void display_init(void);
void display_task(void);
void display_suspend(void);
void display_wakeup(void);

// keymap.c hooks for the display state (display_state.c).
void display_backlight_adjusted(void);    // a BL+/BL- press: shows the backlight popup
void display_snapshot_before_lower(void); // before LOWER stashes the language (no-op while the stash is full)
