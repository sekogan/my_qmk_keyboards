#!/usr/bin/env bash
# Sofle-only build of the qmk_firmware commit currently checked out by `git bisect`
# inside the submodule (plan §10). Exit 125 = "skip this commit".
# On success the hex is copied to ../qmk-data/qmk-scratch/bisect/<qmk commit>/ (with
# SHA256SUMS + BUILDINFO); flash that directory with scripts/flash-sofle.sh <dir>.
set -u
root=$(git -C "$(dirname "$0")" rev-parse --show-toplevel); q="$root/qmk_firmware"
QMK_DATA=$(cd "$root/.." && pwd)/qmk-data
. "$root/scripts/toolchain.env"
qc=$(git -C "$q" rev-parse HEAD)   # the bisect commit, before any temporary cherry-pick
git -C "$q" submodule update --init --recursive -q || exit 125
# avr-libc 2.2.x needs #23890 (eeprom_write_qword); some develop-side commits predate it (e.g. 27 of 0.26.0's 269):
if ! git -C "$q" merge-base --is-ancestor 8b5cdfabf5d05958a607efa174e64377d36e4b64 HEAD; then
    git -C "$q" cherry-pick -n 8b5cdfabf5d05958a607efa174e64377d36e4b64 || { git -C "$q" reset -q --hard; exit 125; }
fi
rm -rf "$q/.build"
"$root/scripts/qmk" compile -kb sofle/rev1 -km sekogan -e SKIP_VERSION=yes; rc=$?
git -C "$q" reset -q --hard     # drop the temporary cherry-pick before bisect moves on
[ $rc -eq 0 ] || exit 125       # unrelated build failure -> skip this commit
dest="$QMK_DATA/qmk-scratch/bisect/${qc:0:12}"
rm -rf "$dest"; mkdir -p "$dest"
cp "$q/.build/sofle_rev1_sekogan.hex" "$dest/"
printf 'qmk_commit=%s\nouter_commit=%s\nimage_id=%s\nbuilt=%s\n' "$qc" "$(git -C "$root" rev-parse HEAD)" "$QMK_IMAGE_ID" "$(date -u +%FT%TZ)" > "$dest/BUILDINFO"
(cd "$dest" && sha256sum sofle_rev1_sekogan.hex BUILDINFO > SHA256SUMS)
echo "bisect build: $dest   (flash each half: $root/scripts/flash-sofle.sh $dest)"
