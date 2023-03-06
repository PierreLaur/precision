/*
  ==============================================================================

    Metronome.cpp
    Created: 22 Dec 2022 9:27:55pm
    Author:  pierr

  ==============================================================================
*/

#include "Metronome.h"
#include "Utils.h"

int Metronome::count(AudioBuffer<float> &buffer, int totalNumOutputChannels, int blockStart, int numNewSamples, double sampleRate)
{
  samplesPerBeat = beatsToSamples(1.0, sampleRate);
  int samplesInBeat = blockStart % samplesPerBeat;
  int samplesBeforeBeat = samplesPerBeat - samplesInBeat;

  if (samplesInBeat == 0 || numNewSamples >= samplesBeforeBeat)
  {
    // Click
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
      auto writePointer = buffer.getWritePointer(channel);
      writePointer[samplesBeforeBeat] = 4.0f;
    }
    return samplesBeforeBeat;
  }
  else
    return 0;
}