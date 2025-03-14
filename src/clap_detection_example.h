#ifndef CLAP_DETECTION_EXAMPLE_H
#define CLAP_DETECTION_EXAMPLE_H

#include "clap_detection.h"

ClapDetection clapdetection(5, 20, 500);

void detect_claps(int pinToRead, int pinToWrite)
{
    int currentAmplitude = analogRead(pinToRead);

    if (clapdetection.processAmplitude(currentAmplitude))
    {
        Serial.println("Beep Boop, you wake the computah!");
        // digitalWrite(pinToWrite, HIGH);
        // analogWrite(pinToWrite, 255);
        delay(500);
    }
}

#endif