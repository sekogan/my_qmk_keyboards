#!/usr/bin/env python3
"""Pixel-exact check of the Elora display renderer (run by scripts/check-elora-display.sh).

    compare.py <render_states binary> <diff output dir>

1. The tracked sketch copy re-renders the five natives byte-identically (it is the oracle).
2. The firmware palette: hsv_to_rgb_nocie(skin_hsv[]) truncates to skin_565[] (render_states).
3. The five design test states (render_states list), drawn by the firmware renderer and
   rasterized here from the committed font images and icon PNGs (the exact QFF/QGF inputs) and
   skin_565[], equal the natives truncated to RGB565 (r>>3, g>>2, b>>3).
   Diffs: <diff dir>/display-diff-<state>.png.
4. An extended matrix (every layer, OS, language, layout, instant QWERTY and OS AUTO, mods and
   locks varied; the popup at every level) against the sketch script.
5. Incremental: skin_draw_changes(a, b) over the frame of a == the frame of b, for all
   ordered pairs of the test states and a sample of the matrix. The frames of a and b are the
   references steps 3 and 4 checked skin_draw_all() against (the natives, the sketch).
6. ds_resolve() cases (render_states model).
"""
import os
import random
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, "..", ".."))
GFX = os.path.join(ROOT, "keyboards/splitkb/halcyon/elora/keymaps/sekogan/graphics")
NATIVES = os.path.join(HERE, "reference", "natives")
sys.path.insert(0, os.path.join(HERE, "reference", "sketch"))
sys.path.insert(0, HERE)

import skin as S  # noqa: E402
from PIL import Image  # noqa: E402
from skin_names import ICONS, LAYERS  # noqa: E402

W, H = 135, 240
OSES = ["linux", "windows", "mac", "unknown"]                   # ds_os_t
FONTS = ["font_val28_caps", "font_l24_caps", "font_l18_caps"]   # enum gfx_font

# Frames are RGB565 kept as RGB bytes with the dropped low bits cleared (r & 0xF8, g & 0xFC,
# b & 0xF8): equal bytes <=> equal RGB565.
MASK5 = [v & 0xF8 for v in range(256)]
MASK6 = [v & 0xFC for v in range(256)]
UNPAINTED = b"\x01\x01\x01"  # never a masked color


def rgb565_bytes(c):
    """RGB565 value -> the masked RGB bytes of a frame pixel."""
    return bytes(((c >> 11) << 3, ((c >> 5) & 0x3F) << 2, (c & 0x1F) << 3))


def image565(img):
    return img.convert("RGB").point(MASK5 + MASK6 + MASK5).tobytes()


# ---------------------------------------------------------------- rasterizer
class Font:
    def __init__(self, path):
        img = Image.open(path).convert("RGB")
        self.adv = img.width // 95
        assert self.adv * 95 == img.width
        self.h = img.height - 1
        px = img.load()
        self.cells = []
        for i in range(95):
            self.cells.append([[px[i * self.adv + x, 1 + y] == (255, 255, 255) for x in range(self.adv)]
                               for y in range(self.h)])


class Icon:
    def __init__(self, path):
        img = Image.open(path).convert("L")
        assert img.size == (20, 20)
        self.ink = [[img.getpixel((x, y)) > 127 for x in range(20)] for y in range(20)]


FONT_DATA = [Font(os.path.join(GFX, f + ".png")) for f in FONTS]
ICON_DATA = [Icon(os.path.join(GFX, f"icon_{i}.png")) for i in ICONS]


_rows = {}


def _cached(key, make):
    rows = _rows.get(key)
    if rows is None:
        rows = _rows[key] = make()
    return rows


def glyph_rows(font, f, ch, fg, bg):
    """The cell rows of one glyph in fg on bg, as frame bytes."""
    def make():
        return [b"".join(fg if ink else bg for ink in row) for row in font.cells[ord(ch) - 0x20]]
    return _cached(("g", f, ch, fg, bg), make)


def text_rows(f, s, fg, bg):
    font = FONT_DATA[f]
    return _cached(("t", f, s, fg, bg),
                   lambda: [b"".join(r) for r in zip(*(glyph_rows(font, f, ch, fg, bg) for ch in s))])


def icon_rows(i, fg, bg):
    return _cached(("i", i, fg, bg), lambda: [b"".join(fg if ink else bg for ink in row) for row in ICON_DATA[i].ink])


