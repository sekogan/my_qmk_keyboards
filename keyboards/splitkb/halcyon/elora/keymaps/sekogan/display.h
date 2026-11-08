// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// The Halcyon TFT module (ST7789, 135x240) on the master half. All functions are
// no-ops on the slave half.
void display_init(void);
void display_task(void);
void display_suspend(void);
void display_wakeup(void);
