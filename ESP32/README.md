# DOOM ESP32 - WAD storage

Place the `doom1.wad` file at one of the following absolute paths, depending on
the filesystem you mount in ESP-IDF:

- SPIFFS: `/spiffs/doom1.wad`
- SDMMC (VFS FAT on SD card): `/sdcard/doom1.wad`

## PlatformIO (VS Code)

The ESP32 folder is laid out as a PlatformIO project:

- `platformio.ini` defines the ESP32/ESP-IDF environment.
- `src/` contains the ESP32-specific sources.
- `components/doom` pulls in the original DOOM engine sources from
  `../linuxdoom-1.10` and excludes the PC-only `i_*` implementations.

Open the `ESP32` directory in VS Code with the PlatformIO extension and build
as usual.
