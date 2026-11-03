#!/usr/bin/env bash
# SV-canary (plan §6): QMK hooks are weak symbols, so a renamed or no-longer-called
# hook still compiles and silently does nothing. Build without LTO (so unreferenced
# functions are dropped by --gc-sections) and require each hook as a STRONG text
# symbol (type T). QMK's own weak defaults show up as W and do not count.
# Builds use a separate TARGET/INTERMEDIATE_OUTPUT in ../tmp/qmk-build, so the normal
# (LTO) build outputs are left alone; the non-LTO .hex is deleted afterwards.
set -euo pipefail
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel)
PROJECT_ROOT=$(cd "$root/.." && pwd)
. "$root/scripts/toolchain.env"
km=sekogan
build="$PROJECT_ROOT/tmp/qmk-build"   # QMK build dir (scripts/qmk)
tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
fail=0

# canary_build <kb> -> prints "<name>" of the non-LTO build
canary_build() {
  local kb=$1 name="${1//\//_}_${km}_canary"
  # "too large" is fine: the .elf is kept
  "$root/scripts/qmk" compile -kb "$kb" -km "$km" -e SKIP_VERSION=yes -e LTO_ENABLE=no -e EXTRAFLAGS= \
      -e TARGET="$name" -e INTERMEDIATE_OUTPUT="/qmk_build/obj_$name" >"$tmp/$name.log" 2>&1 || true
  rm -f "$build/$name.hex"   # never flashable by accident
  [ -f "$build/$name.elf" ] || { cat "$tmp/$name.log" >&2; echo "canary build failed: $kb" >&2; exit 1; }
  echo "$name"
}
# symbols <name> -> "<type> <symbol>" lines
symbols() {
  podman run --rm --pull=never -v "$build":/b:ro,Z -w /b "$QMK_IMAGE" avr-nm "$1.elf" | awk 'NF==3{print $2, $3}'
}
# require <name> <types-regex> <symbol>...
require() {
  local name=$1 types=$2; shift 2
  for sym in "$@"; do
    if ! awk -v t="^($types)\$" -v s="$sym" '$1 ~ t && $2 == s {found=1} END {exit !found}' "$tmp/$name.nm"; then
      echo "MISSING in $name: $sym (want type $types)"; fail=1
    fi
  done
}

# Sofle: the expected strong hooks are a committed file (edited by every step that adds/removes a hook).
n=$(canary_build sofle/rev1); symbols "$n" > "$tmp/$n.nm"
awk '$1=="T"{print $2}' "$tmp/$n.nm" | sort > "$tmp/sofle-T.txt"
missing=$(grep -v '^#' "$root/scripts/canary-sofle.expected" | sort | comm -13 "$tmp/sofle-T.txt" -)
if [ -n "$missing" ]; then echo "MISSING strong (T) hooks in sofle/rev1:"; echo "$missing"; fail=1; fi

# KBD75 (plan §6, from 1.17)
n=$(canary_build kbdfans/kbd75hs); symbols "$n" > "$tmp/$n.nm"
require "$n" T led_update_user
require "$n" '[DdBbRr]' key_overrides tap_dance_actions
n=$(canary_build kbdfans/kbd75/rev2); symbols "$n" > "$tmp/$n.nm"
require "$n" T led_update_user

if [ $fail = 0 ]; then echo "canary OK: $(grep -vc '^#' "$root/scripts/canary-sofle.expected") Sofle hooks + KBD75 checks"; fi
exit $fail
