#!/usr/bin/env bash
# Layer sync: the Elora keymap must hold the Sofle's layers, with the four inner bottom-row keys
# blank (the keymap.c files are separate, so nothing else catches drift).
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
ok = len(sofle) == len(elora)
for i, (s, e) in enumerate(zip(sofle, elora)):
    # Elora indices 42-45 (L20, L19, R19, R20) are blank; Sofle 42-43 (encoder pushes) are not carried over.
    if len(s) != 60 or len(e) != 62 or e[42:46] != ["KC_NO"] * 4 or e[:42] + e[46:] != s[:42] + s[44:]:
        print(f"layer {i}: Elora differs from Sofle"); ok = False
sys.exit(0 if ok else 1)
PY
echo "layer check OK: Elora matches Sofle"
