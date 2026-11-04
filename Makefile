.SILENT:

MAKEFLAGS += --no-print-directory

# The front door for the usual tasks. These are thin wrappers: the logic is in scripts/,
# which run the toolchain inside the pinned image (the host has no avr-gcc and no qmk).
# QMK finds this External Userspace through qmk.json, not through this Makefile.

QMK_USERSPACE := $(patsubst %/,%,$(dir $(shell realpath "$(lastword $(MAKEFILE_LIST))")))
ifeq ($(QMK_USERSPACE),)
    QMK_USERSPACE := $(shell pwd)
endif
SCRIPTS := $(QMK_USERSPACE)/scripts

# "<keyboard>:<keymap>" for every build target in qmk.json (used by `make check`).
QMK_TARGETS = $(shell python3 -I -c 'import json, sys; print(" ".join(kb + ":" + km for kb, km in json.load(open(sys.argv[1]))["build_targets"]))' $(QMK_USERSPACE)/qmk.json)

# Optional firmware directory or .hex for `make flash` (default: the release of HEAD's VERSION).
FW ?=

# keymapviz only rewrites the ASCII-art comments in keymap.c, so it runs on the
# host from a venv in build/venv (`make venv` creates it; disposable).
VENV := $(QMK_USERSPACE)/build/venv
KEYMAPVIZ ?= $(VENV)/bin/keymapviz
KM_SOFLE := $(QMK_USERSPACE)/keyboards/sofle/keymaps/sekogan

.PHONY: all release flash check help keymapviz venv clean submodule

all: submodule
	$(SCRIPTS)/build_all.sh

release: submodule
	$(SCRIPTS)/build_all.sh --release

flash: submodule
	$(SCRIPTS)/flash-sofle.sh $(if $(FW),"$(FW)")

check: submodule
	$(SCRIPTS)/canary.sh
	set -e; for t in $(QMK_TARGETS); do \
	  echo "lint $${t%%:*} -km $${t##*:}"; \
	  $(SCRIPTS)/qmk lint -kb "$${t%%:*}" -km "$${t##*:}"; \
	done

help:
	echo 'make                build all targets, print size + sha256 (scripts/build_all.sh);'
	echo '                    a copy goes to build/scratch/check/'
	echo 'make release        build a release: build/firmware/qmk-<QMK version>-<VERSION>/'
	echo '                    (scripts/build_all.sh --release; clean tree only)'
	echo 'make flash          flash one Sofle half from the release of HEAD'"'"'s VERSION'
	echo '                    (scripts/flash-sofle.sh); FW=<dir|hex> flashes another build'
	echo 'make check          hook canary (scripts/canary.sh) + qmk lint for every target in qmk.json'
	echo 'make keymapviz      regenerate the layer diagrams in the Sofle keymap.c'
	echo 'make venv           create the keymapviz venv in build/venv'
	echo 'make clean          remove build/qmk and build/scratch (never build/readback)'
	echo 'Other qmk commands: scripts/qmk <subcommand> ... (e.g. scripts/qmk compile -kb sofle/rev1 -km sekogan)'

# qmk_firmware is the pinned submodule; the build/flash/check scripts need it.
submodule:
	test -f $(QMK_USERSPACE)/qmk_firmware/Makefile \
	  || { echo "qmk_firmware submodule missing. Run: git submodule update --init --recursive" >&2; exit 1; }

keymapviz:
	test -x $(KEYMAPVIZ) || $(MAKE) venv
	$(KEYMAPVIZ) -k sofle -r $(KM_SOFLE)/keymap.c -c $(KM_SOFLE)/keymapviz.toml >/dev/null && rm -f $(KM_SOFLE)/keymap.c.bac

venv:
	python3 -m venv $(VENV) && PIP_NO_CACHE_DIR=1 $(VENV)/bin/pip install -q -r $(QMK_USERSPACE)/requirements-dev.txt

# Convenience only: removes QMK's build directory and scratch files. Everything else under
# build/ (firmware releases, toolchain cache, venv) is disposable and can be deleted by hand,
# except build/readback/ (flash dumps read from the keyboard; cannot be recreated), which
# make clean never touches.
clean:
	rm -rf $(QMK_USERSPACE)/build/qmk $(QMK_USERSPACE)/build/scratch
