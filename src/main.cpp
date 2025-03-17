#include <Arduino.h>
#include "wifi_arduino.h"
#include "clap_detection.h"

#define PhotoResistor_PIN 2    // Will eventually be digital input pin for light-sensor
#define SoundAnalog_PIN A1     // Will eventually be analog input pin for sound-sensor
#define SoundDigital_PIN 3     // Will eventually be digital input pin for sound-sensor
#define LED_PIN 4              // Eventual digital-output pin a light-source
#define Measured_Light_Value 0 // Eventually analog-input from light sensor

#define SampleSize 5      // Will serve as calibration for our sound-sensor
#define Sound_Treshold 25 // Will serve as calibration for our sound-sensor
#define ClapWindow 5000   // Will serve as calibration for our sound-sensor
ClapDetection clapdetection(SampleSize, Sound_Treshold, ClapWindow);

// Function Declarations:

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PhotoResistor_PIN, INPUT);
  pinMode(SoundAnalog_PIN, INPUT);
  pinMode(SoundDigital_PIN, INPUT);
  ConnectToWifi();
}

void loop()
{
  // Serial.print("Light Intensity: ");
  // Serial.println(digitalRead(PhotoResistor_PIN));
  Serial.print("Sound Intensity: ");
  Serial.println(analogRead(SoundAnalog_PIN));

  clapdetection.detect_claps(SoundAnalog_PIN, LED_PIN);

  // digitalWrite(LED_PIN, HIGH);
  // if (digitalRead(PhotoResistor_PIN) == HIGH)
  // {
  //   digitalWrite(LED_PIN, HIGH);
  // }
  // else
  // {
  //   digitalWrite(LED_PIN, LOW);
  // }

  delay(100);
}