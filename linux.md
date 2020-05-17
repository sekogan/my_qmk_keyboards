# Flashing on Linux

Download and compile the firmware:

```
pip3 install --user qmk
cd ~/projects
qmk clone sekogan/qmk_firmware.git
QMK_HOME=./qmk_firmware qmk setup
git clone https://github.com/sekogan/kbd75v2.git
cp -r kbd75v2/keymap/. qmk_firmware/keyboards/kbdfans/kbd75/keymaps/sekogan
qmk compile -kb kbdfans/kbd75 -km sekogan
```

Switch the keyboard into the bootloader mode (CapsLock + Esc + Backspace).
Use `lsusb` command to verify that the device is connected:

```
Bus 003 Device 011: ID 03eb:2ff4 Atmel Corp. atmega32u4 DFU bootloader
```

Flash the firmware:

```
sudo dfu-programmer atmega32u4 erase
sudo dfu-programmer atmega32u4 flash qmk_firmware/.build/kbdfans_kbd75_rev1_sekogan.hex
sudo dfu-programmer atmega32u4 launch
```
