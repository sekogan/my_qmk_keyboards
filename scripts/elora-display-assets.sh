#!/usr/bin/env bash
# Regenerates the Elora display assets (skin) inside the pinned image:
#   1. scripts/elora-display-assets.py: the font images, icon PNGs, display_palette.h and
#      display_skin_metrics.h, all derived from the tracked sketch copy and the BDF fonts;
#   2. qmk painter-convert-font-image / painter-convert-graphics (-f mono2): the .qff.c/.h and
#      .qgf.c/.h that the firmware links.
# Everything it writes is committed; the build never runs it. Running it twice gives no diff.
set -euo pipefail
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel)
gfx=keyboards/splitkb/halcyon/elora/keymaps/sekogan/graphics
"$root/scripts/qmk" --exec sh -euc '
  gfx=$1
  python3 -I -B scripts/elora-display-assets.py
  for f in font_val28_caps font_l24_caps font_l18_caps; do
    qmk painter-convert-font-image -i "$gfx/$f.png" -f mono2 -o "$gfx"
  done
  for i in shift ctrl alt super win cmd; do
    qmk painter-convert-graphics -i "$gfx/icon_$i.png" -f mono2 -o "$gfx"
  done
' sh "$gfx"
