#include <Arduino.h>
#include <WiFiS3.h>

#include "arduino_secrets.h"

#define PhotoResistor_PIN A0                // Will eventually be digital input pin for light-sensor
#define SoundAnalog_PIN A1                  // Will eventually be analog input pin for sound-sensor
#define SoundDigital_PIN 3                  // Will eventually be digital input pin for sound-sensor
#define LED_PIN 4                           // Eventual digital-output pin a light-source
#define Measured_Light_Value 0              // Eventually analog-input from light sensor
#define Sound_Treshold 500                  // Will serve as calibration for our sound-sensor
#define Light_Dark_Value 100                // Sensor value in complete darkness， can changes
#define Light_Bright_Value 800              // Sensor value in bright light, can changes
#define WIFI_RECONNECTION_ATTEMPTS 10       // Used in connectToWifi()
#define WIFI_TIME_BETWEEN_RECONNECTION 1000 // Defined in milliseconds. Used in connectToWifi()

bool isLampOn = false;

// YOU NEED THIS IN A SEPERATE "arduino_secrets.h" FILE OR IT WILL NOT WORK/COMPILE
//
// -----  WiFi Settings  ----
// const char SSID[] = "SSID_NAME_HERE";           // your WiFi network name
// const char PASSWORD[] = "WIFI_PASSWORD_HERE";   // your WiFi password

// Function Declarations:

void Sound_toggleLampOnClap();
void ConnectToWifi();
void Light_AdjustBrightness();

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PhotoResistor_PIN, INPUT);
  pinMode(SoundAnalog_PIN, INPUT);
  pinMode(SoundDigital_PIN, INPUT);
}

void loop()
{
  // Serial.print("Light Intensity: ");
  // Serial.println(digitalRead(PhotoResistor_PIN));
  Serial.print("Sound Intensity: ");
  Serial.println(analogRead(SoundAnalog_PIN));

  Sound_toggleLampOnClap();
  delay(100);
  Light_AdjustBrightness();
}

// Function Definitions:

void Sound_toggleLampOnClap()
{
  if (analogRead(SoundAnalog_PIN) < Sound_Treshold)
  {
    isLampOn = !isLampOn;
    digitalWrite(LED_PIN, isLampOn);
  }
  else
  {
    return;
  }
}

void Light_AdjustBrightness()
{
  int sensorValue = analogRead(PhotoResistor_PIN);

  int brightness = map(sensorValue, Light_Dark_Value, Light_Bright_Value, 0, 255); // Converts sensor values to PWM range (0-255), If sensor = 100 → brightness = 0 (LED off),If sensor = 800 → brightness = 255 (max brightness)
  brightness = constrain(brightness, 0, 255);                                      // Ensure brightness value is valid

  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print(" → Brightness: ");
  Serial.println(brightness);

  if (isLampOn)
  {
    analogWrite(LED_PIN, brightness); // Set LED brightness, send PWM signal to LED
  }
  else
  {
    analogWrite(LED_PIN, 0); // Set LED brightness, send PWM signal to LED
  }
}

/**
 * @brief Meant to be used in setup() and does NOT handle errors
 */
void ConnectToWifi()
{
  WiFi.disconnect(); // Ensures a clean start
  WiFi.end();        // Hard reset of the WiFi module
  delay(1000);       // Give it time(ms) to reset

  int numberOfAttempts = 0;

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Attempting to connect to Wi-Fi.");
    WiFi.begin(SSID, PASSWORD);
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Connected!");
      numberOfAttempts = 0;
      return;
    }
    Serial.print("Attempt: ");
    Serial.print(numberOfAttempts + 1);
    Serial.print("\n");

    Serial.print("Wi-Fi Status Code: ");
    Serial.print(WiFi.status());
    Serial.print("\n");

    if (numberOfAttempts >= WIFI_RECONNECTION_ATTEMPTS)
    {
      Serial.println("Connection failed.");
      return;
    }

    numberOfAttempts++;
    delay(WIFI_TIME_BETWEEN_RECONNECTION);
  }
}
