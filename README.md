# ESP8266 Step Tracker with OLED Display and Web Interface

This project is a modular step-tracking system built on the ESP8266 platform. It uses an OLED display to show real-time step information and time, and a web interface to view and modify goals. The system is designed to be modular, with separate managers for EEPROM storage, steps tracking, OLED display, WiFi, and web server functionality.

---

## Features

- **Step Tracking:** Tracks daily and global steps using a step manager.
- **OLED Display:** Shows current steps, global steps, daily goals, and current time.
- **Web Interface:** Displays steps and goals with an interactive page. Users can update the daily goal without refreshing the page.
- **EEPROM Storage:** Saves global steps and daily goals persistently between reboots.
- **Multiple Screens on OLED:** Supports multiple screens and can switch between them using a button.
- **Time Synchronization:** NTP-based time keeping for accurate current time display.
- **Modular Codebase:** Organized in separate files for easier maintenance and extension.

---

## Project Structure

```
src/
├─ main.cpp             # Main program, initializes all modules
├─ OLEDManager.h/cpp    # Handles the OLED display
├─ StepsManager.h/cpp   # Tracks steps and daily goals
├─ EEPROMManager.h/cpp  # Handles persistent storage
├─ WebServer.h/cpp      # Handles the web interface and endpoints
├─ WiFiManager.h/cpp    # Handles WiFi connection
├─ utils.h/cpp          # Utility functions, e.g., time formatting
```

---

## Libraries Used

- [U8g2lib](https://github.com/olikraus/U8g2) – Advanced OLED UI library.
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) – Asynchronous web server.
- [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) – TCP library for ESP8266.
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) – Basic OLED driver.
- [EEPROM](https://www.arduino.cc/en/Reference/EEPROM) – Non-volatile storage.

---

## Usage

### 1. Connect to WiFi

Configure your WiFi SSID and password in `main.cpp`:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

### 2. Build and Upload

Use PlatformIO to build and upload:

```bash
platformio run --target upload --upload-port COM3
```

### 3. Using the OLED Display

- The display shows multiple screens:
  - Current steps and goal.
  - Distance traveled in km (based on step length).
  - Global steps.
- Use a button connected to `BUTTON_PIN` (default `D3`) to switch screens.

### 4. Web Interface

- Access the ESP8266 IP in a browser.
- View:
  - Current time.
  - Steps today and global steps.
  - Daily goal.
- Update the daily goal without refreshing the page.

### 5. Persistent Storage

- Global steps and daily goal are automatically stored in EEPROM.
- Saves automatically every minute.


---

## Future Enhancements

- Integrate a gyroscope (MPU6050) for more accurate step counting.
- Implement a compass or direction indicator.
- Save more structured data (e.g., JSON) in EEPROM for easier extension.
- Add CSS/JS enhancements to the web page for better UX.
- Implement distance tracking in km or miles based on step length.

---

## Notes

- Step length can be customized in `StepsManager` to calculate distance accurately.
- OLED rendering uses `U8g2` for cleaner graphics and better font control.
- Web interface updates via AJAX to allow real-time monitoring without reloading the page.

---

## License

This project is open-source. You may use and modify it freely under the MIT License.

