# Elora display: default skin design guidelines

The rules that every element of the default skin follows, and the reasons for them. Read this
before you change the skin, so the design stays consistent.

The panel is the Halcyon TFT: a 135x240 ST7789 driven by Quantum Painter, RGB565, portrait.

**Sources of truth.** If this document and the code disagree, the code wins. Fix the document.

| What | Where |
|---|---|
| Geometry: boxes, origins, gaps; layout static asserts | `display_skin_layout.h` (`SKIN_*`) |
| Centred text x tables (generated) | `display_skin_metrics.h` |
| Colours (generated) | `display_palette.h` (`enum skin_color`, `skin_565[]`) |
| Fonts and icons | `display_gfx.h` (`enum gfx_font`, `enum gfx_icon`), `graphics/` |
| Drawing | `display_skin.c` |
| What is shown, and when (muting, popup timing, mods) | `display_state.c`, `display_state.h` |
| The design oracle: colours, icons, the on/off rule, the reference drawing | `scripts/elora_display_test/reference/sketch/skin.py` |
| The reference images | `scripts/elora_display_test/reference/natives/skin-*.png` |

Paths without a directory are in `keyboards/splitkb/halcyon/elora/keymaps/sekogan/`.

## 1. Principles

1. **Two classes of text.** Every piece of text belongs to exactly one class. Always reason about
   the two classes separately.
   - **Class A, read at a glance:** values that change, such as the layer, the OS, the layout,
     the language and the backlight level. They can change at any moment, and the new state must
     register in a sideways glance while typing. Class A text is big, bold and bright.
   - **Class B, read on a closer look:** static labels such as the legends, AUTO, the lock names
     and the scale. They don't change; they only remind you what a thing means. Class B text is
     smaller and dimmer. For indicators, the fill carries the state, not the text.
2. **The background is black.** Colour carries meaning. Nothing is decorative.
3. **Fixed positions.** Every element keeps its coordinates in every state, so nothing jumps
   around. An off state is a dim version of the lit look, never empty space.
4. **Flat.** Use solid rectangles and bitmap text only. No frames, outlines, rules, gradients,
   glows, shadows or rounded corners.
5. **Pixel-exact.** The firmware reproduces the reference sketch exactly, and `make check`
   enforces it (section 10).

## 2. Type

Only three fonts, all bold bitmap fonts, used **only at their native size**. Never scale, pixel-double
or synthesise bold, and never use a non-bold weight or italics. Calibration on the real panel showed
that anything smaller than 12x24 can't be read at a glance and that anything smaller than 9x18
can't be read at all. Scaled bitmaps look blocky and uneven.

| `enum gfx_font` | Font (native cell) | Cropped cell w x h (`SKIN_*_ADV`, `SKIN_*_H`) | Cap height | Use |
|---|---|---|---|---|
| `GFX_VAL28` | Terminus 14x28 bold (derivative) | 14 x 20 | 18 (cell rows 0..17) | **Class A values:** OS, layout and language values; the popup readout `{n}/10` |
| `GFX_L24` | Leggie 12x24 bold | 12 x 18 | 14 (cell rows 2..15) | **The tally** (layer name); **the lock labels**; the popup title DISPLAY / BACKLIGHT |
| `GFX_L18` | Leggie 9x18 bold | 9 x 11 | 11 (the whole cell) | **Class B:** the legends OS / LAYOUT / LANGUAGE, AUTO, the scale `0` `5` `10` |

Rules:
- **Uppercase A–Z, 0–9 and `/` only.** The fonts are cropped to the rows these glyphs ink (VAL28
  rows 4..23 of the BDF cell, where the `Q` tail ends on row 23; L24 rows 4..21; L18 rows
  4..14). `scripts/elora-display-assets.py` asserts that every string the renderer draws
  (`STRINGS`) uses only the glyphs allowed for its font (`ALLOWED`). L18 has no `/`, because its
  `/` inks outside the crop.
