#ifndef CLAP_DETECTION_H // CLAP_DETECTION_EXAMPLE_H
#define CLAP_DETECTION_H // CLAP_DETECTION_EXAMPLE_H

#include <Arduino.h>

// ClapDetection(int sampleSize, int amplitudeMargin, int clapWindow);
class ClapDetection
{
public:
    // Constructor (Header)
    ClapDetection(int sampleSize, int amplitudeMargin, int clapWindow);
    // Destructor (Header)
    ~ClapDetection();

    // void detect_claps(int pinToRead, int pinToWrite);
    bool detect_claps(int pinToRead);

private:
    // Function Declarations
    void updateSoundSamples(int newSample);
    int computeMovingAverage();
    bool processAmplitude(int amplitude);

    // Configuration parameters
    const int sampleSize;      // Number of samples in the moving average
    const int amplitudeMargin; // Margin above the baseline to consider a peak
    const int clapWindow;      // Time(in ms) window for detecting consecutive claps

    // Buffer and indices for moving average
    int *soundSamples;
    int sampleIndex;

    // Variables for clap detection
    int lastPeakTime;
    int clapCount;
};

#endif // CLAP_DETECTION_EXAMPLE_H