def rasterize(ops, pal, base=None):
    """ops: primitive lines of one frame, drawn over base (a frame) or nothing -> frame bytes."""
    canvas = bytearray(base) if base is not None else bytearray(UNPAINTED * (W * H))

    def box(x, y, w, h):
        assert 0 <= x and 0 <= y and x + w <= W and y + h <= H, f"out of the screen: {x},{y},{w},{h}"

    def blit(x, y, rows):
        for yy, row in enumerate(rows):
            i = ((y + yy) * W + x) * 3
            canvas[i:i + len(row)] = row

    for line in ops:
        p = line.split(" ", 6) if line.startswith("T ") else line.split()
        k = p[0]
        if k == "F":
            x, y, w, h, c = map(int, p[1:6])
            box(x, y, w, h)
            blit(x, y, [pal[c] * w] * h)
        elif k == "T":
            x, y, f, fg, bg = map(int, p[1:6])
            s = p[6]
            font = FONT_DATA[f]
            box(x, y, font.adv * len(s), font.h)
            blit(x, y, text_rows(f, s, pal[fg], pal[bg]))
        elif k == "I":
            x, y, i, fg, bg = map(int, p[1:6])
            box(x, y, 20, 20)
            blit(x, y, icon_rows(i, pal[fg], pal[bg]))
        elif k == "X":
            pass
        else:
            raise ValueError(f"unknown primitive: {line}")
    assert UNPAINTED[0] not in canvas[0::3], "the frame leaves pixels unpainted"
    return bytes(canvas)


def frames(text):
    out, cur, name = [], None, None
    for line in text.splitlines():
        if line.startswith("BEGIN"):
            cur, name = [], line[6:]
        elif line == "END":
            out.append((name, cur))
            cur = None
        else:
            cur.append(line)
    return out


def run(binary, mode, stdin=None):
    r = subprocess.run([binary, mode], input=stdin, capture_output=True, text=True)
    if r.returncode != 0:
        sys.stderr.write(r.stderr)
        raise SystemExit(f"FAIL: render_states {mode} exited with {r.returncode}")
    return r.stdout


# ---------------------------------------------------------------- states
def sketch_state(st):
    """display_state_t fields -> a sketchlib state dict."""
    mods = {m: {"held": bool(st[m]), "oneshot": False} for m in ("shift", "ctrl", "alt", "gui")}
    return dict(layer=LAYERS[st["layer"]], os=OSES[st["os"]], os_detected=bool(st["os_auto"]),
                layout="qwerty" if st["qwerty"] else "colemak", instant=bool(st["layout_auto"]),
                lang="secondary" if st["secondary"] else "primary", mods=mods, num=bool(st["num"]),
                caps=bool(st["caps"]), scroll=bool(st["scrl"]), caps_word=bool(st["word"]),
                backlight=st["backlight"], backlight_overlay=bool(st["overlay"]))


def lang_muted(layer, os_):
    """As ds_resolve(): the sketch derives it from the layer and the OS."""
    return int(LAYERS[layer] != "BASE" or OSES[os_] in ("mac", "unknown"))


def matrix(names):
    rnd = random.Random(14)
    states = []
    for layer in range(5):
        for os_ in range(4):
            for secondary in (0, 1):
                for qwerty in (0, 1):
                    for layout_auto in (0, 1):
                        for os_auto in (0, 1):
                            if os_auto and OSES[os_] == "unknown":
                                continue  # ds_resolve never gives AUTO with an unknown OS
                            st = dict(layer=layer, os=os_, backlight=rnd.randrange(11), os_auto=os_auto,
                                      qwerty=qwerty, layout_auto=layout_auto, secondary=secondary,
                                      overlay=0)
                            for f in ("shift", "ctrl", "alt", "gui", "caps", "word", "num", "scrl"):
                                st[f] = rnd.randrange(2)
                            st["lang_muted"] = lang_muted(layer, os_)
                            states.append(st)
    for n in range(11):  # the popup at every level; main-screen fields random (not drawn)
        st = {f: rnd.randrange(2) for f in names if f != "lang_muted"}
        st.update(layer=rnd.randrange(5), os=rnd.randrange(4), backlight=n, overlay=1)
        st["lang_muted"] = lang_muted(st["layer"], st["os"])
        states.append(st)
    return states


def diff_image(want, got, path, s=4):
    """expected | actual | differing pixels in red, nearest-neighbor 4x for viewing."""
    widen5 = [v | v >> 5 for v in range(256)]
    widen6 = [v | v >> 6 for v in range(256)]
    img = Image.new("RGB", (W * 3 + 8, H), (48, 48, 52))
    for x0, data in ((0, want), (W + 4, got)):
        img.paste(Image.frombytes("RGB", (W, H), data).point(widen5 + widen6 + widen5), (x0, 0))
    red = b"".join(b"\xff\x00\x00" if want[i:i + 3] != got[i:i + 3] else b"\x00\x00\x00"
                   for i in range(0, len(want), 3))
    img.paste(Image.frombytes("RGB", (W, H), red), (2 * W + 8, 0))
    os.makedirs(os.path.dirname(path), exist_ok=True)
    img.resize((img.width * s, img.height * s), Image.NEAREST).save(path)


