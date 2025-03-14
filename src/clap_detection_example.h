#ifndef CLAP_DETECTION_EXAMPLE_H
#define CLAP_DETECTION_EXAMPLE_H

#include "clap_detection.h"

ClapDetection clapdetection(5, 20, 500);

void dd(){}

void detect_claps(int pinToRead)
{
    int currentAmplitude = analogRead(pinToRead);

    if (clapdetection.processAmplitude(currentAmplitude))
    {
        Serial.println("Beep Boop, you wake the computah!");
    }
}

#endif