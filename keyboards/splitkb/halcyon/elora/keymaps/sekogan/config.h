// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#undef TAPPING_TERM
#define TAPPING_TERM 200

#define TAPPING_TOGGLE 1 // a single tap of TT(NUMPAD) toggles it
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

// Delay between press and release in tap_code(); the board default is 0.
#define TAP_CODE_DELAY 10

// Core Tri Layer (TL_LOWR/TL_UPPR). quantum/tri_layer.c compiles these without layers.h,
// so they are numbers; keymap.c checks them against enum layers.
#define TRI_LAYER_LOWER_LAYER 3  /* LOWER */
#define TRI_LAYER_UPPER_LAYER 4  /* RAISE */
#define TRI_LAYER_ADJUST_LAYER 5 /* EXTRA */

// Halcyon TFT module: ST7789 on SPID1 (SCK/MOSI/MISO pins are in the board config.h).
#define LCD_CS_PIN GP13
#define LCD_DC_PIN GP16
#define LCD_RST_PIN GP26
#define LCD_SPI_DIVISOR 0
#define LCD_SPI_MODE 3
#define LCD_WIDTH 135
#define LCD_HEIGHT 240
#define LCD_OFFSET_X 52
#define LCD_OFFSET_Y 40
#define ST7789_NO_AUTOMATIC_VIEWPORT_OFFSETS
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 0 // the panel timeout is in display.c, together with the backlight
#define SEK_DISPLAY_TIMEOUT 120000 // ms without key activity before the panel and backlight go off
#define SEK_DISPLAY_BL_OVERLAY_MS 2500 // the backlight popup stays this long after the last BL+/BL-; hidden earlier when EXTRA is left

// Backlight: GP27 = PWM slice 5, channel B.
#define BACKLIGHT_PIN GP27
#define BACKLIGHT_PWM_DRIVER PWMD5
#define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_B
#define BACKLIGHT_LEVELS 10
