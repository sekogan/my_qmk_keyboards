@echo off

set THIS_DIR=%~dp0
set SOURCES=%THIS_DIR%.qmk
set ABORT=exit /b 1

if exist %SOURCES% (
    echo Destroying VM...
    pushd %SOURCES% || %ABORT%
    vagrant destroy -f || %ABORT%
    popd || %ABORT%
    echo.

    echo Removing QMK sources...
    rmdir /s /q %SOURCES% || %ABORT%
    echo.
)

echo SUCCESS!
