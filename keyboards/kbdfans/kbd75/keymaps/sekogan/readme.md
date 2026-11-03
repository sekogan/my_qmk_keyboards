# Firmware for Kbd75 v2

This is a 83-key layout.
The goal is to make everyday coding and debugging simple and comfortable.

Features:

- Large **[Shift]**, right **[Control]** and **[Alt]** keys.
- Almost all TKL keys are on the base layer. In particular all keys from
  the navigation cluster.
- **[Delete]** is in the top right corner, easy to find.
- **[Fn]** is a second function of **[Caps Lock]** key. Doesn't have a dedicated **[Fn]** key.
- Keyboard settings moved to a special layer that is not so easy to access
  (**[Caps Lock + Escape]**). This was done on purpose to prevent accidental key
  presses. **[Caps Lock + Escape + Backspace]** switches the keyboard into
  bootloader mode.
- Bootmagic is configured in "lite" mode. Press **[Backspace]** while connecting
  the keyboard to USB to reset EEPROM and jump into bootloader.
  Note that **[Caps Lock + Escape + Backspace]** does not reset EEPROM.

Hardware documentation: [docs/kbd75v2](../../../../../docs/kbd75v2/).

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
cp -r my_qmk_keyboards/kbd75v2/keymap/. qmk_firmware/keyboards/kbdfans/kbd75/keymaps/sekogan
QMK_HOME=./qmk_firmware qmk compile -kb kbdfans/kbd75/rev2 -km sekogan
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
sudo dfu-programmer atmega32u4 flash qmk_firmware/.build/kbdfans_kbd75_rev2_sekogan.hex
sudo dfu-programmer atmega32u4 launch
```
