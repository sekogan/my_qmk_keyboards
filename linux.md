# Flashing on Linux

Download and compile the firmware:

```
cd ~/projects
toolbox create qmk_build
toolbox enter qmk_build
pip install --user qmk
qmk clone sekogan/qmk_firmware.git

# Answer yes to all questions
QMK_HOME=./qmk_firmware qmk setup

git clone https://github.com/sekogan/kbd75v2.git
cp -r kbd75v2/keymap/. qmk_firmware/keyboards/kbdfans/kbd75/keymaps/sekogan
qmk compile -kb kbdfans/kbd75/rev2 -km sekogan
```

Find and connect a second keyboard.

On the host outside the container:

Switch the keyboard into the bootloader mode (CapsLock + Esc + F12).
Use `lsusb` command (from usbutils package) to verify that the device is connected:

```
Bus 003 Device 011: ID 03eb:2ff4 Atmel Corp. atmega32u4 DFU bootloader
```

Flash the firmware:

```
cd ~/projects
sudo dnf install dfu-programmer
sudo dfu-programmer atmega32u4 erase
sudo dfu-programmer atmega32u4 flash qmk_firmware/.build/kbdfans_kbd75_rev2_sekogan.hex
sudo dfu-programmer atmega32u4 launch
```
