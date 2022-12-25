/*
  ==============================================================================

    Metronome.cpp
    Created: 22 Dec 2022 9:27:55pm
    Author:  pierr

  ==============================================================================
*/

#include "Metronome.h"
#include "Utils.h"

void Metronome::count(AudioBuffer<float> &buffer, int totalNumOutputChannels, int numSamples, double sampleRate)
{
  if (numSamplesBeforeBeat < numSamples)
  {

    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
      auto writePointer = buffer.getWritePointer(channel);
      writePointer[numSamplesBeforeBeat] = 4.0f;
    }
    samplesPerBeat = beatsToSamples(1.0, sampleRate);
    numSamplesBeforeBeat += samplesPerBeat;
  }
  else
  {
    numSamplesBeforeBeat -= numSamples;
  }
}

void Metronome::reset()
{
  numSamplesBeforeBeat = 0;
}