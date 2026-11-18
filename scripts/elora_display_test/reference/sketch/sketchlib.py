"""Trimmed copy of scratch/display-sketches/lib/sketchlib.py: only what skin.py uses.

The functions and the test states are copied verbatim. Changes: fonts are read from the
keymap's graphics/ directory (GRAPHICS_DIR); the Spleen fonts, OTF, glyph(), CAPTIONS and the
sheet/run output helpers are left out.
"""
import os

from PIL import Image, ImageDraw

__all__ = [
    "GRAPHICS_DIR",
    "W", "H", "BLACK", "canvas",
    "BDF", "L18", "L24",
    "text", "ghost",
    "STATE_ORDER", "STATES", "MOD_NAMES",
    "Image", "ImageDraw",
]

# ------------------------------------------------------------------ paths
GRAPHICS_DIR = os.path.normpath(os.path.join(
    os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", "..",
    "keyboards/splitkb/halcyon/elora/keymaps/sekogan/graphics"))

W, H = 135, 240
BLACK = (0, 0, 0)


def canvas():
    """A fresh black native canvas and its ImageDraw."""
    img = Image.new("RGB", (W, H), BLACK)
    return img, ImageDraw.Draw(img)


# ------------------------------------------------------------------ fonts
class BDF:
    """Minimal BDF bitmap font reader producing 1-bit masks."""

    def __init__(self, path):
        self.glyphs = {}
        cur = bitmap = None
        for line in open(path, encoding="latin-1"):
            p = line.split()
            if not p:
                continue
            k = p[0]
            if k == "FONT_ASCENT":
                self.ascent = int(p[1])
            elif k == "PIXEL_SIZE":
                self.height = int(p[1])
            elif k == "ENCODING":
                cur = {"enc": int(p[1])}
            elif k == "DWIDTH":
                cur["dw"] = int(p[1])
            elif k == "BBX":
                cur["bbx"] = tuple(map(int, p[1:5]))
            elif k == "BITMAP":
                bitmap = []
            elif k == "ENDCHAR":
                cur["bits"] = bitmap
                self.glyphs[cur["enc"]] = cur
                bitmap = None
            elif bitmap is not None:
                bitmap.append(int(k, 16))

    def width(self, s):
        return sum(self.glyphs.get(ord(c), self.glyphs[32])["dw"] for c in s)

    def mask(self, s):
        w = self.width(s)
        m = Image.new("1", (max(w, 1), self.height), 0)
        px = m.load()
        x = 0
        for c in s:
            g = self.glyphs.get(ord(c), self.glyphs[32])
            bw, bh, ox, oy = g["bbx"]
            nbytes = (bw + 7) // 8
            top = self.ascent - (oy + bh)
            for r, row in enumerate(g["bits"]):
                for col in range(bw):
                    if row & (1 << (nbytes * 8 - 1 - col)):
                        xx, yy = x + ox + col, top + r
                        if 0 <= xx < w and 0 <= yy < self.height:
                            px[xx, yy] = 1
            x += g["dw"]
        return m


L24 = BDF(os.path.join(GRAPHICS_DIR, "leggie-24b.bdf"))  # 12x24 bold
L18 = BDF(os.path.join(GRAPHICS_DIR, "leggie-18b.bdf"))  # 9x18 bold


# ------------------------------------------------------------------ drawing
def text(img, x, y, s, font, col):
    """Paint string s with its cell's top-left at (x, y). Returns the end x."""
    m = font.mask(s)
    img.paste(Image.new("RGB", m.size, col), (x, y), m)
    return x + m.size[0]


def ghost(c, f=0.16):
    """Unlit version of a color: same hue, scaled towards black."""
    return tuple(int(v * f) for v in c)


# ------------------------------------------------------------------ states
MOD_NAMES = ("shift", "ctrl", "alt", "gui")


def _mods(held=(), oneshot=()):
    return {m: {"held": m in held, "oneshot": m in oneshot} for m in MOD_NAMES}


_BASE_STATE = dict(
    layer="BASE", os="linux", os_detected=True, layout="colemak", instant=False,
    lang="primary", num=True, caps=False, scroll=False, caps_word=False, backlight=6,
    backlight_overlay=False,
)


def _state(mods=None, **kw):
    st = dict(_BASE_STATE, mods=mods or _mods())
    assert set(kw) <= set(st), f"unknown state keys: {set(kw) - set(st)}"
    st.update(kw)
    return st


STATE_ORDER = ["idle", "lower-secondary", "extra-no-os", "numpad-shortcut", "backlight-adjust"]
STATES = {
    "idle": _state(),
    "lower-secondary": _state(layer="LOWER", layout="qwerty", instant=True, lang="secondary",
                              os="windows", os_detected=False, mods=_mods(oneshot=("shift",)),
                              caps=True),
    "extra-no-os": _state(layer="EXTRA", os="unknown", os_detected=False, caps_word=True,
                          backlight=9),
    "numpad-shortcut": _state(layer="NUMPAD", layout="qwerty", os="mac",
                              mods=_mods(held=("ctrl", "alt", "gui")), num=False, scroll=True),
    "backlight-adjust": _state(layer="EXTRA", caps=True, caps_word=True, backlight=10,
                               backlight_overlay=True),
}
