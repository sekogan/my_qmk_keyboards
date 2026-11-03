# Firmware for my QMK-based keyboards

Custom [QMK](https://qmk.fm) keymaps, organised as a QMK
[External Userspace](https://docs.qmk.fm/newbs_external_userspace):

| Keyboard | QMK target | Keymap |
|---|---|---|
| [Sofle](keyboards/sofle/keymaps/sekogan/readme.md) | `sofle/rev1` | `keyboards/sofle/keymaps/sekogan/` |
| [KBD75 v3.1](keyboards/kbdfans/kbd75hs/keymaps/sekogan/readme.md) | `kbdfans/kbd75hs` | `keyboards/kbdfans/kbd75hs/keymaps/sekogan/` |
| [KBD75 v2](keyboards/kbdfans/kbd75/keymaps/sekogan/readme.md) | `kbdfans/kbd75/rev2` | `keyboards/kbdfans/kbd75/keymaps/sekogan/` |

**Pinned versions**

- **Firmware version = `VERSION`** (one [SemVer](https://semver.org) version for all three
  keyboards; see below).
- **QMK version = the `qmk_firmware` git submodule** (currently 0.26.0). Bumping QMK
  is a commit that moves the submodule.
- **Toolchain = `scripts/toolchain.env`**: the official `qmk_cli` container image,
  pinned by image ID and run with rootless Podman. Nothing else (compiler, Python,
  qmk CLI) needs to be installed on the host.

## Directory layout

Everything lives in one project directory; nothing is kept in your home directory.

```
<project>/
├── my_qmk_keyboards/   this repo
├── firmware/           every kept firmware build (not in git)
│   └── qmk-<QMK version>-<firmware version>/   e.g. qmk-0.26.0-1.0.3/
│       ├── sofle.hex  kbd75hs.hex  kbd75v2.hex  (read-only)
│       ├── SHA256SUMS
│       └── README     Status line (may be updated later) + origin: commit, tags, QMK
│                      version, toolchain image ID, compiler, date
├── toolchain/          qmk-toolchain-1.2.0-b7d7fa8f.oci.tar (+ .sha256): the saved Podman image
├── backups/            repo bundles
└── tmp/                disposable: check builds (tmp/build), bisect builds, flash readbacks,
                        compile-flag snapshots
```

## Firmware version

`VERSION` holds one [Semantic Versioning](https://semver.org) version for the whole repo
(all three keyboards). It is **not** compiled into the firmware, so unchanged sources still
give byte-identical builds.

| Part | Bump when |
|---|---|
| MAJOR | you have to relearn something: a key, layer or shortcut moved, was removed or changed |
| MINOR | something was added without changing existing behaviour |
| PATCH | no intended behaviour change: refactor, QMK bump, toolchain change, bug fix |

A commit that produces firmware to keep bumps `VERSION` in the same commit; a commit whose
firmware is byte-identical to the current version keeps it. `scripts/build_all.sh --archive`
saves the build as `../firmware/qmk-<QMK version>-<VERSION>/` (the version is everything after
the last `-`). Running it again for the same version is a no-op if the build is identical, and
is refused if it differs ("VERSION not bumped?"). Each directory's `README` has a `Status:`
line (`untested`, `tested <date>: <what>`, `released`, ...) that you can edit later; the
firmware files themselves are read-only and listed in `SHA256SUMS`.

Versions 1.0.0–1.0.3 were assigned retroactively (the repo got `VERSION` with 1.0.3):
1.0.0 = the 2024-06-15 build that was on the Sofle, 1.0.1 = its gcc-15 rebuild,
1.0.2 = end of the userspace refactor, 1.0.3 = QMK 0.26.0.

Commands below run inside `my_qmk_keyboards/` unless noted.

## Setup

```bash
# in <project>/
git clone --recurse-submodules https://github.com/sekogan/my_qmk_keyboards
sudo dnf install podman avrdude dfu-programmer
```

Load the pinned toolchain image from the saved archive (QMK's registries keep only a
few tags, so the archive is the reliable copy; path, image ID and registry digest are
in `scripts/toolchain.env`):

```bash
# in <project>/
(cd toolchain && sha256sum -c qmk-toolchain-1.2.0-b7d7fa8f.oci.tar.sha256)
podman load -i toolchain/qmk-toolchain-1.2.0-b7d7fa8f.oci.tar
podman image inspect --format '{{.Id}}' localhost/qmk-toolchain:1.2.0-b7d7fa8f
# -> 4f4c32f9c637dde3ab59c9f9068a1b3b2b098067e7677f8e7cab98d34cdde60b
```

If the archive is lost but the digest is still on the registry:
`podman pull <QMK_IMAGE_SOURCE>` and `podman tag <QMK_IMAGE_SOURCE> <QMK_IMAGE>`
(values from `scripts/toolchain.env`), then save a new archive with
`podman save --format oci-archive`.

udev rules, so avrdude / dfu-programmer can open the bootloaders without root:

```bash
sudo cp qmk_firmware/util/udev/50-qmk.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
```

## Build

```bash
scripts/qmk userspace-compile                     # all targets in qmk.json
scripts/qmk compile -kb sofle/rev1 -km sekogan    # one target
scripts/build_all.sh                              # all targets + size and sha256 (reproducible: SKIP_VERSION=yes)
```

`scripts/qmk` runs any `qmk` subcommand inside the pinned image, with this repo
as the userspace and the submodule as QMK home. QMK's build directory is
`../tmp/qmk-build/`; nothing is written into the repo. `scripts/build_all.sh` also
copies the three `.hex` files (short names + `SHA256SUMS`) to `../tmp/build/`.

`.elf`/`.map` files are not kept in `../firmware/` (only `qmk-0.25.8-1.0.0` has them,
because that 2024 build cannot be reproduced with the pinned toolchain). Builds are
reproducible, so to get them for a version, check out its commit and rebuild:

```bash
# in my_qmk_keyboards/, with a clean tree; e.g. firmware 1.0.3 = tag qmk-0.26.0
# (the exact commit of every version is in ../firmware/*/README)
git switch --detach qmk-0.26.0 && git submodule update --init --recursive
scripts/build_all.sh               # .elf/.map now in ../tmp/qmk-build/
sha256sum ../tmp/build/sofle.hex ../firmware/qmk-0.26.0-1.0.3/sofle.hex   # must match
git switch qmk-modernization && git submodule update --init --recursive
```

Commits older than the `VERSION` file (firmware 1.0.2 and 1.0.3) still have the old
scripts: there the output is in `qmk_firmware/.build/` and `*.hex` copies in the
repo root (delete them afterwards). 1.0.1 was built outside this repo layout (plan step 1.0b).

Checks used when changing things:

- `scripts/canary.sh`: verifies that every QMK hook the keymaps implement is
  really linked (QMK hooks are weak symbols; a renamed hook would compile and
  silently do nothing). The expected Sofle list is `scripts/canary-sofle.expected`.
- `scripts/qmk lint -kb <kb> -km sekogan`.

## Flash

Flashing runs on the host, not in the container.

- **Sofle** (Pro Micro, Caterina bootloader): flash **each half** from an archived build,
  never from QMK's build directory (any build may replace or delete files there):
  ```bash
  scripts/build_all.sh --archive                  # -> ../firmware/qmk-<QMK version>-<VERSION>/
  scripts/flash-sofle.sh                          # the archived build of this commit's VERSION
  scripts/flash-sofle.sh ../firmware/qmk-0.26.0-1.0.3   # or any firmware dir, ../tmp/build, ../tmp/bisect/<commit>
  ```
  The script prints the hex's sha256 and the commit it was built from, refuses a file
  whose hash does not match its `SHA256SUMS`, then waits for you to double-tap the
  reset button of the half and runs avrdude.
- **KBD75** (atmel-dfu): put the board into the bootloader (Caps Lock + Esc + F12),
  then
  ```bash
  sudo dfu-programmer atmega32u4 erase
  sudo dfu-programmer atmega32u4 flash ../firmware/qmk-<QMK version>-<VERSION>/kbd75hs.hex   # or kbd75v2.hex
  sudo dfu-programmer atmega32u4 launch
  ```

## Keymap diagrams

The ASCII layer diagrams in the Sofle `keymap.c` are generated by
[keymapviz](https://github.com/yskoht/keymapviz), pinned in `requirements-dev.txt`:

```bash
python3 -m venv .venv && PIP_NO_CACHE_DIR=1 .venv/bin/pip install -r requirements-dev.txt
make keymapviz
```
