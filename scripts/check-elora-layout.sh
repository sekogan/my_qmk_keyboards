#!/usr/bin/env bash
# Layer sync: the Elora keymap must hold the Sofle's layers, except for the Elora-only keys
# listed below (the keymap.c files are separate, so nothing else catches drift).
set -euo pipefail
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel)
mkdir -p "$root/build/scratch"; tmp=$(mktemp -d -p "$root/build/scratch" .layout.XXXXXX); trap 'rm -rf "$tmp"' EXIT
sofle=keyboards/sofle/keymaps/sekogan/keymap.c
elora=keyboards/splitkb/halcyon/elora/keymaps/sekogan/keymap.c
"$root/scripts/qmk" c2json --no-cpp -km sekogan -kb sofle/rev1 "$sofle" > "$tmp/sofle.json"
"$root/scripts/qmk" c2json --no-cpp -km sekogan -kb splitkb/halcyon/elora/rev2 "$elora" > "$tmp/elora.json"
python3 -I - "$tmp/sofle.json" "$tmp/elora.json" <<'PY'
import json, sys
sofle = json.load(open(sys.argv[1]))["layers"]
elora = json.load(open(sys.argv[2]))["layers"]
QWERTY, COLEMAK, NUMPAD, LOWER, RAISE, EXTRA = range(6)
# Elora indices: rows of 12 (0-35), the bottom row of 16 with L20, L19, R19, R20 at 42-45,
# then the thumbs (left 52-56, right 57-61). Sofle 42-43 (encoder pushes) are not carried over.
# The four inner bottom-row keys: assigned on QWERTY and COLEMAK, R20 transparent on NUMPAD, blank otherwise.
assigned = ["KC_LSFT", "KC_LNG1", "KC_LNG2", "L_NUM"]
inner = {QWERTY: assigned, COLEMAK: assigned, NUMPAD: ["KC_NO"] * 3 + ["KC_TRNS"]}  # R20 (L_NUM) toggles NUMPAD off
blank = ["KC_NO"] * 4
# Other Elora-only keys, by layer: {index: keycode}.
elora_only = {
    QWERTY:  {52: "KC_LGUI", 57: "KC_ENT", 61: "KC_RGUI"},
    COLEMAK: {52: "KC_LGUI", 57: "KC_ENT", 61: "KC_RGUI"},
    RAISE:   {51: "KC_TRNS"},
    EXTRA:   {14: "BL_UP", 18: "KC_NO", 20: "KC_VOLU", 26: "BL_DOWN", 29: "SEK_OS",
              31: "KC_MPRV", 32: "KC_VOLD", 33: "KC_MNXT", 48: "KC_MUTE", 49: "KC_MPLY"},
}
ok = len(sofle) == len(elora)
for i, (s, e) in enumerate(zip(sofle, elora)):
    if len(s) != 60 or len(e) != 62:
        print(f"layer {i}: unexpected size"); ok = False; continue
    expected = s[:42] + inner.get(i, blank) + s[44:]
    for k, v in elora_only.get(i, {}).items():
        expected[k] = v
    for k, (want, got) in enumerate(zip(expected, e)):
        if want != got:
            print(f"layer {i} key {k}: Elora has {got}, expected {want}"); ok = False
sys.exit(0 if ok else 1)
PY
echo "layer check OK: Elora matches Sofle"
