.SILENT:

MAKEFLAGS += --no-print-directory

QMK_USERSPACE := $(patsubst %/,%,$(dir $(shell realpath "$(lastword $(MAKEFILE_LIST))")))
ifeq ($(QMK_USERSPACE),)
    QMK_USERSPACE := $(shell pwd)
endif

# qmk_firmware is the pinned submodule. (The upstream template asks a host `qmk` CLI
# via `qmk env QMK_FIRMWARE`; this repo has no host qmk, the toolchain is in the image.)
QMK_FIRMWARE_ROOT := $(QMK_USERSPACE)/qmk_firmware
ifeq ($(wildcard $(QMK_FIRMWARE_ROOT)/Makefile),)
    $(error qmk_firmware submodule missing. Run: git submodule update --init --recursive)
endif

# keymapviz only rewrites the ASCII-art comments in keymap.c, so it runs on the
# host from a venv in build/venv (`make venv` creates it; build/ is disposable).
VENV := $(QMK_USERSPACE)/build/venv
KEYMAPVIZ ?= $(VENV)/bin/keymapviz
KM_SOFLE := $(QMK_USERSPACE)/keyboards/sofle/keymaps/sekogan

.PHONY: keymapviz venv clean
keymapviz:
	test -x $(KEYMAPVIZ) || $(MAKE) venv
	$(KEYMAPVIZ) -k sofle -r $(KM_SOFLE)/keymap.c -c $(KM_SOFLE)/keymapviz.toml >/dev/null && rm -f $(KM_SOFLE)/keymap.c.bac

venv:
	python3 -m venv $(VENV) && PIP_NO_CACHE_DIR=1 $(VENV)/bin/pip install -q -r $(QMK_USERSPACE)/requirements-dev.txt

# Convenience only: removes QMK's build directory and scratch files. Everything under
# build/ (firmware archives, toolchain cache, venv too) is disposable and can be deleted.
clean:
	rm -rf $(QMK_USERSPACE)/build/qmk $(QMK_USERSPACE)/build/scratch

%:
	+$(MAKE) -C $(QMK_FIRMWARE_ROOT) $(MAKECMDGOALS) QMK_USERSPACE=$(QMK_USERSPACE)
