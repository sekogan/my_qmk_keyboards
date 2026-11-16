# Halcyon Elora keymap: maintainer notes

What the code does not show. The repo-wide rules (versioning, releases, checks) are in the
[root README](../../../../../../README.md).

## Layers

The Elora `keymap.c` holds its own copy of the Sofle layers, so a layer change is made in
both files. `scripts/check-elora-layout.sh` (part of `make check`) fails if they drift apart;
the intended differences are listed in the script.

## Fonts

Both display fonts are derivatives: the glyphs were cropped to their inked rows and
converted to QFF, with no other changes. "Terminus Font" is a Reserved Font Name under the
SIL OFL, so the font derived from it must not be called Terminus (it is `font_val28_caps`).
