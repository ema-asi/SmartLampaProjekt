int lightSensorPin = A1; 
int ledPin = 5;   // LED connected to digital pin 5 (must be a PWM pin)      


const int DARK_VALUE = 100; // Sensor value in complete darkness， can changes
const int BRIGHT_VALUE =800; // Sensor value in bright light, can changes

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);         
}

void loop() {
  int sensorValue = analogRead(lightSensorPin);


  int brightness = map(sensorValue, DARK_VALUE, BRIGHT_VALUE, 0,255); //Converts sensor values to PWM range (0-255), If sensor = 100 → brightness = 0 (LED off),If sensor = 800 → brightness = 255 (max brightness)
  brightness = constrain(brightness, 0, 255); // Ensure brightness value is valid  

  analogWrite(ledPin, brightness); // Set LED brightness, send PWM signal to LED


  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print(" → Brightness: ");
  Serial.println(brightness);

  delay(50);
}