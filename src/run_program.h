#ifndef RUN_PROGRAM_H
#define RUN_PROGRAM_H
#include <Arduino.h>
#include <WiFiS3.h>

#include "arduino_secrets.h"
// YOU NEED THIS IN A SEPERATE "arduino_secrets.h" FILE OR IT WILL NOT WORK/COMPILE
//
// -----  WiFi Settings  ----
// const char SSID[] = "SSID_NAME_HERE";           // your WiFi network name
// const char PASSWORD[] = "WIFI_PASSWORD_HERE";   // your WiFi password

#define LightSensor_PIN 2                          // Will eventually be digital input pin for light sensor
#define SoundAnalog_PIN A1                         // Will eventually be analog input pin for sound sensor
#define SoundDigital_PIN 3                         // Will eventually be digital input pin for sound sensor
#define LED_PIN 4                                  // Eventual digital-output pin a light source
#define Sound_Treshold 500                         // Will serve as calibration for our sound sensor
#define Light_Treshold 500                         // Will serve as calibration for our light sensor
#define WIFI_RECONNECTION_ATTEMPTS 10              // Used in connectToWifi()
#define WIFI_TIME_BETWEEN_RECONNECTION 2000        // Defined in milliseconds. Used in connectToWifi()

extern "C" char *sbrk(int i);

int measuredLightValue = 0;                        // analog input from light sensor
int measuredSoundValue = 0;                        // analog input from sound sensor

bool TestOn = false;
bool runWithWiFi = false;

// Function Declarations:

void ConnectToWifi();
void updateMeasuredLightValue();
void updateMeasuredSoundValue();
void test_LightValue();
void test_SoundValue();
void lampAutomation_clap_activation();
void lampAutomation_consecutive_clap_activation();
int getFreeMemory();
void printFreeMemory();

void setup()
{
  Serial.begin(115200); // Initialize this before anything, this function call is what makes listening to COM-port possible
  Serial.println("Setup started.");

  pinMode(LED_PIN, OUTPUT);
  pinMode(LightSensor_PIN, INPUT);
  pinMode(SoundAnalog_PIN, INPUT);
  pinMode(SoundDigital_PIN, INPUT);

  if (runWithWiFi)
  {
    ConnectToWifi();
  }

  Serial.println("Setup finished.");
}

void loop()
{
  if (!TestOn)
  {
    if (runWithWiFi && WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Wi-fi disconnected.");
      ConnectToWifi();
    }
    lampAutomation_clap_activation();
    lampAutomation_consecutive_clap_activation();
    delay(150);
  }

  if (TestOn)
  {
    if (runWithWiFi)
    {
      ConnectToWifi();
    }
    test_LightValue();
    test_SoundValue();
    printFreeMemory();
    delay(150);
  } 
}

// Function Definitions:

void test_LightValue()
{
    updateMeasuredLightValue();
    Serial.println(measuredLightValue);
}

void test_SoundValue()
{
    updateMeasuredSoundValue();
    Serial.println(measuredSoundValue);
}

void updateMeasuredLightValue()
{
  measuredLightValue = analogRead(LightSensor_PIN);
}

void updateMeasuredSoundValue()
{
  measuredSoundValue = analogRead(SoundAnalog_PIN);
}

void lampAutomation_clap_activation()
{
  if (analogRead(LightSensor_PIN) <= Light_Treshold)
  {
    if (analogRead(SoundAnalog_PIN) > Sound_Treshold)
    {
      digitalWrite(LED_PIN, HIGH);
    }
    else
    {
      return;
    }
  }
  else
  {
    return;
  }
}

void lampAutomation_consecutive_clap_activation()
{
  static int startTime{};
  static int elapsedTime{};
  static int detectionCount = 0;

  startTime = millis();

  if (analogRead(LightSensor_PIN) <= Light_Treshold)
  {
    if (analogRead(SoundAnalog_PIN) > Sound_Treshold)
    {
      ++detectionCount;
      if (detectionCount == 2)
      {
        digitalWrite(LED_PIN, HIGH);
        detectionCount = 0;
      }
    }
    else
    {
      return;
    }
  }
  else
  {
    return;
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

int getFreeMemory() 
{
  char top;
  return &top - sbrk(0);
}

void printFreeMemory() 
{
  Serial.print("Free Memory: ");
  Serial.print(getFreeMemory());
  Serial.println(" Bytes.");
}


#endif // RUN_PROGRAM_H