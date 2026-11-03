# Firmware for Kbd75 v3.1

## How to build and flash on Fedora linux

Download and compile the firmware:

```bash
toolbox create qmk_build
toolbox enter qmk_build
pip install qmk --user --upgrade
```

```bash
cd ~/projects
qmk clone qmk/qmk_firmware.git
git clone https://github.com/sekogan/my_qmk_keyboards.git
```

```bash
QMK_HOME=./qmk_firmware qmk setup
```

Answer yes to all questions.

```bash
cp -r my_qmk_keyboards/kbd75hs/keymap/. qmk_firmware/keyboards/kbdfans/kbd75hs/keymaps/sekogan
QMK_HOME=./qmk_firmware qmk compile -kb kbdfans/kbd75hs -km sekogan
```

Find and connect a second keyboard.

On the host outside the container:

Switch the keyboard into the bootloader mode (CapsLock + Esc + F12).
Use `lsusb` command (from usbutils package) to verify that the device is connected:

```
Bus 003 Device 011: ID 03eb:2ff4 Atmel Corp. atmega32u4 DFU bootloader
```

Flash the firmware:

```bash
cd ~/projects
sudo dnf install dfu-programmer
sudo dfu-programmer atmega32u4 erase
sudo dfu-programmer atmega32u4 flash qmk_firmware/.build/kbdfans_kbd75hs_sekogan.hex
sudo dfu-programmer atmega32u4 launch
```
