# 🛜NetReaper
A tool that deauth 2.4GHz Wi-Fi networks via ESP32 on serial console.

# ✨Features
- Minimal Setup.
- Simply controlled by Serial Console.
- LED status for deauth attack status.

# 📦Requirements
- `1` ESP32 Development Board
- `1` Micro-B USB / Type-C USB Cable with data transfer support

# ⚙️Setup Arduino IDE
1. Download `Arduino IDE 2.X.X` from [here](https://www.arduino.cc/en/software/) according to your Operating System.
2. Install it.
3. Go to `File` → `Preferences` → `Additional Boards Manager URLs`.
4. Paste the following link :
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
5. Click on `OK`.
6. Go to `Tools` → `Board` → `Board Manager`.
7. Wait for sometimes and search `esp32` by `Espressif Systems`.
8. Simply install version `2.0.14`.
   - Wait for sometime and after that it is installed.
9. Open file manager of your operating system.
10. Go to the following path according to your operating system.
    - Windows: C:\Users\<YOUR_USERNAME>\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.14
    - macOS: ~/Library/Arduino15/packages/esp32/hardware/esp32\2.0.14
    - Linux: ~/.arduino15/packages/esp32/hardware/esp32\2.0.14
11. There is a file named `platform.txt`.
12. Open it in a text editor.
13. Go to line `45`.
14. Replace the line with the following line :
```
compiler.c.elf.flags.esp32=-zmuldefs -T esp32.rom.redefined.ld -T memory.ld -T sections.ld -T esp32.rom.ld -T esp32.rom.api.ld -T esp32.rom.libgcc.ld -T esp32.rom.newlib-data.ld -T esp32.rom.syscalls.ld -T esp32.peripherals.ld  -mlongcalls -Wno-frame-address -Wl,--cref -Wl,--gc-sections -fno-rtti -fno-lto -Wl,--wrap=esp_log_write -Wl,--wrap=esp_log_writev -Wl,--wrap=log_printf -u ld_include_hli_vectors_bt -u _Z5setupv -u _Z4loopv -u esp_app_desc -u pthread_include_pthread_impl -u pthread_include_pthread_cond_impl -u pthread_include_pthread_local_storage_impl -u pthread_include_pthread_rwlock_impl -u include_esp_phy_override -u ld_include_highint_hdl -u start_app -u start_app_other_cores -u __ubsan_include -Wl,--wrap=longjmp -u __assert_func -u vfs_include_syscalls_impl -Wl,--undefined=uxTopUsedPriority -u app_main -u newlib_include_heap_impl -u newlib_include_syscalls_impl -u newlib_include_pthread_impl -u newlib_include_assert_impl -u __cxa_guard_dummy 
```
15. Save the file and close.
16. Close the Arduino IDE and open again.
17. Done!

# 🔧Install
1. Download or Clone the Repository.
2. Open the folder and just double click on `NetReaper.ino` file.
   - It opens in Arduino IDE.
3. Compile the code.
4. Select the correct board from the `Tools` → `Board` → `esp32`.
   - It is `ESP32 Dev Module`.
6. Select the correct port number of that board.
7. Go to `Tools` and configure the following :
   - `CPU Frequency` to `240MHz (WiFi/BT)`.
   - `Upload Speed` to `921600`.
   - `Partition Scheme` to `Minimal SPIFFS`.
   - `Flash Size` to `4MB (32Mb)`.
8. Upload the code.
9. Unplug and plug the `ESP32` on the PC/Laptop.
10. Done!

# 🔧Install using ESP Web Flasher
1. Open Adafruit ESP Web Flasher from [here](https://adafruit.github.io/Adafruit_WebSerial_ESPTool/).
2. Set the Baud Rate to `115200 Baud`.
3. Connect `ESP32` with a USB cable and then to the PC/Laptop.
4. Press and hold the `BOOT` button.
5. Click on `Connect` button.
6. Select your Device COM Port in the Pop-Up Window.
7. Release the `BOOT` button.
   - When connected successfully, then it show this <img width="386" height="217" alt="Adafruit ESP Web Flasher" src="https://github.com/user-attachments/assets/47b8888e-25d2-4a29-ab6d-25055cf3a033" />
8. Click on `Erase` button.
   - Wait for sometimes to successfully erased.
9. Download `3` files from Releases.
    - The files are :
      1. `bootloader.bin`
      2. `partitions.bin`
      3. `netreaper.bin`
10. Select `bootloader.bin` file with offset `0x1000`.
11. Select `partitions.bin` file with offset `0x8000`.
12. Select `netreaper.bin` file with offset `0x10000`.
13. Click on `Program` button.
    - Wait for sometimes to successfully programmed.
14. Press and release the `BOOT` button.
15. Unplug and plug the `ESP32` on the PC/Laptop.
16. Done! `NetReaper` is ready.

# 📟Using Serial Console
1. Open Serial Console by click [here](https://wirebits.github.io/SerialConsole/).
2. Click on `Connect` button.
   - It show a pop-up window and it has connect devices with port number.
   - Select the correct device.
   - Click on `Connect` button in pop-up window.
3. When it shows `Connected! Go On!` then your ESP32 Development Board is ready to use.
4. Press and release `EN` or `RST` button.
5. Type `help` to get available commands.

# 🚦Indicators
- LED On - Deauth Attack Started
- LED Off - Deauth Attack Stopped
