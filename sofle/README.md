# Firmware for Sofle keyboard

Derived from the [default keymap](https://github.com/qmk/qmk_firmware/tree/master/keyboards/sofle/keymaps/default).

![My Sofle!](sofle1000.jpg)

## How to flash

The following recipe is for Fedora 38.

```bash
mkdir qmk
cd qmk
git clone https://github.com/sekogan/my_qmk_keyboards
git clone https://github.com/qmk/qmk_firmware
cd qmk_firmware
git checkout 0.21.3  # version 0.15.11 also works, see notes below
```

NOTE: more recent versions of QMK are not compatible with the language feature.
It was broken somewhere between 0.15.11 and 0.16.9.
UPD: 0.21.3 seems okay at least on Linux.

```bash
cd ..
virtualenv venv
. venv/bin/activate
pip3 install qmk
pip3 install keymapviz
QMK_HOME=./qmk_firmware qmk setup
```

Answer yes to all questions.

Copy udev rules to `/etc` if suggested by the qmk setup or
if the `make flash` command leads to an error `Waiting for /dev/ttyACM0 to become writable`:

```bash
sudo cp qmk_firmware/util/udev/50-qmk.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
```

```bash
cd my_qmk_keyboards/sofle
make flash
```
