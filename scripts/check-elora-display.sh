#!/usr/bin/env bash
# Elora display: the skin renderer must be pixel-exact with the design's natives.
# In the pinned image (gcc, Python 3 + Pillow), from tracked files only:
#   - compiles display_skin.c, display_test_states.c and the pure part of display_state.c
#     with the host recorder (scripts/elora_display_test/host_gfx.c, render_states.c) and
#     QMK's quantum/color.c (hsv_to_rgb_nocie() for the palette check), with placeholder
#     versions (compare.py gives the version popup its versions on render_states' V line);
#   - scripts/elora_display_test/compare.py: the sketch copy re-renders the natives, the
#     palette, the five design test states against the natives (RGB565), an extended matrix
#     and the incremental redraws against the sketch, and the ds_resolve() cases.
# Diff images of failing states go to build/scratch/display-diff-<state>.png.
set -euo pipefail
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel)
mkdir -p "$root/build/scratch"
tmp=$(mktemp -d -p "$root/build/scratch" .display.XXXXXX); trap 'rm -rf "$tmp"' EXIT
rel=${tmp#"$root"/}
"$root/scripts/qmk" --exec sh -euc '
  km=keyboards/splitkb/halcyon/elora/keymaps/sekogan
  t=scripts/elora_display_test
  qmk_inc="-I qmk_firmware/quantum -I qmk_firmware/platforms"
  # QMK code, not ours: its hsv_to_rgb_impl() leaves use_cie unused without USE_CIE1931_CURVE.
  gcc -std=c11 -Wall -Wextra -Werror -Wno-unused-parameter -O2 $qmk_inc \
    -c qmk_firmware/quantum/color.c -o "$1/color.o"
  gcc -std=c11 -Wall -Wextra -Werror -O2 -DSEK_DISPLAY_HOST -DSEK_KEYMAP_VERSION=0.0.0 -DSEK_QMK_VERSION=0.0.0 \
    -include "$km/config.h" -I "$km" $qmk_inc \
    "$km/display_skin.c" "$km/display_test_states.c" "$km/display_state.c" \
    "$t/host_gfx.c" "$t/render_states.c" "$1/color.o" -o "$1/render_states"
  python3 -I -B "$t/compare.py" "$1/render_states" build/scratch
' sh "$rel"
echo "display check OK: skin pixel-exact"
