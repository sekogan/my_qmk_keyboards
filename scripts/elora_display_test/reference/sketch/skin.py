"""The skin: i12.2 top, i13c2 sections, L24 lock labels, centered popup.

Tracked copy of the sketch script of the design the Elora display firmware
reproduces pixel for pixel. It is the oracle of scripts/check-elora-display.sh and the source of
the colors, icons and centering used by scripts/elora-display-assets.py. Changes from the
original, nothing else: this docstring, the sketchlib import (the trimmed copy next to this
file), the T28 font path (the copy in the keymap's graphics/) and no run() at the end.
Added after the copy: the version popup (version_popup(), the sixth test state "version").
"""
import math
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sketchlib import *  # noqa: E402,F403


def hx(s):
    return tuple(int(s[i:i + 2], 16) for i in (1, 3, 5))


# ---------------------------------------------------------------- palette (brief §2)
VFD = hx("#66FAE6")
FL_AMBER = hx("#FFA000")
ALERT = hx("#FF3018")
PANEL = hx("#CCCCCC")
MUTED = hx("#6E7380")
MUTED_AMBER = hx("#966614")
BASE_FILL = hx("#1C1E22")
BASE_TXT = hx("#8A8F98")
HAIR = hx("#145048")
PIN_C = hx("#1E8C80")
PIN_R = hx("#A02838")
TALLY = {"LOWER": hx("#4696FF"), "RAISE": hx("#FF6E3C"),
         "NUMPAD": hx("#FFCD28"), "EXTRA": hx("#CD5FFF")}
LAMP_G = hx("#00CC57")
AMBER = hx("#F39E0F")
MOD_COL = {"shift": hx("#D9AB00"), "ctrl": hx("#FF9194"),
           "alt": hx("#3CC876"), "gui": hx("#8AB0FF")}

A, B = L24, L18
OFF_FILL, OFF_TXT = 0.22, 0.45
T28 = BDF(os.path.join(GRAPHICS_DIR, "ter-u28b.bdf"))  # Terminus bold, class A


# ---------------------------------------------------------------- icons (brief §4)
def _dist(px, py, a, b):
    (ax, ay), (bx, by) = a, b
    dx, dy = bx - ax, by - ay
    t = max(0.0, min(1.0, ((px - ax) * dx + (py - ay) * dy) / (dx * dx + dy * dy)))
    return math.hypot(px - ax - t * dx, py - ay - t * dy)


def draw_icon(d, x0, y0, segs, sh, dsh, r, col):
    """Copied from hw-test/round3_make.py."""
    moved = []
    for a, b in segs:
        diag = a[0] != b[0] and a[1] != b[1]
        ex = dsh if diag else 0.0
        moved.append(((a[0] + sh + ex, a[1] + sh), (b[0] + sh + ex, b[1] + sh)))
    for j in range(21):
        for i in range(21):
            if any(_dist(i + 0.5, j + 0.5, a, b) <= r + 1e-9 for a, b in moved):
                d.point((x0 + i, y0 + j), fill=col)


SEGS = {
    "shift": [((10, 2), (3, 9)), ((3, 9), (6, 9)), ((6, 9), (6, 17)), ((6, 17), (14, 17)),
              ((14, 17), (14, 9)), ((14, 9), (17, 9)), ((17, 9), (10, 2))],
    "ctrl": [((3, 13), (10, 6)), ((10, 6), (17, 13))],
    "alt": [((1, 5), (5, 5)), ((5, 5), (15, 15)), ((15, 15), (19, 15)), ((11, 5), (19, 5))],
    "cmd": [((6, 2), (6, 18)), ((14, 2), (14, 18)), ((2, 6), (18, 6)), ((2, 14), (18, 14)),
            ((2, 2), (6, 2)), ((2, 2), (2, 6)), ((14, 2), (18, 2)), ((18, 2), (18, 6)),
            ((2, 14), (2, 18)), ((2, 18), (6, 18)), ((18, 14), (18, 18)), ((14, 18), (18, 18))],
}


def icon(d, x0, y0, name, col):
    if name in SEGS:
        draw_icon(d, x0, y0, SEGS[name], 0.0, 0.0, 1.0, col)
    elif name == "diamond":
        for j in range(20):
            for i in range(20):
                if abs(i + 0.5 - 10) + abs(j + 0.5 - 10) <= 8.5:
                    d.point((x0 + i, y0 + j), fill=col)
    elif name == "win":
        band = set(range(2, 9)) | set(range(11, 18))
        for j in band:
            for i in band:
                d.point((x0 + i, y0 + j), fill=col)


def ink_bounds(s, font):
    m = font.mask(s)
    bb = m.getbbox()
    return bb[0], bb[2] - bb[0]  # left offset, ink width


def box(d, x, y, w, h, col):
    d.rectangle([x, y, x + w - 1, y + h - 1], fill=col)


def lamp(img, d, bx, by, bw, bh, tx, ty, label, col, lit, font=B):
    box(d, bx, by, bw, bh, col if lit else ghost(col, OFF_FILL))
    text(img, tx, ty, label, font, BLACK if lit else ghost(col, OFF_TXT))


def centered_cell_x(s, font, x0=4, w=127):
    """Cell x that centers the measured ink of s in x0..x0+w-1."""
    off, ink = ink_bounds(s, font)
    return x0 + (w - ink) // 2 - off


# ---------------------------------------------------------------- draw
OS_NAME = {"linux": "LINUX", "windows": "WINDOWS", "mac": "MACOS", "unknown": "UNKNOWN"}


