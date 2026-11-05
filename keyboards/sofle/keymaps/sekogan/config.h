// Copyright 2021-2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#undef TAPPING_TERM
#define TAPPING_TERM 200

#undef ENCODER_RESOLUTION
#define ENCODER_RESOLUTION 4

// Core Tri Layer (TL_LOWR/TL_UPPR). quantum/tri_layer.c compiles these without layers.h,
// so they are numbers; keymap.c checks them against enum layers.
#define TRI_LAYER_LOWER_LAYER 2  /* LOWER */
#define TRI_LAYER_UPPER_LAYER 4  /* RAISE */
#define TRI_LAYER_ADJUST_LAYER 5 /* EXTRA */

#ifdef OLED_ENABLE

#    define SPLIT_LAYER_STATE_ENABLE
#    define SPLIT_LED_STATE_ENABLE

#endif // OLED_ENABLE
