#include <Arduino.h>
#include "wifi_arduino.h"
#include "clap_detection.h"

#define PhotoResistor_PIN 2    // Will eventually be digital input pin for light-sensor
#define SoundAnalog_PIN A1     // Will eventually be analog input pin for sound-sensor
#define SoundDigital_PIN 3     // Will eventually be digital input pin for sound-sensor
#define LED_PIN 4              // Eventual digital-output pin a light-source
#define Measured_Light_Value 0 // Eventually analog-input from light sensor
#define Sound_Treshold 500                  // Will serve as calibration for our sound-sensor
#define Light_Dark_Value 100                // Sensor value in complete darkness， can changes
#define Light_Bright_Value 800              // Sensor value in bright light, can changes
#define SampleSize 5      // Will serve as calibration for our sound-sensor
#define Sound_Treshold 25 // Will serve as calibration for our sound-sensor
#define ClapWindow 5000   // Will serve as calibration for our sound-sensor

ClapDetection clapdetection(SampleSize, Sound_Treshold, ClapWindow);
bool isLampOn = false;

// Function Declarations:
void Light_AdjustBrightness();
void lightOnClaps();

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
  Serial.print("Light Intensity: ");
  Serial.println(digitalRead(PhotoResistor_PIN));
  Serial.print("Sound Intensity: ");
  Serial.println(analogRead(SoundAnalog_PIN));
  
  Light_AdjustBrightness();
  
  lightOnClaps();
  
  delay(100);
}

// Function Definitions:

void Light_AdjustBrightness()
{
  int brightness = map(analogRead(PhotoResistor_PIN), Light_Dark_Value, Light_Bright_Value, 0, 255); // Converts sensor values to PWM range (0-255), If sensor = 100 → brightness = 0 (LED off),If sensor = 800 → brightness = 255 (max brightness)
  brightness = constrain(brightness, 0, 255);                                                        // Ensure brightness value is valid

  Serial.print("Sensor: ");
  Serial.print(analogRead(PhotoResistor_PIN));
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

void lightOnClaps()
{
  if (clapdetection.detect_claps(SoundAnalog_PIN))
  {
    Serial.println("Beep Boop, you wake the computah!"); // Debugging message
    isLampOn = !isLampOn;
    digitalWrite(LED_PIN, isLampOn);
    delay(5000);
  }
}