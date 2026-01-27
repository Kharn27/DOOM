# DOOM ESP32 - WAD storage

## PlatformIO (VS Code)

This folder is structured as an ESP-IDF PlatformIO project. Open the `ESP32`
directory in VS Code and use the PlatformIO extension to build/flash.

Place the `doom1.wad` file at one of the following absolute paths, depending on
the filesystem you mount in ESP-IDF:

- SPIFFS: `/spiffs/doom1.wad`
- SDMMC (VFS FAT on SD card): `/sdcard/doom1.wad`
