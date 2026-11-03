#!/usr/bin/env bash
# Sofle-only build of the qmk_firmware commit currently checked out by `git bisect`
# inside the submodule (plan §10). Exit 125 = "skip this commit".
# On success the hex is copied to ../tmp/bisect/<qmk commit>/sofle.hex (with
# SHA256SUMS + README); flash that directory with scripts/flash-sofle.sh <dir>.
set -u
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel); q="$root/qmk_firmware"
PROJECT_ROOT=$(cd "$root/.." && pwd)
. "$root/scripts/toolchain.env"
qc=$(git -C "$q" rev-parse HEAD)   # the bisect commit, before any temporary cherry-pick
git -C "$q" submodule update --init --recursive -q || exit 125
# avr-libc 2.2.x needs #23890 (eeprom_write_qword); some develop-side commits predate it (e.g. 27 of 0.26.0's 269):
if ! git -C "$q" merge-base --is-ancestor 8b5cdfabf5d05958a607efa174e64377d36e4b64 HEAD; then
    git -C "$q" cherry-pick -n 8b5cdfabf5d05958a607efa174e64377d36e4b64 || { git -C "$q" reset -q --hard; exit 125; }
fi
rm -rf "$PROJECT_ROOT/tmp/qmk-build"
"$root/scripts/qmk" compile -kb sofle/rev1 -km sekogan -e SKIP_VERSION=yes; rc=$?
git -C "$q" reset -q --hard     # drop the temporary cherry-pick before bisect moves on
[ $rc -eq 0 ] || exit 125       # unrelated build failure -> skip this commit
dest="$PROJECT_ROOT/tmp/bisect/${qc:0:12}"
rm -rf "$dest"; mkdir -p "$dest"
cp "$PROJECT_ROOT/tmp/qmk-build/sofle_rev1_sekogan.hex" "$dest/sofle.hex"
printf 'Bisect build (unversioned, disposable)\nStatus: untested\n\n  qmk commit:  %s\n  outer commit: %s\n  toolchain:   %s\n  built:       %s\n' "$qc" "$(git -C "$root" rev-parse HEAD)" "$QMK_IMAGE_ID" "$(date -u +%FT%TZ)" > "$dest/README"
(cd "$dest" && sha256sum sofle.hex > SHA256SUMS)
echo "bisect build: $dest   (flash each half: $root/scripts/flash-sofle.sh $dest)"
