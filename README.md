# Firmware for my QMK-based keyboards

Custom [QMK](https://qmk.fm) keymaps, organised as a QMK
[External Userspace](https://docs.qmk.fm/newbs_external_userspace):

| Keyboard | QMK target | Keymap |
|---|---|---|
| [Sofle](keyboards/sofle/keymaps/sekogan/readme.md) | `sofle/rev1` | `keyboards/sofle/keymaps/sekogan/` |
| [KBD75 v3.1](keyboards/kbdfans/kbd75hs/keymaps/sekogan/readme.md) | `kbdfans/kbd75hs` | `keyboards/kbdfans/kbd75hs/keymaps/sekogan/` |
| [KBD75 v2](keyboards/kbdfans/kbd75/keymaps/sekogan/readme.md) | `kbdfans/kbd75/rev2` | `keyboards/kbdfans/kbd75/keymaps/sekogan/` |

**Pinned versions** — the repo alone is enough to reproduce any build:

- **Firmware version = `VERSION`** (one [SemVer](https://semver.org) version for all three
  keyboards; see below). History: [CHANGELOG.md](CHANGELOG.md).
- **QMK version = the `qmk_firmware` git submodule** (currently 0.29.0). Bumping QMK
  is a commit that moves the submodule.
- **Toolchain = `scripts/toolchain.env`**: the official `qmk_cli` container image,
  pinned by registry digest and image ID, run with rootless Podman. Nothing else
  (compiler, Python, qmk CLI) needs to be installed on the host.

All commands run from the repo root. Nothing outside the repo is used.
`make` is the front door (`make help` lists the targets); it calls the scripts in `scripts/`,
which can also be run directly when you need extra arguments.

## Directory layout

```
README.md CHANGELOG.md VERSION qmk.json Makefile requirements-dev.txt
keyboards/             the keymaps
docs/                  hardware docs (KBD75 v2)
scripts/               build/flash/check scripts; toolchain.env = the toolchain pin
qmk_firmware/          QMK, as a pinned git submodule
build/                 git-ignored; everything in it is disposable (a cache), except build/readback/
├── firmware/qmk-<QMK version>-<VERSION>/   releases: sofle.hex kbd75hs.hex kbd75v2.hex
│                                           (read-only) + SHA256SUMS + README (origin)
├── toolchain/         saved copy of the toolchain image (qmk-toolchain-*.oci.tar + .sha256)
├── qmk/               QMK's build directory (.elf/.map/objects of the last builds)
├── readback/          flash dumps read from the Sofle halves (NOT disposable: cannot be
│                      recreated without the hardware; SHA256SUMS; back it up)
├── scratch/           check builds (scratch/check), bisect builds, notes
└── venv/              keymapviz venv
```

`make clean` removes `build/qmk` and `build/scratch`; the rest of `build/` except `build/readback/`
can be deleted by hand at any time: releases are rebuilt from their commit, the image is loaded
or pulled again. `build/readback/` holds data read from the hardware; keep it (and back it up).

## Firmware version

`VERSION` holds one [Semantic Versioning](https://semver.org) version for the whole repo
(all three keyboards). It is **not** compiled into the firmware, so unchanged sources still
give byte-identical builds.

| Part | Bump when |
|---|---|
| MAJOR | you have to relearn something: a key, layer or shortcut moved, was removed or changed |
| MINOR | something was added without changing existing behaviour |
| PATCH | no intended behaviour change: refactor, QMK bump, toolchain change, bug fix |

A commit that produces firmware to keep bumps `VERSION` and adds a [CHANGELOG.md](CHANGELOG.md)
entry in the same commit; a commit whose firmware is byte-identical keeps the version.
`make release` (`scripts/build_all.sh --release`) builds a **release** of a clean commit:
`build/firmware/qmk-<QMK version>-<VERSION>/` (the version is everything after the last `-`),
read-only, with `SHA256SUMS` and a `README` naming its origin. Running it again for the same
version is a no-op if the build is identical, and is refused if it differs ("VERSION not
bumped?"). A release is reproducible (rebuild its commit with the pinned image and you get
the same bytes), so it is fine to keep it in the disposable `build/`.

Versions 1.0.0–1.0.3 were assigned retroactively (`VERSION` was introduced at 1.0.3).

## Setup

```bash
git clone --recurse-submodules https://github.com/sekogan/my_qmk_keyboards
cd my_qmk_keyboards
sudo dnf install podman avrdude dfu-programmer
sudo cp qmk_firmware/util/udev/50-qmk.rules /etc/udev/rules.d/ && sudo udevadm control --reload-rules
```

The toolchain image is fetched on first use by `scripts/qmk`: from podman's store if
present, else from the saved archive `build/toolchain/*.oci.tar` (checksum verified), else
pulled by the pinned digest (and then saved to `build/toolchain/`). The image ID is always
verified. QMK's registries keep only a few tags, so keep a copy of `build/toolchain/` on
your backup medium; to restore it, put it back into `build/toolchain/`.

## Build

```bash
make              # all targets + size and sha256 (scripts/build_all.sh; reproducible: SKIP_VERSION=yes)
make release      # ... and make a release in build/firmware/ (scripts/build_all.sh --release)
make check        # hook canary + qmk lint for every target (see below)
make help         # all targets
```

For anything else, use the scripts directly, e.g.:

```bash
scripts/qmk userspace-compile                     # all targets in qmk.json
scripts/qmk compile -kb sofle/rev1 -km sekogan    # one target
```

`scripts/qmk` runs any `qmk` subcommand inside the pinned image (`scripts/qmk --exec <cmd>`
runs other tools, e.g. `avr-nm`), with the repo as the userspace and the submodule as QMK
home. QMK's build directory is `build/qmk/`; nothing else in the repo is written.
`make` (`scripts/build_all.sh`) also copies the three `.hex` files (short names + `SHA256SUMS`)
to `build/scratch/check/`.

`build/firmware/` keeps `.hex` files only. Builds are reproducible, so to get the
`.elf`/`.map` of a version, check out its commit (in its `README`) and rebuild:

```bash
# clean tree; e.g. firmware 1.0.3 = tag qmk-0.26.0
git switch --detach qmk-0.26.0 && git submodule update --init --recursive
scripts/build_all.sh               # .elf/.map now in build/qmk/ (older commits have no make targets)
sha256sum build/scratch/check/sofle.hex build/firmware/qmk-0.26.0-1.0.3/sofle.hex   # must match
git switch qmk-modernization && git submodule update --init --recursive
```

Commits before the current layout (firmware 1.0.2, 1.0.3) still have older scripts, which
put the output in `qmk_firmware/.build/` plus `*.hex` copies in the repo root (delete them
afterwards). 1.0.0 and 1.0.1 were built outside this repo layout; 1.0.0 used the 2024 host
toolchain (avr-gcc 14.1) and cannot be reproduced with the pinned image.

Checks used when changing things (`make check` runs both):

- `scripts/canary.sh`: verifies that every QMK hook the keymaps implement is
  really linked (QMK hooks are weak symbols; a renamed hook would compile and
  silently do nothing). The expected Sofle list is `scripts/canary-sofle.expected`.
- `scripts/qmk lint -kb <kb> -km sekogan` for each target in `qmk.json`.

## Flash

Flashing runs on the host, not in the container.

- **Sofle** (Pro Micro, Caterina bootloader): flash **each half** from a release or another
  build with `SHA256SUMS`, never from QMK's build directory (any build may replace or delete
  files there):
  ```bash
  make release                                 # -> build/firmware/qmk-<QMK version>-<VERSION>/
  make flash                                   # the release of this commit's VERSION (scripts/flash-sofle.sh)
  make flash FW=build/firmware/qmk-0.26.0-1.0.3  # or any firmware dir, build/scratch/check, build/scratch/bisect/<commit>
  ```
  The script prints the hex's sha256 and origin, refuses a file whose hash does not match
  its `SHA256SUMS`, then waits for you to double-tap the reset button of the half and runs
  avrdude.
- **KBD75** (atmel-dfu): put the board into the bootloader (Caps Lock + Esc + F12),
  then
  ```bash
  sudo dfu-programmer atmega32u4 erase
  sudo dfu-programmer atmega32u4 flash build/firmware/qmk-<QMK version>-<VERSION>/kbd75hs.hex   # or kbd75v2.hex
  sudo dfu-programmer atmega32u4 launch
  ```

## Keymap diagrams

The ASCII layer diagrams in the Sofle `keymap.c` are generated by
[keymapviz](https://github.com/yskoht/keymapviz), pinned in `requirements-dev.txt`:

```bash
make keymapviz     # creates build/venv on first use (make venv)
```
