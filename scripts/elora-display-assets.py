#!/usr/bin/env python3
"""Elora display assets for the skin design (run by scripts/elora-display-assets.sh).

Everything is derived from the tracked sketch copy in scripts/elora_display_test/reference/sketch/
(skin.py + the trimmed sketchlib.py), the BDF fonts in the keymap's graphics/ and the box geometry
of display_skin_layout.h:

  graphics/font_{val28,l24,l18}_caps.png   QFF font images for `qmk painter-convert-font-image`
  graphics/icon_<name>.png                 20x20 1-bit mod icons for `qmk painter-convert-graphics`
  display_palette.h                        the colour table: HSV888 for QP, RGB565 for the host check
  display_skin_metrics.h                   the measured text-cell x tables (centred on ink)

The font images are 1 bpp, cell = BDF DWIDTH, cropped to the rows the drawn glyphs ink (A-Z, 0-9
and '/'). Nothing is scaled. Every output is checked here (see the asserts) and is byte-identical
between runs.
"""
import os
import re
import sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
KM = os.path.join(ROOT, "keyboards/splitkb/halcyon/elora/keymaps/sekogan")
GFX = os.path.join(KM, "graphics")
sys.path.insert(0, os.path.join(ROOT, "scripts/elora_display_test/reference/sketch"))
sys.path.insert(0, os.path.join(ROOT, "scripts/elora_display_test"))

import skin as S  # noqa: E402
from PIL import Image, ImageDraw  # noqa: E402
from skin_names import ICONS, LAYERS  # noqa: E402


def layout_defines():
    """The numeric SKIN_* geometry of display_skin_layout.h (the firmware's single source)."""
    with open(os.path.join(KM, "display_skin_layout.h")) as f:
        return {m[1]: int(m[2]) for m in re.finditer(r"^#define (SKIN_\w+) (\d+)\b", f.read(), re.M)}


L = layout_defines()

# ---------------------------------------------------------------- strings the renderer draws
LOCKS = [("CAPS", L["SKIN_LOCK_X1"]), ("WORD", L["SKIN_LOCK_X2"]),   # label, bar x
         ("NUM", L["SKIN_LOCK_X1"]), ("SCRL", L["SKIN_LOCK_X2"])]
READOUTS = [f"{n}/10" for n in range(11)]
STRINGS = {
    "val28": ["LINUX", "WINDOWS", "MACOS", "UNKNOWN", "COLEMAK", "QWERTY", "PRIMARY", "SECONDARY"]
    + READOUTS,
    "l24": LAYERS + [label for label, _ in LOCKS] + ["DISPLAY", "BACKLIGHT"],
    "l18": ["OS", "AUTO", "LAYOUT", "LANGUAGE", "0", "5", "10"],
}
# The glyphs each font may draw. L18 never draws '/' (its '/' inks rows 3..16, outside the crop).
CAPS_DIGITS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
ALLOWED = {"val28": CAPS_DIGITS + "/", "l24": CAPS_DIGITS + "/", "l18": CAPS_DIGITS}
for name, strings in STRINGS.items():
    for s in strings:
        assert set(s) <= set(ALLOWED[name]), f"{name} string {s!r} has a glyph outside {ALLOWED[name]}"

# ---------------------------------------------------------------- fonts
# name -> (sketch font, cropped rows of the BDF cell, inclusive). Firmware text y = sketch y + top.
FONTS = {
    "val28": (S.T28, 4, 23),  # Terminus 14x28 bold (the sketch's T28); the Q tail ends on row 23
    "l24": (S.L24, 4, 21),    # Leggie 12x24 bold; '/' spans 4..21
    "l18": (S.L18, 4, 14),    # Leggie 9x18 bold
}
GLYPHS = [chr(c) for c in range(0x20, 0x7F)]
MARKER = (255, 0, 255)
WHITE = (255, 255, 255)


def inked_rows(font, chars):
    rows = set()
    for c in chars:
        m = font.mask(c)
        for y in range(m.height):
            if any(m.getpixel((x, y)) for x in range(m.width)):
                rows.add(y)
    return min(rows), max(rows)


def font_strip(font, top, bottom):
    advs = {font.glyphs.get(ord(c), font.glyphs[32])["dw"] for c in GLYPHS}
    assert len(advs) == 1, "not a monospace font"
    adv = advs.pop()
    h = bottom - top + 1
    img = Image.new("RGB", (len(GLYPHS) * adv, 1 + h), (0, 0, 0))
    px = img.load()
    for i, c in enumerate(GLYPHS):
        px[i * adv, 0] = MARKER
        m = font.mask(c)
        assert m.size == (adv, font.height)
        for y in range(top, bottom + 1):
            for x in range(adv):
                if m.getpixel((x, y)):
                    px[i * adv + x, 1 + y - top] = WHITE
    return img, adv


