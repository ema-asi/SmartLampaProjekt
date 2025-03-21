# Smart Lamp Controller

## Description

This is an Arduino-based project that implements a smart lamp that can be controlled using a sound sensor (clap control) and a light sensor. The project also includes support for Wi-Fi connectivity. 

## Features

- **Clap Control**: Turns the lamp on and off when sound levels exceed a certain threshold.
- **Automatic Brightness Adjustment**: Adjusts the LED brightness based on ambient light levels.
- **Wi-Fi Connectivity**: Allows the device to connect to Wi-Fi for future expansion.
- **Test Mode**: Enables testing of sensors and memory usage.

## Hardware

- Arduino-compatible microcontroller (Arduino Uno Rev4 Wifi)
- KY-037 Condenser Microphone - Sound sensor (analog and digital output)
- LM393 Light Detector - Photoresistor (light sensor)
- LED (lamp controlled by the system)
- Wi-Fi module (supports the WiFiS3 library)
- LCD-display 

## Installation

1. **Connect the components** according to the wiring diagram:
   - Sound sensor (analog) → A1
   - Sound sensor (digital) → D3
   - Light sensor → A0
   - LED → D4
   - LCD-display → rs, en, d4, d5, d6, d7
2. **Upload the code** to your Arduino device via the Arduino IDE.
3. **Create an `arduino_secrets.h` file** in the project folder and fill in the Wi-Fi details:
   ```cpp
   #ifndef ARDUINO_SECRETS_H
   #define ARDUINO_SECRETS_H
   const char SSID[] = "SSID_NAME_HERE";
   const char PASSWORD[] = "WIFI_PASSWORD_HERE";
   #endif
4. Open the Serial Monitor in the Arduino IDE to view sensor data and debug information.

## Usage

### Normal Operation:
- Clap to turn the lamp on/off.
- Brightness adjusts automatically based on ambient light.

### Test Mode:
- Change `TestOn = true;` in the code to activate test mode.
- Tests sensors and displays readings in the Serial Monitor.

## Troubleshooting

### Wi-Fi does not connect:
- Ensure the SSID and password are correct.
- Try reducing `WIFI_RECONNECTION_ATTEMPTS`.

### Clap control does not work:
- Adjust `Sound_Treshold` based on your environment.
- Make sure the sound sensor is properly connected.

### Brightness does not adjust:
- Verify that the light sensor is working by checking the Serial Monitor.
- Adjust `Light_Dark_Value` and `Light_Bright_Value` if needed.

## Future Development
- Remote control via a web page or mobile app.
- Integration with a cloud service for remote management.
- Use of machine learning to improve sound recognition.
- Rörelsedetektering: Utrusta lampan med sensorer som tänder eller släcker ljuset baserat på rörelse i rummet, vilket kan spara energi och öka bekvämligheten.
- Implementera en funktion där lampans färgtemperatur och ljusstyrka automatiskt anpassas under dagen för att efterlikna naturligt ljus.


## Authors
Jessica Sjösten - Maimehs
Oscar Asserlund
Emanuela Asiedu
Chengjun Yan - Chengjun724
Seref




# TaklampaProjekt

Länk till tinkercad:
https://www.tinkercad.com/things/3Xi7Myx2Kyr-smart-lampa?sharecode=aGGOfHm3zqtYueofBSl5g6LcX2ZJUXyLeKuHZmQJwHo
