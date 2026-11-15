// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// display_gfx.h with Quantum Painter: drawing goes to the rgb565 surface, gfx_flush() sends its
// dirty area to the panel.

#include <stdbool.h>
#include "qp.h"

// Loads the fonts and icons. false if any of them failed to load.
bool gfx_qp_init(painter_device_t surface, painter_device_t lcd);
