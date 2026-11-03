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

%:
	+$(MAKE) -C $(QMK_FIRMWARE_ROOT) $(MAKECMDGOALS) QMK_USERSPACE=$(QMK_USERSPACE)
