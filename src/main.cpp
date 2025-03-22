#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <array>
#include "Adafruit_LTR329_LTR303.h"
#include "wifi_arduino.h"
#include "clap_detection.h"

#define PhotoResistor_PIN A0   // Analog input pin for light sensor
#define SoundAnalog_PIN A1     // Analog input pin for sound sensor
#define SoundDigital_PIN 7     // Digital input pin for sound sensor (will eventually be removed)
#define LED_PIN 6              // PWM-output pin for our light source
#define Measured_Light_Value 0 // Eventually analog input for light sensor
#define Sound_Treshold 500     // Will serve as calibration for our sound sensor
#define Light_Dark_Value 115   // Sensor value in complete darkness
#define Light_Bright_Value 870 // Sensor value in bright light

// Below works as calibration for our ClapDetection class
#define SampleSize 5
#define Sound_Treshold 20
#define ClapWindow 5000

constexpr bool WiFiOn = false;                                      // This turns on project wide usage of WiFi
constexpr bool DebugOn = false;                                     // This turns on project wide debugging prints
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
extern "C" char *sbrk(int i);

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
ClapDetection clapdetection(SampleSize, Sound_Treshold, ClapWindow);
Adafruit_LTR303 ltr = Adafruit_LTR303();

bool isLampOn = false;
int brightness = 0;

// Function Declarations:

void initializeLTR303();
void reconnectToWiFi();
int getFreeMemory();
void printFreeMemory();
int lightSensorAverageReading();
void detectClaps();
void lampActivation();

void setup()
{
    Serial.begin(115200);                           // Initialize the serial COM-port on the Arduino
    lcd.begin(16, 2);                               // Sets up the display, defining number or rows(Y) and columns(X)

    initializeLTR303();

    pinMode(LED_PIN, OUTPUT);
    pinMode(PhotoResistor_PIN, INPUT);
    pinMode(SoundAnalog_PIN, INPUT);
    pinMode(SoundDigital_PIN, INPUT);

    // WiFi Related Setup
    if (WiFiOn)
    {
        lcd.println("Connecting to WiFi!");
        ConnectToWifi();
        lcd.clear();
        lcd.println("Established connection!");
    }
}

void loop()
{
    // This all belongs to LTR-303 testing
    bool valid;
    uint16_t visible_plus_ir, infrared;

    if (ltr.newDataAvailable())
    {
        valid = ltr.readBothChannels(visible_plus_ir, infrared);
        if (valid)
        {
            Serial.print("CH0 Visible Light + IR: ");
            Serial.println(visible_plus_ir);
            Serial.print("CH1 Infrared: ");
            Serial.println(infrared);
        }
    }
    // LTR-303 Testing Code Done

    if (WiFiOn)
    {
        reconnectToWiFi();
    }

    // Serial.print("Light Intensity: ");               // Debugging message
    // Serial.println(analogRead(PhotoResistor_PIN));   // Debugging message
    // Serial.print("Sound Intensity: ");               // Debugging message
    // Serial.println(analogRead(SoundAnalog_PIN));     // Debugging message
    // brightness = lightSensorAverageReading();        // Debugging message

    detectClaps();
    lampActivation();

    delay(100);
}

/**
 * @brief Check if WiFi is connected, otherwise calls ConnectToWifi()
 */
void reconnectToWiFi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ConnectToWifi();
    }
}

/**
 * @brief Reads the light sensor and calculates the average brightness value.
 *
 * This function reads the light sensor value, scales it to a PWM range (0-255),
 * and maintains a running average of the last 5 readings to smooth out fluctuations.
 *
 * @return int The average brightness value scaled to the range 0-255.
 */
int lightSensorAverageReading()
{
    bool valid;
    uint16_t visible_plus_ir, infrared, visible;
    const int8_t sizeOfArray = 20;                                          // Size of the array to store brightness values
    static std::array<int, sizeOfArray> brightnessValues{};                 // Array to store the last 5 brightness values
    static int8_t index = 0;                                                // Index to keep track of the current position in the array
    int sum{};                                                              // Variable to store the sum of brightness values

    // int sensor_input = analogRead(PhotoResistor_PIN);                    // Read the current value from the light sensor (Old code we used with photoresistor)

    int sensor_input = ltr.readBothChannels(visible_plus_ir, infrared);     // Read both channels from our new LTR303 sensor
    visible = (visible_plus_ir - infrared);                                 // Calculates visible light by subtracting IR

    if (DebugOn)
    {
        Serial.print("Visible Plus IR: ");                                   // Debugging message
        Serial.println(visible_plus_ir);                                     // Debugging message
        Serial.print("Infrared Light: ");                                    // Debugging message
        Serial.println(infrared);                                            // Debugging message
        Serial.print("Visible Light: ");                                     // Debugging message
        Serial.println(visible);                                             // Debugging message
    }

    // Scale the sensor input to a range of 0-255
    int scaled_input = (sensor_input - Light_Dark_Value) * 255 / (Light_Bright_Value - Light_Dark_Value);

    // Constrain the scaled input to be within the range 0-255
    scaled_input = (scaled_input > 255) ? 255 : (scaled_input < 0) ? 0 : scaled_input;

    brightnessValues[index] = scaled_input;                     // Store the scaled input in the array
    index = (index + 1) % sizeOfArray;                          // Increment the index and wrap around if necessary

    for (const int &elements : brightnessValues)                // Calculate the sum of the brightness values in the array
    {
        sum += elements;
    }

    return (sum / sizeOfArray);                                 // Calculate and then return the average brightness value
}

void lampActivation()
{
    if (isLampOn)
    {
        analogWrite(LED_PIN, lightSensorAverageReading());      // Sets brightness based on 
    }
    else if (!isLampOn)
    {
        analogWrite(LED_PIN, 0);                                // Set LED brightness to 0
    }
    else return;
}

void detectClaps()
{
    if (clapdetection.detect_claps(SoundAnalog_PIN))
    {
    // Serial.println("Clapdetection says: Beep Boop, you wake the computah!"); // Debugging message that can be turned on
    isLampOn = !isLampOn;
    lampActivation();
    // lampActivation(); // Make sure the lamp is on/off without delay
    // delay(5000);     // Commenting out this as we've agreed to not have functions call on delays
    }
}

void initializeLTR303()
{
    ltr.begin();                                    // Inits it
    ltr.setGain(LTR3XX_GAIN_96);                    // Sets gain i,e sensitivity
    ltr.setIntegrationTime(LTR3XX_INTEGTIME_100);   // Sets time we expect a whole signal to be in (i,e one signal is complete in 100 ms)
    ltr.setMeasurementRate(LTR3XX_MEASRATE_200);    // Works with above function, this defines how often we measure and for how long
    ltr.enableInterrupt(false);                     // Disables(Or Enables) hardware interrupt pin (INT)
    ltr.setInterruptPolarity(false);                // Sets polarity of INT
    ltr.setLowThreshold(2000);
    ltr.setHighThreshold(30000);
    ltr.setIntPersistance(4);
}

// This code shows the remaining free memory between the heap and the stack
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