def tally(img, d, layer, font, h, cy):
    if layer == "BASE":
        fill, tcol = BASE_FILL, BASE_TXT
    else:
        fill, tcol = TALLY[layer], BLACK
    box(d, 4, 4, 127, h, fill)
    off, ink = ink_bounds(layer, font)
    ink_x = 4 + (127 - ink) // 2
    text(img, ink_x - off, cy, layer, font, tcol)


def panel(img, d, st, ty, my, ly1, ly2):
    """Mods at tile y my; lock rows at bar y ly1/ly2 (labels at bar y + 1)."""
    os_ = st["os"]
    gui_icon = {"windows": "win", "mac": "cmd"}.get(os_, "diamond")
    for k, m in enumerate(("shift", "ctrl", "alt", "gui")):
        x = 4 + 33 * k
        lit = st["mods"][m]["held"] or st["mods"][m]["oneshot"]
        c = MOD_COL[m]
        box(d, x, my, 28, 28, c if lit else ghost(c, OFF_FILL))
        icon(d, x + 4, my + 4, gui_icon if m == "gui" else m,
             BLACK if lit else ghost(c, OFF_TXT))
    # 61x20 bars, L24 labels centered on ink, cell y = bar y - 3
    for bx, by, label, col, on in ((4, ly1, "CAPS", LAMP_G, st["caps"]),
                                   (70, ly1, "WORD", AMBER, st["caps_word"]),
                                   (4, ly2, "NUM", LAMP_G, st["num"]),
                                   (70, ly2, "SCRL", LAMP_G, st["scroll"])):
        lamp(img, d, bx, by, 61, 20, centered_cell_x(label, A, bx, 61), by - 3,
             label, col, on, A)


def sections(img, d, st, VA, os_cell, lay_cell, lang_cell, os_val, lay_val, lang_val):
    """Legend cells are B text-cell y; tag boxes at cell y + 2; values in font VA."""
    os_, layer = st["os"], st["layer"]
    unknown = os_ == "unknown"
    text(img, 3, os_cell, "OS", B, PANEL)
    lamp(img, d, 92, os_cell + 2, 39, 15, 93, os_cell, "AUTO", AMBER,
         st["os_detected"] and not unknown)
    text(img, 3, os_val, OS_NAME[os_], VA, ALERT if unknown else VFD)
    text(img, 3, lay_cell, "LAYOUT", B, PANEL)
    lamp(img, d, 92, lay_cell + 2, 39, 15, 93, lay_cell, "AUTO", AMBER, st["instant"])
    text(img, 3, lay_val, st["layout"].upper(), VA, VFD)
    text(img, 3, lang_cell, "LANGUAGE", B, PANEL)
    muted = layer != "BASE" or os_ in ("mac", "unknown")
    sec = st["lang"] == "secondary"
    if sec:
        col = MUTED_AMBER if muted else FL_AMBER
    else:
        col = MUTED if muted else VFD
    text(img, 3, lang_val, "SECONDARY" if sec else "PRIMARY", VA, col)


def meter(img, d, n, seg_y, scale_y):
    for k in range(10):
        c = LAMP_G if k < 6 else FL_AMBER if k < 8 else ALERT
        box(d, 4 + 13 * k, seg_y, 10, 16, c if k + 1 <= n else ghost(c, OFF_FILL))
    text(img, 3, scale_y, "0", B, PANEL)
    text(img, 62, scale_y, "5", B, PANEL)
    text(img, 113, scale_y, "10", B, PANEL)


# ---------------------------------------------------------------- version popup
# Two label/value pairs, centered vertically as one 92-row group (74 / 92 / 74): label caps,
# 6 rows, value caps, 16 rows, label caps, 6 rows, value caps. Every line centered on its ink.
# Ink rows: KEYMAP 74-87 (L24 caps are cell rows 6..19), value 94-111 (VAL28 caps are cell rows
# 4..21), QMK 128-141, value 148-165.
TEST_VERSIONS = ("1.5.1", "0.34.7")  # the keymap and QMK versions of the "version" test state


def version_popup(keymap_version, qmk_version):
    img, d = canvas()
    for s, font, col, y in (("KEYMAP", A, PANEL, 68), (keymap_version, T28, VFD, 90),
                            ("QMK", A, PANEL, 122), (qmk_version, T28, VFD, 144)):
        text(img, centered_cell_x(s, font), y, s, font, col)
    return img


STATE_ORDER = STATE_ORDER + ["version"]
STATES = dict(STATES, version=dict(STATES["idle"], layer="EXTRA", version_overlay=True,
                                   keymap_version=TEST_VERSIONS[0], qmk_version=TEST_VERSIONS[1]))


def draw(st):
    if st.get("version_overlay"):
        return version_popup(st["keymap_version"], st["qmk_version"])
    img, d = canvas()
    if st["backlight_overlay"]:
        # popup (68 / 104 / 68), every line centered on its measured ink
        rd = f"{st['backlight']}/10"
        text(img, centered_cell_x("DISPLAY", A), 62, "DISPLAY", A, PANEL)
        text(img, centered_cell_x("BACKLIGHT", A), 81, "BACKLIGHT", A, PANEL)
        text(img, centered_cell_x(rd, T28), 107, rd, T28, VFD)
        meter(img, d, st["backlight"], 139, 157)
        return img
    tally(img, d, st["layer"], A, 20, 1)
    panel(img, d, st, None, 29, 62, 87)
    sections(img, d, st, T28, 113, 156, 199, 128, 171, 214)
    return img

