// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "qp.h"

// Temporary hardware preview of display mockups (display-sketches/hw-test/). The first
// step enters preview mode, which is sticky until a power cycle: the status screen stops
// drawing and the current picture stays on the panel.

// direction > 0: next picture, otherwise the previous one; both wrap around.
void display_preview_step(int8_t direction);
bool display_preview_active(void);
// Draws the current picture into surface if it changed since the last call; returns true
// if it drew.
bool display_preview_render(painter_device_t surface);
