# Smart Heating Controller Attachment

This repository contains the design, firmware, and web interface for a **Smart Heating Controller Attachment** that enhances an existing heating system with Wi-Fi functionality. With this attachment, you can control your heating remotely, set timers, and enjoy advanced automation without replacing the original controller.

<p align="center">
  <img src="./img/Module.png" alt="Controller Attachment" width="28%" />
  <img src="./img/ComboDesctopOn.png" alt="Web Interface" width="45%" />
</p>

---

## Features

### 1. **Wi-Fi Control**

<img src="./img/ControlPanelDesctopOn.png" alt="Website Main Page" width="70%" />

- Turn your heating system **ON** or **OFF** remotely via a web interface.
- Adjust settings conveniently without physical access to the controller.

#### Light and Dark Themes, On/Off State, and Mobile/Desktop View

The web interface is responsive and supports:

- **Light and Dark themes** with a toggle switch.

<p align="center">
  <img src="./img/ControlPanelDesctopLightTheme.png" alt="Light Theme" width="45%" />
  <img src="./img/ControlPanelDesctopOff.png" alt="Dark Theme" width="45%" />
</p>

- Visual indication of heating states:
  - **Red**: Heating is ON.
  - **Blue**: Heating is OFF.

<p align="center">
  <img src="./img/ControlPanelDesctopOn.png" alt="Heating On" width="45%" />
  <img src="./img/ControlPanelDesctopOff.png" alt="Heating Off" width="45%" />
</p>

- Optimized views for both **mobile** and **desktop** devices.

<p align="center">
  <img src="./img/ComboDesctopOn.png" alt="Desktop View" width="45%" />
  <img src="./img/ComboPhone.png" alt="Mobile View" width="16%" />
</p>

Try a preview of the website here: [Website Preview](https://htmlpreview.github.io/?https://raw.githubusercontent.com/I2oman/Smart_Heating/master/Smart_Heating_Website/controlpanel.html)

---

### 2. **Flexible Timer Configuration**

<p align="center">
  <img src="./img/TimersDesctopOn.png" alt="Website Timer Page Dark" width="45%" />
  <img src="./img/TimersPanelDesctopLightTheme.png" alt="Website Timer Page Light" width="45.4%" />
</p>

- Set timers for specific days or ranges of days, such as:
  - **Monday only**
  - **Monday to Friday**
  - **Saturday to Sunday**
  - Or any other custom day combinations.
- Configure hour-specific timers within each day or day range:
  - Example: From **8:00 AM to 10:00 AM** and **6:00 PM to 10:00 PM**.
- Timers are stored in the controller’s memory:
  - Settings remain intact even after a restart or power outage.

---

### 3. **Battery Backup**

<img src="./img/Battery.png" alt="Battery Backup" width="70%" />

- Includes a **24–26 hour battery backup**:
  - Can be replaced daily with a fully charged one.
  - Supports on-site charging for uninterrupted operation.

---

## Components

1. **Firmware:**

   - Written in Arduino-compatible C++ for reliability and ease of customization.

2. **Hardware:**

   - Designed as an add-on to fit existing heating controllers.
   - Equipped with a robust battery system for power resilience.

3. **Web Interface:**
   - A user-friendly dashboard to manage heating, timers, and automation settings.

---

## How It Works

### Initial Setup

In the `.ino` firmware file, a maintenance mode allows uploading HTML/CSS/JS files for the web interface to the ESP32.

1. Ensure the `#define MAINTENANCE` line is uncommented in the firmware.
2. Upload the firmware through the Arduino IDE.
3. Open the serial terminal and wait for the message: `"Done uploading!!!"`.
4. After successful upload, comment out the `#define MAINTENANCE` line and re-upload the firmware for normal operation.

<p align="center">
  <img src="./img/ArduinoIDEMaintenance.png" alt="Arduino IDE Maintenance Mode" width="70%" />
</p>

---

### Normal Operation

- **Installation:** Attach the module to your existing heating controller.
- **Setup:** Connect to the Wi-Fi network and access the web interface.
- **Operation:** Configure timers, toggle heating, and let the system handle the rest.

---

## Photos

### Heating Controller with Module Detached

<img src="./img/Controller.png" alt="Controller Module Detached" width="70%" />

### Module and Battery Photo

<img src="./img/Module_and_battery.png" alt="Module and Battery" width="70%" />

### Module Rear View

<img src="./img/Module_rare.png" alt="Module Rear View" width="70%" />

### Website

<p align="center">
  <img src="./img/ComboDesctopOn.png" alt="Desktop dark theme on" width="45%" />
  <img src="./img/Combo desctopOff.png" alt="Desktop dark theme off" width="45%" />
</p>
<p align="center">
  <img src="./img/Combo desctopLight.png" alt="Desktop light theme combined" width="45%" />
  <img src="./img/ComboPhone.png" alt="Mobile" width="16%" />
</p>

### Heating Controller with Module Attached

<img src="./img/Controller_with_module.png" alt="Controller with Module Attached" width="70%" />
