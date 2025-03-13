#include <Arduino.h>
#include <WiFiS3.h>

#include "arduino_secrets.h"

#define PhotoResistor_PIN 2                 // Will eventually be digital input pin for light-sensor
#define SoundAnalog_PIN A1                  // Will eventually be analog input pin for sound-sensor
#define SoundDigital_PIN 3                  // Will eventually be digital input pin for sound-sensor
#define LED_PIN 4                           // Eventual digital-output pin a light-source
#define Measured_Light_Value 0              // Eventually analog-input from light sensor
#define Sound_Treshold 500                  // Will serve as calibration for our sound-sensor
#define WIFI_RECONNECTION_ATTEMPTS 10       // Used in connectToWifi()
#define WIFI_TIME_BETWEEN_RECONNECTION 1000 // Defined in milliseconds. Used in connectToWifi()

// YOU NEED THIS IN A SEPERATE "arduino_secrets.h" FILE OR IT WILL NOT WORK/COMPILE
//
// -----  WiFi Settings  ----
// const char SSID[] = "SSID_NAME_HERE";           // your WiFi network name
// const char PASSWORD[] = "WIFI_PASSWORD_HERE";   // your WiFi password

// Function Declarations:

void ConnectToWifi();

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
  Serial.print("Light Intensity: ");
  Serial.println(digitalRead(PhotoResistor_PIN));

  if (digitalRead(PhotoResistor_PIN) == HIGH)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }

  Serial.print("Sound Intensity: ");
  Serial.println(analogRead(SoundAnalog_PIN));
  Serial.println(digitalRead(SoundDigital_PIN));

  if (digitalRead(SoundDigital_PIN) == HIGH)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }

  // int soundLevel{}; // int SoundLevel = 0;

  // soundLevel = analogRead(SoundAnalog_PIN);
  // Serial.println(soundLevel);

  // delay(100);
}

// Function Definitions:

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