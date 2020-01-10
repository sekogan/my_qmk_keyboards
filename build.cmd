@echo off

:: These values must match the outside world
set QMK_REPO=https://github.com/qmk/qmk_firmware
set QMK_REPO_TAG=0.7.106
set BUILD_TARGET=kbdfans/kbd75/rev2:sekogan
set KEYMAP_DIR=keyboards\kbdfans\kbd75\keymaps\sekogan
set FIRMWARE_FILE_NAME=kbdfans_kbd75_rev2_sekogan.hex
set DOCKER_CONTAINER=qmkfm-base_container
set BUILD_COMMAND=/bin/bash -l -c 'cd /vagrant ^&^& make %BUILD_TARGET%'

:: These are internals
set THIS_DIR=%~dp0
set SOURCES_DIR=%THIS_DIR%.qmk
set BINARIES_DIR=%THIS_DIR%firmware
set ABORT=exit /b 1

if not exist %SOURCES_DIR% (
    echo Downloading QMK source tree...
    git clone %QMK_REPO% %SOURCES_DIR% ^
        --branch %QMK_REPO_TAG% --single-branch --recurse-submodules ^
        || %ABORT%
    echo.
)

echo Copying keymap into QMK source tree...
xcopy /E /I /Y keymap %SOURCES_DIR%\%KEYMAP_DIR% || %ABORT%
echo.

pushd %SOURCES_DIR% || %ABORT%

echo Starting VM...
vagrant up || %ABORT%
echo.

echo Starting docker container...
vagrant ssh -c "docker restart %DOCKER_CONTAINER%" || %ABORT%
echo.

echo Building...
vagrant ssh -c "docker exec -it %DOCKER_CONTAINER% %BUILD_COMMAND%" || %ABORT%
echo.

echo Copying results to %BINARIES_DIR% ...
xcopy /I /Y %FIRMWARE_FILE_NAME% %BINARIES_DIR%\ || %ABORT%
echo.

echo.
echo.
echo SUCCESS!
echo.
echo.
echo Please find the firmware in %BINARIES_DIR%
echo.
echo How to flash:
echo.
echo - Start tools\qmk_toolbox.exe. Allow it to install drivers.
echo - Switch the keyboard into bootloader mode:
echo   - option A: press Caps Lock + Escape + Backspace,
echo   - option B: disconnect from USB and connect again while pressing
echo     Backspace. Unlike the first option also clears EEPROM.
echo   A message should appear in qmk_toolbox saying that device is connected.
echo - Select the .hex file with the firmware in qmk_toolbox
echo   and press Flash button.
echo.
