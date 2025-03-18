#include <Arduino.h>
#include "wifi_arduino.h"
#include "clap_detection.h"
#include <LiquidCrystal.h>
#include <array>

#define PhotoResistor_PIN A0   // Analog input pin for light sensor
#define SoundAnalog_PIN A1     // Analog input pin for sound sensor
#define SoundDigital_PIN 3     // Digital input pin for sound sensor (will eventually be removed)
#define LED_PIN 4             // PWM-output pin for our light source
#define Measured_Light_Value 0 // Eventually analog input for light sensor
#define Sound_Treshold 500     // Will serve as calibration for our sound sensor
#define Light_Dark_Value 100   // Sensor value in complete darkness
#define Light_Bright_Value 800 // Sensor value in bright light
// Below works as calibration for our ClapDetection class
#define SampleSize 5      
#define Sound_Treshold 20
#define ClapWindow 5000

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
ClapDetection clapdetection(SampleSize, Sound_Treshold, ClapWindow);
bool isLampOn = true;
int brightness = 0;

// Function Declarations:

int light_AdjustBrightness();
void lightOnClaps();
void setBrightness();
void reconnectToWiFi();

void setup()
{
    Serial.begin(115200);
    lcd.begin(16, 2); // Sets up the display, defining number or rows(Y) and columns(X)

    pinMode(LED_PIN, OUTPUT);
    pinMode(PhotoResistor_PIN, INPUT);
    pinMode(SoundAnalog_PIN, INPUT);
    pinMode(SoundDigital_PIN, INPUT);

    ConnectToWifi();
}

void loop()
{
    reconnectToWiFi();
    
    Serial.print("Light Intensity: ");
    Serial.println(digitalRead(PhotoResistor_PIN));
    Serial.print("Sound Intensity: ");
    Serial.println(analogRead(SoundAnalog_PIN));

    brightness = light_AdjustBrightness();

    lightOnClaps();
    setBrightness();

    // Example usage of LCD-functionality:
    lcd.print("Hello World!");

    delay(100);
}

void reconnectToWiFi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ConnectToWifi();
    }
}

int light_AdjustBrightness()
{
    // arduino::map()
    // Re-maps a number from one range to another. That is, a value of fromLow would get mapped to toLow, a value of fromHigh to toHigh, values in-between to values in-between, etc.
    // Does not constrain values to within the range, because out-of-range values are sometimes intended and useful. The constrain() function may be used either before or after this function, if limits to the ranges are desired.
    // Note that the "lower bounds" of either range may be larger or smaller than the "upper bounds" so the map() function may be used to reverse a range of numbers, for example
    // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // ardunio::constrain()
    // x: the number to constrain Allowed data types: all data types
    // a: the lower end of the range. Allowed data types: all data types
    // b: the upper end of the range. Allowed data types: all data types
    // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    int brightnessLevel = map(analogRead(PhotoResistor_PIN), Light_Dark_Value, Light_Bright_Value, 0, 255); // Converts sensor values to PWM range (0-255), If sensor = 100 → brightness = 0 (LED off),If sensor = 800 → brightness = 255 (max brightness)
    return constrain(brightnessLevel, 0, 255);

    analogWrite(LED_PIN, 50);
}

// Proposed replacement for above code
int lightSensorAverageReading()
{
    std::array<int, 5> brightnessValues{};
    int sensor_input{};
    int8_t index = 0;

    if (index > 4)
    {
        index = 0;
    }

    sensor_input = analogRead(PhotoResistor_PIN);
    if (sensor_input > 500)
    {
        brightnessValues[index] = 500;
    }
    else if (sensor_input < 0)
    {
        brightnessValues[index] = 0;
    }
    index++;

    int sum{};
    for (int i = 0; i < 4; i++)
    {
        sum += brightnessValues[i];
    }
    return sum;
}

void lightOnClaps()
{
  if (clapdetection.detect_claps(SoundAnalog_PIN))
  {
    Serial.println("Clapdetection says: Beep Boop, you wake the computah!"); // Debugging message
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
    // analogWrite(LED_PIN, lightSensorAverageReading());  // Proposed solution
  }
  else
  {
    analogWrite(LED_PIN, 0); // Set LED brightness, send PWM signal to LED
  }
}