#include <Arduino.h>
#include "wifi_arduino.h"
#include "clap_detection.h"

#define PhotoResistor_PIN A0   // Analog input pin for light sensor
#define SoundAnalog_PIN A1     // Analog input pin for sound sensor
#define SoundDigital_PIN 3     // Digital input pin for sound sensor (will eventually be removed)
#define LED_PIN 4              // PWM-output pin for our light source
#define Measured_Light_Value 0 // Eventually analog-input from light sensor
#define Sound_Treshold 500     // Will serve as calibration for our sound-sensor

#define Light_Dark_Value 100   // Sensor value in complete darkness
#define Light_Bright_Value 800 // Sensor value in bright light

#define SampleSize 5      // Will serve as calibration for our sound-sensor
#define Sound_Treshold 20 // Will serve as calibration for our sound-sensor
#define ClapWindow 5000   // Will serve as calibration for our sound-sensor

ClapDetection clapdetection(SampleSize, Sound_Treshold, ClapWindow);
bool isLampOn = true;
int brightness = 0;

// Function Declarations:

int light_AdjustBrightness();
void lightOnClaps();
void setBrightness();

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

  brightness = light_AdjustBrightness();

  lightOnClaps();
  setBrightness();

  delay(100);
}

// TODO: check if map is necessary
int light_AdjustBrightness()
{
  int brightnessLevel = map(analogRead(PhotoResistor_PIN), Light_Dark_Value, Light_Bright_Value, 0, 255); // Converts sensor values to PWM range (0-255), If sensor = 100 → brightness = 0 (LED off),If sensor = 800 → brightness = 255 (max brightness)
  return constrain(brightnessLevel, 0, 255);

  analogWrite(LED_PIN, 50);
}

void lightOnClaps()
{
  if (clapdetection.detect_claps(SoundAnalog_PIN))
  {
    Serial.println("Beep Boop, you wake the computah!"); // Debugging message
    isLampOn = !isLampOn;
    setBrightness(); // Make sure the lamp is on/off without delay
    delay(5000);
  }
}

void setBrightness()
{
  if (isLampOn)
  {
    analogWrite(LED_PIN, brightness); // Set LED brightness, send PWM signal to LED
  }
  else
  {
    analogWrite(LED_PIN, 0); // Set LED brightness, send PWM signal to LED
  }
}