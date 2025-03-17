// #ifndef CLAP_DETECTION_EXAMPLE_H
// #define CLAP_DETECTION_EXAMPLE_H

// /**
//  * @brief stores amplitude reading in an int and uses ClapDetection class to process this
//  * @param pinToRead
//  * @param pinToWrite
//  */
// void detect_claps(int pinToRead, int pinToWrite)
// {
//     int currentAmplitude = analogRead(pinToRead);

//     if (clapdetection.processAmplitude(currentAmplitude) == true)
//     {
//         Serial.println("Beep Boop, you wake the computah!");
//         digitalWrite(pinToWrite, HIGH);
//         // analogWrite(pinToWrite, 255);
//         delay(5000);
//     }
//     else if (clapdetection.processAmplitude(currentAmplitude) == false)
//     {
//         digitalWrite(pinToWrite, LOW);
//     }
// }

// #endif