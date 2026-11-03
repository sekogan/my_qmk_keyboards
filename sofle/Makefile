.PHONY: all clean keymapviz sync

QMK_DIR=../../qmk_firmware
KEYMAP=sekogan

all: build

keymapviz: keymap/keymap.c keymap/keymapviz.toml
	keymapviz -k sofle -r keymap/keymap.c -c keymap/keymapviz.toml
	rm -f keymap/keymap.c.bac

sync: keymapviz
	rm -rf $(QMK_DIR)/keyboards/sofle/keymaps/$(KEYMAP)
	cp -a keymap/. $(QMK_DIR)/keyboards/sofle/keymaps/$(KEYMAP)

build: sync
	cd $(QMK_DIR) && \
	qmk compile -kb sofle/rev1 -km $(KEYMAP)

flash: sync
	cd $(QMK_DIR) && \
	qmk flash -kb sofle/rev1 -km $(KEYMAP)

clean:
	rm -rf $(QMK_DIR)/keyboards/sofle/keymaps/$(KEYMAP)
	cd $(QMK_DIR) && \
	qmk clean