def check_strip(img, adv, font, top, bottom, strings):
    px = img.load()
    assert img.width == len(GLYPHS) * adv
    assert sum(1 for x in range(img.width) if px[x, 0] == MARKER) == len(GLYPHS)
    for s in strings:
        want = font.mask(s)
        # nothing outside the cropped rows, and no glyph spills out of its cell
        for y in list(range(0, top)) + list(range(bottom + 1, font.height)):
            assert not any(want.getpixel((x, y)) for x in range(want.width)), f"{s!r} inks row {y}"
        for k, c in enumerate(s):
            gi = GLYPHS.index(c)
            for y in range(top, bottom + 1):
                for x in range(adv):
                    got = px[gi * adv + x, 1 + y - top] == WHITE
                    assert got == bool(want.getpixel((k * adv + x, y))), f"{s!r} differs at {k * adv + x},{y}"


# ---------------------------------------------------------------- icons
SKETCH_ICON = {"super": "diamond"}


def icon_image(name):
    big = Image.new("RGB", (21, 21), (0, 0, 0))
    S.icon(ImageDraw.Draw(big), 0, 0, SKETCH_ICON.get(name, name), WHITE)
    for i in range(21):
        assert big.getpixel((20, i)) == (0, 0, 0) and big.getpixel((i, 20)) == (0, 0, 0), \
            f"icon {name} lights column or row 20"
    out = Image.new("1", (20, 20), 0)
    for y in range(20):
        for x in range(20):
            if big.getpixel((x, y)) == WHITE:
                out.putpixel((x, y), 1)
    assert out.getbbox() is not None
    return out


# ---------------------------------------------------------------- colours
def hsv_to_rgb_nocie(h, s, v):
    """hsv_to_rgb_impl(hsv, false) from qmk_firmware/quantum/color.c."""
    if s == 0:
        return v, v, v
    region = h * 6 // 255
    remainder = ((h * 2 - region * 85) * 3) & 0xFF
    p = (v * (255 - s)) >> 8
    q = (v * (255 - ((s * remainder) >> 8))) >> 8
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8
    if region in (0, 6):
        return v, t, p
    if region == 1:
        return q, v, p
    if region == 2:
        return p, v, t
    if region == 3:
        return p, q, v
    if region == 4:
        return t, p, v
    return v, p, q


def rgb565(c):
    return ((c[0] >> 3) << 11) | ((c[1] >> 2) << 5) | (c[2] >> 3)


def find_hsv(target):
    """Deterministic HSV888 whose truncated RGB565 equals the sketch colour's."""
    want = rgb565(target)
    r5, g6, b5 = want >> 11, (want >> 5) & 0x3F, want & 0x1F
    # v is one of the output channels, p (= v(255-s)>>8) the smallest one
    vs = sorted({v for v in range(256) if v >> 3 in (r5, b5) or v >> 2 == g6})
    best = None
    for v in vs:
        for s in range(256):
            p = (v * (255 - s)) >> 8
            if not (p >> 3 in (r5, b5) or p >> 2 == g6):
                continue
            for h in range(256):
                rgb = hsv_to_rgb_nocie(h, s, v)
                if rgb565(rgb) != want:
                    continue
                key = (sum((a - b) ** 2 for a, b in zip(rgb, target)), h, s, v)
                if best is None or key < best[0]:
                    best = (key, (h, s, v))
    assert best is not None, f"no HSV888 reaches {target}"
    return best[1]


def palette():
    g = S.ghost
    cols = [
        ("BLACK", S.BLACK), ("VFD", S.VFD), ("FL_AMBER", S.FL_AMBER), ("ALERT", S.ALERT),
        ("PANEL", S.PANEL), ("MUTED", S.MUTED), ("MUTED_AMBER", S.MUTED_AMBER),
        ("BASE_FILL", S.BASE_FILL), ("BASE_TXT", S.BASE_TXT),
        ("TALLY_LOWER", S.TALLY["LOWER"]), ("TALLY_RAISE", S.TALLY["RAISE"]),
        ("TALLY_NUMPAD", S.TALLY["NUMPAD"]), ("TALLY_EXTRA", S.TALLY["EXTRA"]),
        ("LAMP_G", S.LAMP_G), ("AMBER", S.AMBER),
        ("SHIFT", S.MOD_COL["shift"]), ("CTRL", S.MOD_COL["ctrl"]),
        ("ALT", S.MOD_COL["alt"]), ("GUI", S.MOD_COL["gui"]),
    ]
    for name, c in [("LAMP_G", S.LAMP_G), ("AMBER", S.AMBER), ("SHIFT", S.MOD_COL["shift"]),
                    ("CTRL", S.MOD_COL["ctrl"]), ("ALT", S.MOD_COL["alt"]), ("GUI", S.MOD_COL["gui"])]:
        cols.append((name + "_OFF_FILL", g(c, S.OFF_FILL)))
        cols.append((name + "_OFF_TXT", g(c, S.OFF_TXT)))
    cols.append(("FL_AMBER_OFF_FILL", g(S.FL_AMBER, S.OFF_FILL)))
    cols.append(("ALERT_OFF_FILL", g(S.ALERT, S.OFF_FILL)))
    assert len(cols) == 33
    return [(name, c, find_hsv(c)) for name, c in cols]


