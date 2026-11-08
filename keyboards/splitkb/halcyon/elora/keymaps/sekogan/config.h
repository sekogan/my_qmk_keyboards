// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#undef TAPPING_TERM
#define TAPPING_TERM 200

// Delay between press and release in tap_code(); the board default is 0.
#define TAP_CODE_DELAY 10

// Core Tri Layer (TL_LOWR/TL_UPPR). quantum/tri_layer.c compiles these without layers.h,
// so they are numbers; keymap.c checks them against enum layers.
#define TRI_LAYER_LOWER_LAYER 2  /* LOWER */
#define TRI_LAYER_UPPER_LAYER 4  /* RAISE */
#define TRI_LAYER_ADJUST_LAYER 5 /* EXTRA */
