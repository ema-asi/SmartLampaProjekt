

#include <avr/sleep.h>
#include <avr/wdt.h>

volatile bool wakeUpFlag = false;

// Pausar Service Routine (ISR) föt att vakna till
ISR(WDT_vect)
{
    wakeUpFlag = true; // sätter väckarflaggan när den vaknar
}

void setup()
{
    Serial.begin(9600);
    pinMode(A0, INPUT); // Exempel på där den blir kopplad
    // Configure watchdog timer for 1-second intervals
    configureWatchdogTimer();
}

void loop()
{
    if (wakeUpFlag)
    {
        wakeUpFlag = false; // Reset the flag

        int sensorValue = analogRead(A0);
        Serial.print("Sensor Value: ");
        Serial.println(sensorValue);
    }

    // MCU sleep mode
    goToSleep();
}

// Watchdog Timer 1-second intervaller
void configureWatchdogTimer()
{
    MCUSR &= ~(1 << WDRF);              // Clear reset flag
    WDTCSR |= (1 << WDCE) | (1 << WDE); // Enable changes to WDT
    WDTCSR = (1 << WDP2) | (1 << WDP1); // Set timeout interval to ~1 second
    WDTCSR |= (1 << WDIE);              // Enable WDT interrupt
}

// Put MCU into sleep mode
void goToSleep()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Use power-down mode
    cli();                               // Disable interrupts
    sleep_enable();                      // Enable sleep mode
    sei();                               // Re-enable interrupts
    sleep_cpu();                         // Enter sleep mode

    // CPU wakes up here after ISR triggers
    sleep_disable(); // Disable sleep mode upon wakeup
}

/*#include "ArduinoLowPower.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  LowPower.sleep(5000);
}*/