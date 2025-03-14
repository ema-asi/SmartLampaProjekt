#include "clap_detection.h"

// Constructor
ClapDetection::ClapDetection(int sampleSize, int amplitudeMargin, int clapWindow)
            : sampleSize(sampleSize), amplitudeMargin(amplitudeMargin), clapWindow(clapWindow), sampleIndex(0), lastPeakTime(0), clapCount(0)
            {
              soundSamples = new int[sampleSize];
              for (int i = 0; i < sampleSize; i++)
              {
                soundSamples[i] = 0;
              }
            }
// End of Constructor

// Destructor
ClapDetection::~ClapDetection()
{
  delete[] soundSamples;
}

// Updates the circular buffer with a new sound sample.
void ClapDetection::updateSoundSamples(int newSample) 
{
    soundSamples[sampleIndex] = newSample;
    sampleIndex = (sampleIndex + 1) % sampleSize;
}

// Computes and returns the moving average of the stored sound samples.
int ClapDetection::computeMovingAverage() 
{
    long sum = 0;
    for (int i = 0; i < sampleSize; i++) 
    {
        sum += soundSamples[i];
    }
    return sum / sampleSize;
}

// Processes the current amplitude reading to detect claps.
// If a peak (clap) is detected above the dynamic baseline, it checks if two
// peaks occur within the defined time window. Returns true when a double clap is detected.
bool ClapDetection::processAmplitude(int amplitude) 
{
    // Update the moving average buffer with the new sample.
    updateSoundSamples(amplitude);
    int baseline = computeMovingAverage();
    
    // Check if the amplitude exceeds the baseline plus a set margin.
    if (amplitude > (baseline + amplitudeMargin)) 
    {
        unsigned long currentTime = millis();  // Non-blocking timer
        
        // Reset the counter if the time between peaks exceeds the clapWindow.
        if (currentTime - lastPeakTime > clapWindow) 
        {
            clapCount = 0;
        }
        
        // Increment the clap counter and update the last peak time.
        clapCount++;
        lastPeakTime = currentTime;
        
        // If two peaks occur within the window, register a double clap.
        if (clapCount >= 2) 
        {
            clapCount = 0; // Reset for the next detection cycle.
            return true;
        }
    }
    return false;
}

