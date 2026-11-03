// Copyright 2021-2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#undef TAPPING_TERM
#define TAPPING_TERM 200

#undef ENCODER_RESOLUTION
#define ENCODER_RESOLUTION 4

#ifdef OLED_ENABLE

#    define SPLIT_LAYER_STATE_ENABLE
#    define SPLIT_LED_STATE_ENABLE

#endif // OLED_ENABLE