def bad_pixels(want, got):
    return sum(1 for i in range(0, len(want), 3) if want[i:i + 3] != got[i:i + 3])


def main():
    binary, diff_dir = sys.argv[1], sys.argv[2]
    fail = False

    # 1. the sketch copy is faithful
    natives = {}
    for k in S.STATE_ORDER:
        native = Image.open(os.path.join(NATIVES, f"skin-{k}.png")).convert("RGB")
        if S.draw(S.STATES[k]).tobytes() != native.tobytes():
            print(f"FAIL: the sketch copy does not re-render the native {k}")
            fail = True
        natives[k] = image565(native)
    if fail:
        return 1
    print(f"sketch copy: re-renders the {len(natives)} natives byte-identically")

    # 2. palette
    pal = {}
    for line in run(binary, "palette").splitlines():
        _, i, c = line.split()
        pal[int(i)] = rgb565_bytes(int(c))
    print(f"palette: {len(pal)} colors, hsv_to_rgb_nocie(skin_hsv) == skin_565")

    names = run(binary, "fields").split()

    def fields(st):
        return " ".join(str(int(st[f])) for f in names)

    def render(lines):
        return frames(run(binary, "render", "".join(line + "\n" for line in lines)))

    # The reference frame of every state drawn below: the native or the sketch.
    ref = {}

    # 3. the design test states against the natives
    test_states = [dict(zip(names, map(int, line.split()))) for line in run(binary, "list").splitlines()]
    assert len(test_states) == len(S.STATE_ORDER)
    out = render(f"A {fields(st)}" for st in test_states)
    assert len(out) == len(test_states)
    for name, st, (_, ops) in zip(S.STATE_ORDER, test_states, out):
        ref[fields(st)] = natives[name]
        got = rasterize(ops, pal)
        if got != natives[name]:
            path = os.path.join(diff_dir, f"display-diff-{name}.png")
            diff_image(natives[name], got, path)
            print(f"FAIL: {name}: {bad_pixels(natives[name], got)} pixels differ from the native"
                  f" (see {os.path.relpath(path, ROOT)})")
            fail = True
        else:
            print(f"{name}: pixel-exact ({W}x{H})")

    # 4. the extended matrix against the sketch
    states = matrix(names)
    out = render(f"A {fields(st)}" for st in states)
    assert len(out) == len(states)
    bad_frames = 0
    for st, (_, ops) in zip(states, out):
        want = ref.setdefault(fields(st), image565(S.draw(sketch_state(st))))
        got = rasterize(ops, pal)
        if got != want:
            if not bad_frames:
                path = os.path.join(diff_dir, "display-diff-matrix.png")
                diff_image(want, got, path)
                print(f"FAIL: matrix state {st} differs (see {os.path.relpath(path, ROOT)})")
            bad_frames += 1
    if bad_frames:
        print(f"FAIL: matrix: {bad_frames} of {len(states)} frames differ")
        fail = True
    else:
        print(f"matrix: {len(states)} frames pixel-exact against the sketch")

    # 5. incremental redraws, over the reference frame of a
    pairs = [(a, b) for a in test_states for b in test_states if a is not b]
    pairs += list(zip(states, states[1:]))  # neighbors: one or two fields change at a time
    rnd = random.Random(1414)
    pairs += [(rnd.choice(states), rnd.choice(states)) for _ in range(60)]
    out = render(f"C {fields(a)} {fields(b)}" for a, b in pairs)
    assert len(out) == len(pairs)
    bad_pairs = 0
    for (a, b), (_, ops) in zip(pairs, out):
        if rasterize(ops, pal, ref[fields(a)]) != ref[fields(b)]:
            if not bad_pairs:
                print(f"FAIL: incremental {a} -> {b} differs from a full redraw")
            bad_pairs += 1
    if bad_pairs:
        print(f"FAIL: incremental: {bad_pairs} of {len(pairs)} pairs differ")
        fail = True
    else:
        print(f"incremental: {len(pairs)} pairs, skin_draw_changes(a, b) == skin_draw_all(b)")

    # 6. the state model
    print(run(binary, "model").strip())
    return 1 if fail else 0


if __name__ == "__main__":
    sys.exit(main())