- **The text origin is the top-left of the cropped cell** (the BDF cell's y + 4). Glyphs have no
  gap between cells; the advance is the cell width.
- **Budget rows by cap height, not by the cell.** All text is capitals, so layout is planned on the
  cap rows. Descenders (`Q`) may run into the gap below.
- **Left-aligned text starts at cell x 3** (`SKIN_LEGEND_X`, `SKIN_VALUE_X`): the 1 px left
  bearing puts the ink on x 4, the shared left edge of every fill.
- **Text that is centred is centred on its measured ink**, not on its cell:
  `cell x = x0 + (w - ink width) / 2 - ink left offset` (`centred_cell_x()` in `skin.py`). Never
  hand-compute these: they are generated into `display_skin_metrics.h` (`SKIN_TALLY_X[]`,
  `SKIN_LOCK_X[]`, `SKIN_READOUT_X[]`).
- **Hierarchy comes from colour and fill, never from a new size.** Don't add a fourth font or
  size, and don't enlarge something to emphasise it.
- The lock labels and the popup title are class B in meaning but use L24, deliberately: the lock bars
  match the tally's height and font, and the popup title matches the tally. Their state is still
  carried by the fill (locks) or is static (popup title).

Licences (the BDFs and licence texts are in `graphics/`):
- **Leggie** 12x24 bold and 9x18 bold (`leggie-24b.bdf`, `leggie-18b.bdf`), (c) 2012-2018 Wiktor
  Kerr, CC BY 4.0 (`graphics/LICENCE.leggie`).
- **Terminus** 14x28 bold (`ter-u28b.bdf`), (c) 2020 Dimitar Toshkov Zhekov, SIL OFL 1.1
  (`graphics/OFL.terminus.txt`). The firmware ships a **derived**, cropped caps-only bitmap
  (`font_val28_caps`); credit it as a Terminus derivative ("Terminus Font" is a Reserved Font
  Name under the OFL).

## 3. Colour

All colours are in `display_palette.h`, which is generated from the constants in `skin.py`. Use
`SKIN_*` names only, never raw RGB565 values. L* is the approximate CIE lightness of the design hex.

### 3.1 Text and neutral roles

| `SKIN_*` | Hex | RGB565 | L* | Role |
|---|---|---|---|---|
| `BLACK` | #000000 | 0x0000 | 0 | Background; text and icons on **lit** fills |
| `VFD` | #66FAE6 | 0x67DC | 90 | Live class A values: OS, layout, PRIMARY, the popup readout |
| `FL_AMBER` | #FFA000 | 0xFD00 | 74 | SECONDARY (live); meter segments 7–8 |
| `ALERT` | #FF3018 | 0xF983 | 56 | The UNKNOWN OS value; meter segments 9–10. Nothing else |
| `PANEL` | #CCCCCC | 0xCE79 | 82 | Class B text on black: legends, scale, the popup title |
| `MUTED` | #6E7380 | 0x6B90 | 48 | PRIMARY when the language doesn't apply |
| `MUTED_AMBER` | #966614 | 0x9322 | 47 | SECONDARY when the language doesn't apply |
| `BASE_FILL` / `BASE_TXT` | #1C1E22 / #8A8F98 | 0x18E4 / 0x8C73 | 11 / 59 | The quiet BASE tally |

The names say what each colour does: `VFD` is the glowing cyan used for values, `PANEL` is the
grey of printed panel legends, `TALLY_*` is the layer strip and `LAMP_G` is the green indicator
lamp.

### 3.2 Layer colours (the tally)

| Layer | Fill | Text |
|---|---|---|
| BASE | `BASE_FILL` #1C1E22 | `BASE_TXT` #8A8F98 |
| LOWER | `TALLY_LOWER` #4696FF (0x44BF) | `BLACK` |
| RAISE | `TALLY_RAISE` #FF6E3C (0xFB67) | `BLACK` |
| NUMPAD | `TALLY_NUMPAD` #FFCD28 (0xFE65) | `BLACK` |
| EXTRA | `TALLY_EXTRA` #CD5FFF (0xCAFF) | `BLACK` |

BASE is the default state, so it is quiet. Every other layer is a clearly different saturated fill,
so you notice that you're "stuck" on a layer. EXTRA is emphasised by its colour only. Nothing on
the screen is enlarged or changed on EXTRA (it isn't only a settings layer).

### 3.3 Indicator (lamp) colours

All the lamp colours have the same lightness (L* 72). Hue tells them apart, and brightness never
does: every lit indicator is equally bright, and every off one is equally dim.

| Base | Hex / RGB565 | Off fill (x0.22) | Off content (x0.45) | Used by |
|---|---|---|---|---|
| `LAMP_G` | #00CC57 / 0x066A | `LAMP_G_OFF_FILL` 0x0162 | `LAMP_G_OFF_TXT` 0x02C4 | CAPS, NUM, SCRL; meter 1–6 |
| `AMBER` | #F39E0F / 0xF4E1 | `AMBER_OFF_FILL` 0x3100 | `AMBER_OFF_TXT` 0x6A20 | WORD, both AUTO tags |
| `SHIFT` | #D9AB00 / 0xDD40 | 0x2920 | 0x6260 | Shift tile |
| `CTRL` | #FF9194 / 0xFC92 | 0x38E4 | 0x7208 | Ctrl tile |
| `ALT` | #3CC876 / 0x3E4E | 0x0963 | 0x1AC6 | Alt tile |
| `GUI` | #8AB0FF / 0x8D9F | 0x1927 | 0x3A6E | GUI tile |
| `FL_AMBER` | (3.1) | `FL_AMBER_OFF_FILL` 0x3900 | none | meter 7–8 |
| `ALERT` | (3.1) | `ALERT_OFF_FILL` 0x3840 | none | meter 9–10 |

### 3.4 Colour rules

- **One lit level, one dim level** (`OFF_FILL = 0.22`, `OFF_TXT = 0.45` in `skin.py`):
  - lit: the full colour as the fill, with the content (text or icon) in `BLACK`;
  - off: the fill is colour x0.22, and the content is colour x0.45. Meter segments have only the
    fill.

  Derive every new off colour this way (`ghost()` in the sketch). Never pick one by eye.
- **Off must be clearly dimmer than lit.** Telling on from off at a glance comes first. An off
  label doesn't have to be readable.
- **Brightness floors apply only to text that is read in its current state:** values, legends, the
  scale and labels on lit fills. Class A live values are at least L* ≈ 90 grey or a full-strength
  indicator colour. Class B text is at least L* ≈ 80 grey (`PANEL`). **Off indicators are exempt.**
  Raising an off label to a "readable" floor makes on and off look the same; that was a real
  mistake once.
- **Muted values are exempt as well.** A value that is the default or doesn't apply may be as quiet
  as needed: the BASE tally and the muted language.
- **CAPS is green and WORD is amber, and they are independent.** Each shows its own state, and
  both can be lit at once. Never tint CAPS amber for Caps Word.
- **Red means "act".** `ALERT` is used only for the UNKNOWN OS (the language keys are dead until
  you pick an OS) and for the top of the meter. Its legend stays `PANEL` grey: only the value turns
  red.
- **Amber marks a secondary or automatic state:** SECONDARY, WORD and AUTO.
- **Plain vs inverted:** text whose words change (values) is coloured text on black. On/off modes
  (tally, locks, AUTO, mod tiles) are `BLACK` content on a fill. Never invert legends or values.
- Add a colour only through `skin.py` and the generator (section 10), never in the C code.

## 4. Grid and spacing

- **Content block:** x 4..130 (`SKIN_X0` = 4, `SKIN_CW` = 127). The side margins are 4 px each
  (asserted). Every fill spans or divides this block exactly.
- **Horizontal divisions** (all asserted to sum to `SKIN_CW`):
  - 4 mod tiles: 28 px at a 33 px pitch, 5 px gaps (x 4, 37, 70, 103);
  - 2 lock bars: 61 px at x 4 and 70, 5 px gap; their outer edges line up with the tile edges;
  - 10 meter segments: 10 px at a 13 px pitch, 3 px gaps;
  - the AUTO tags: x 92..130 (`SKIN_TAG_X` 92, `SKIN_TAG_W` 39), flush with the right edge.
- **Vertical rhythm (main screen):** a 4 px top margin, **5 px** gaps inside the top block, **8 px**
  between the groups below it, **2 px** from a legend strip to its value, and a 4 px bottom
  margin. **No free rows at the bottom:** the sections are spread to fill the screen, and the last
  value's caps end on row 235.
- **Text inside fills:** 3 rows of padding above and below the caps (the tally and locks: a 20 px
  box, 14 px caps). Horizontally the text is centred on its ink.
- **Odd remainders stay odd.** Where a pixel grid can't split evenly (icon pads, the `5` of the
  scale over a 3 px gap), don't nudge one side to fake symmetry.

## 5. Zones (main screen), top to bottom

| Rows | Zone | Elements | Constants |
|---|---|---|---|
| 0–3 | margin | | |
| 4–23 | **Tally** (layer) | box 4,4,127,20; L24 at (`SKIN_TALLY_X[layer]`, 5) | `SKIN_TALLY_*` |
| 29–56 | **Mods** | Shift, Ctrl, Alt, GUI; 28x28 tiles at y 29 | `SKIN_MOD_*` |
| 62–81 | **Locks, row 1** | CAPS (x 4), WORD (x 70); 61x20 bars | `SKIN_LOCK_Y1` |
| 87–106 | **Locks, row 2** | NUM (x 4), SCRL (x 70) | `SKIN_LOCK_Y2` |
| 115–149 | **OS** | legend at y 117, AUTO tag 92,115,39,15; value at y 132 | `SKIN_OS_*` |
| 158–192 | **Layout** | legend at y 160, AUTO tag 92,158; value at y 175 | `SKIN_LAYOUT_*` |
| 201–235 | **Language** | legend at y 203 (no tag); value at y 218 | `SKIN_LANG_*` |
| 236–239 | margin | | |

Why this order:
- The layer is at the top, in a modest strip. In the middle of the screen, it competed with the
  rows of same-sized values below. The layer doesn't deserve much space.
- The state the hands control comes next: mods, then locks. These are fill indicators, so they are
  read by colour and need no text.
- The host-facing readouts come last, from the slowest-changing to the one that matters for typing:
  OS, then layout, then language.
- CAPS and WORD are next to each other, because they are both about capitals.

Section anatomy (OS, layout and language are identical):
- a **legend strip**, 15 rows tall: the L18 `PANEL` legend at x 3, with an optional AUTO tag
  on the right in the same strip;
- 2 rows of gap, then the VAL28 value at x 3, drawn over a cleared 128x20 rect
  (`SKIN_VALUE_W`). The widest value, SECONDARY, ends at x 127.
- Sections are separated by black space only. **No horizontal rules or dividers.**

## 6. Indicators

Every indicator is a solid fill with `BLACK` content when lit, and the x0.22 / x0.45 pair when off
(section 3.4). All of them are drawn by `lamp()` or `draw_mod()` in `display_skin.c`.

| Indicator | Lit when (`display_state_t`) | Colour |
|---|---|---|
| Shift / Ctrl / Alt / GUI tile | the mod is held **or one-shot** (`shift`, `ctrl`, `alt`, `gui`). Caps Word's own synthetic Shift doesn't count | `SHIFT`, `CTRL`, `ALT`, `GUI` |
| CAPS | Caps Lock (host LED) | `LAMP_G` |
| WORD | Caps Word | `AMBER` |
| NUM | Num Lock (host LED) | `LAMP_G` |
| SCRL | Scroll Lock (host LED); always shown | `LAMP_G` |
| OS AUTO | autodetection is on **and** the OS is known (`os_auto`) | `AMBER` |
| Layout AUTO | instant QWERTY is active (`layout_auto`) | `AMBER` |

Value rules:
- **OS:** LINUX / WINDOWS / MACOS in `VFD`. UNKNOWN is in `ALERT`, with a grey legend and an
  off AUTO tag.
- **Layout:** the **actual** current layout (COLEMAK or QWERTY) in `VFD`. AUTO says that it was
  switched automatically. No `*` suffixes, and no "TEMP" or "INSTEAD OF".
- **Language:** always PRIMARY or SECONDARY, never a real language name. When it applies,
  PRIMARY is `VFD` and SECONDARY is `FL_AMBER`. It is **muted** (`MUTED` / `MUTED_AMBER`) on every
  layer except BASE (the language keys live on BASE), and also on macOS or an unknown OS, where
  the language keys do nothing (`lang_muted` in `ds_resolve()`).
- **While LOWER or NUMPAD holds the language stash,** the language and layout from before it are
  shown, so tapping LOWER doesn't make them flicker. There is no stash indicator.
- **Layer names are BASE, LOWER, RAISE, NUMPAD, EXTRA.** It is never "NUM": that is the lock.

## 7. Icons

- **Mods are icons, not text** ("SFT" is cryptic), and they have **no captions**.
- 20x20, 1 bit (`SKIN_ICON_SIZE`), centred in the 28x28 tile with a 4 px inset (asserted).
  They are drawn natively on the 20x20 grid in `skin.py` (`SEGS`, `icon()`) and exported by the
  generator to `graphics/icon_*.png`, then `.qgf`.
- Stroke: orthogonal strokes are 2 px. Diagonals are compensated so that they look the same
  weight (distance ≤ 1.0 from the segment centre line). Nothing may ink row or column 20
  (asserted).
- The set: `SHIFT` (an outlined up arrow), `CTRL` (a caret), `ALT` (the option key), and the GUI
  icon by OS: `SUPER` (a diamond) for Linux and unknown, `WIN` (four panes, not a logo) for
  Windows, and `CMD` (⌘) for macOS.
- Icon colour follows the lamp rule: `BLACK` on a lit tile, the x0.45 colour on an off tile.
- Don't scale an icon, use an anti-aliased one, or use a brand logo.

## 8. Backlight popup

- **When:** a BL+ or BL- press on EXTRA shows it. It hides `SEK_DISPLAY_BL_OVERLAY_MS` (2500 ms,
  `config.h`) after the last press, **or at once when EXTRA is left**. Once hidden, it stays hidden
  when you come back to EXTRA (`ds_resolve()`). It is never shown permanently.
- **Full screen:** the main screen isn't drawn under it, and nothing else shares the screen.
- **The whole group is centred vertically:** it is 104 rows with **68 black rows above and 68
  below**. Every line is centred horizontally on its ink.

| Rows (ink) | Element | Font / colour | Constants |
|---|---|---|---|
| 68–81 | `DISPLAY` | L24 / `PANEL` | `SKIN_P1_X` 25, `SKIN_P1_Y` 66 |
| 87–100 | `BACKLIGHT` (5 rows of leading) | L24 / `PANEL` | `SKIN_P2_X` 13, `SKIN_P2_Y` 85 |
| 111–128 | `{n}/10` (**10-row gap** above) | VAL28 / `VFD` | `SKIN_READOUT_X[n]`, `SKIN_READOUT_Y` 111 |
| 139–154 | 10 segments (**10-row gap** above) | lamp rule | `SKIN_SEG_*` |
| 161–171 | scale `0` `5` `10` (6-row gap) | L18 / `PANEL` | `SKIN_SCALE_*` |

- **Even gaps:** title → readout and readout → segments are both 10 rows.
- **Segments:** segment k (0-based) is lit when k + 1 ≤ n. Segments 1–6 are `LAMP_G`, 7–8
  `FL_AMBER` and 9–10 `ALERT`; off segments are x0.22.
- **The readout must be exact:** `{n}/10` makes the graphic precise. Leave room for `10/10`, the
  widest readout.
- The title is two lines, because DISPLAY BACKLIGHT is 204 px wide and doesn't fit on one.
- The scale labels are tied to the segment positions, not centred.

## 9. Don'ts

- No scaled, pixel-doubled or synthetic-bold fonts or icons, no non-bold text and no italics.
- No new font sizes. Don't emphasise anything (EXTRA included) by size.
- No horizontal rules, frames, outlines, pinstripes, glows, gradients or rounded corners.
- No captions under the mod icons, and no explanatory text (MANUAL, TEMP, ON HOLD, "… WHEN
  LOWER IS RELEASED").
- No cryptic abbreviations (CMK, QWE). The only abbreviations are the lock labels NUM, CAPS,
  WORD and SCRL.
- Off states must not be bright, and must not get a brightness floor.
- One lit level and one dim level only. Don't make one indicator brighter than the others.
- Don't colour the OS legend red. Only the UNKNOWN value is red.
- Don't link CAPS and WORD.
- Leave no unused rows at the bottom: rebalance the gaps instead.
- Don't hand-edit `display_palette.h`, `display_skin_metrics.h` or anything generated in
  `graphics/`.
- Don't move an element between states.

## 10. Changing the skin

The order is always **sketch → assets → layout → renderer → check**. The sketch
(`scripts/elora_display_test/reference/sketch/skin.py`) is the design; the firmware follows it.

1. **Design it in the sketch.** Change `skin.py`: the colours, `ghost()` factors, icons or the
   `draw()` coordinates. Keep to the rules above: a class A or B font, lamp-rule colours, the grid
   and the gaps. If the five test states (`STATES` in `sketchlib.py`) can't show the change, make
   sure the extended matrix in `compare.py` covers it.
2. **Re-render the reference images** (from the repo root):

   ```sh
   scripts/qmk --exec python3 -I -B -c "import sys; sys.path.insert(0, 'scripts/elora_display_test/reference/sketch'); import skin as S; [S.draw(S.STATES[k]).save(f'scripts/elora_display_test/reference/natives/skin-{k}.png') for k in S.STATE_ORDER]"
   ```

   Look at all five PNGs at 1:1 and enlarged before you go on. Ideally, check them on the panel.
3. **Regenerate the assets:** `scripts/elora-display-assets.sh`. It rewrites the font strips,
   icons, `display_palette.h` and `display_skin_metrics.h`, and converts them to `.qff`/`.qgf`.
   It asserts that the crop rows, the allowed glyphs per font, the icon bounds and the HSV → RGB565
   round-trip all hold. A new string must be added to `STRINGS` (and a new centred one to
   `metrics_header()`). Running it twice must give no diff. Commit everything it writes.
4. **Update `display_skin_layout.h`.** Put every coordinate in a named `SKIN_*` constant (no
   numbers in `display_skin.c`), and add `_Static_assert`s:
   - the width divisions sum to `SKIN_CW`;
   - every cropped text cell lies inside its fill, or on black with nothing else on its pixels
     (`SKIN_INSIDE`);
   - each zone ends before the next one starts;
   - everything is on the screen.
5. **Update `display_skin.c`.** A main-screen element is a group in `GROUPS[]` (in drawing order)
   with:
   - `draw()`, which paints its **whole** box (the fill first, then text or icon cells with their
     own background), so it can be redrawn alone over anything;
   - `key()`, which encodes **everything** that `draw()` shows. `skin_draw_changes()` redraws a
     group only when its key changes.

   New state goes in `display_state_t` and `ds_resolve()`, never as a getter call in the renderer.
   If the new state needs a test state, update `display_test_states.c` to match the sketch states.
6. **Run `make check`.** `scripts/check-elora-display.sh` builds the renderer on the host and
   runs `scripts/elora_display_test/compare.py`, which checks:
   - the sketch re-renders the natives byte-identically;
   - `skin_hsv[]` gives exactly `skin_565[]`;
   - the five test states are pixel-exact (RGB565) with the natives;
   - an extended matrix (every layer, OS, language, layout, AUTO, mod and lock, and the popup at
     every level) matches the sketch;
   - incremental redraws equal full redraws;
   - the `ds_resolve()` cases pass.

   Diffs of failing states go to `build/scratch/display-diff-<state>.png`.
7. **Flash and look at it on the panel**, because colour and readability can only be judged on the
   panel. Then update this document if a rule or a number changed.