# ---------------------------------------------------------------- headers
HEADER = """// Copyright 2026 Sergei Kogan
// SPDX-License-Identifier: GPL-2.0-or-later

// Generated by scripts/elora-display-assets.sh from the skin sketch
// (scripts/elora_display_test/reference/sketch/skin.py). Do not edit.

#pragma once

#include <stdint.h>
"""


def palette_header(pal):
    out = [HEADER]
    out.append("// Colours of the skin design. The panel shows (R>>3, G>>2, B>>3) of the sketch hex:")
    out.append("// QP converts skin_hsv[] with hsv_to_rgb_nocie() and truncates, which gives exactly skin_565[].")
    out.append("enum skin_color {")
    for name, c, _ in pal:
        out.append(f"    SKIN_{name},")
    out.append("    SKIN_NUM_COLORS,")
    out.append("};")
    out.append("")
    out.append("__attribute__((unused)) static const uint8_t skin_hsv[SKIN_NUM_COLORS][3] = {")
    w = max(len(n) for n, _, _ in pal)
    for name, c, (h, s, v) in pal:
        hexs = "#%02X%02X%02X" % c
        out.append(f"    [SKIN_{name}]{' ' * (w - len(name))} = {{{h:3d}, {s:3d}, {v:3d}}}, // {hexs} -> 0x{rgb565(c):04X}")
    out.append("};")
    out.append("")
    out.append("__attribute__((unused)) static const uint16_t skin_565[SKIN_NUM_COLORS] = {")
    for name, c, _ in pal:
        out.append(f"    [SKIN_{name}]{' ' * (w - len(name))} = 0x{rgb565(c):04X},")
    out.append("};")
    return "\n".join(out) + "\n"


def metrics_header():
    A, T28 = S.A, S.T28
    x0, cw, lock_w = L["SKIN_X0"], L["SKIN_CW"], L["SKIN_LOCK_W"]
    tally = [S.centred_cell_x(layer, A, x0, cw) for layer in LAYERS]
    locks = [S.centred_cell_x(label, A, bx, lock_w) for label, bx in LOCKS]
    readout = [S.centred_cell_x(rd, T28, x0, cw) for rd in READOUTS]

    def table(comment, prefix, names, values):
        out = ["", comment]
        out += [f"#define {prefix}_{n} {v}" for n, v in zip(names, values)]
        out.append(f"__attribute__((unused)) static const uint8_t {prefix}[{len(values)}] = {{"
                   + ", ".join(f"{prefix}_{n}" for n in names) + "};")
        return out

    out = [HEADER]
    out.append("// Text-cell x of the centred labels, measured on the ink with the sketch's centred_cell_x():")
    out.append("// cell x = x0 + (w - ink width) / 2 - ink left offset.")
    out += table(f"// The tally (box {x0},{L['SKIN_TALLY_Y']},{cw},{L['SKIN_TALLY_H']}), by ds_layer_t.",
                 "SKIN_TALLY_X", LAYERS, tally)
    out += table(f"// The lock labels ({lock_w}-wide bars).", "SKIN_LOCK_X", [label for label, _ in LOCKS], locks)
    out.append("")
    out.append(f"// The popup readout \"{{n}}/10\" (centred in {x0}..{x0 + cw - 1}), by n = 0..10.")
    out.append(f"__attribute__((unused)) static const uint8_t SKIN_READOUT_X[{len(readout)}] = {{" + ", ".join(map(str, readout)) + "};")
    return "\n".join(out) + "\n"


def write(path, data):
    mode = "wb" if isinstance(data, bytes) else "w"
    with open(path, mode) as f:
        f.write(data)


def main():
    for name, (font, top, bottom) in FONTS.items():
        rows = inked_rows(font, ALLOWED[name])
        assert rows == (top, bottom), f"{name}: drawn glyphs ink rows {rows}, crop is {(top, bottom)}"
        img, adv = font_strip(font, top, bottom)
        check_strip(img, adv, font, top, bottom, STRINGS[name])
        img.save(os.path.join(GFX, f"font_{name}_caps.png"))
    for name in ICONS:
        icon_image(name).save(os.path.join(GFX, f"icon_{name}.png"))
    write(os.path.join(KM, "display_palette.h"), palette_header(palette()))
    write(os.path.join(KM, "display_skin_metrics.h"), metrics_header())


if __name__ == "__main__":
    main()
