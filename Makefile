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
# host from a repo-local venv:
#   python3 -m venv .venv && PIP_NO_CACHE_DIR=1 .venv/bin/pip install -r requirements-dev.txt
KEYMAPVIZ ?= $(QMK_USERSPACE)/.venv/bin/keymapviz
KM_SOFLE := $(QMK_USERSPACE)/keyboards/sofle/keymaps/sekogan

.PHONY: keymapviz
keymapviz:
	$(KEYMAPVIZ) -k sofle -r $(KM_SOFLE)/keymap.c -c $(KM_SOFLE)/keymapviz.toml >/dev/null && rm -f $(KM_SOFLE)/keymap.c.bac

%:
	+$(MAKE) -C $(QMK_FIRMWARE_ROOT) $(MAKECMDGOALS) QMK_USERSPACE=$(QMK_USERSPACE)
