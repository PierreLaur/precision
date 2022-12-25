/*
  ==============================================================================

    Metronome.h
    Created: 22 Dec 2022 9:27:55pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class Metronome
{
public:
  Metronome(){};

  void count(AudioBuffer<float> &writePointer, int totalNumOutputChannels, int numSamples, double sampleRate);
  void reset();

  int numSamplesBeforeBeat{0};
  int samplesPerBeat{0};

  AudioFormatManager formatManager;
  std::unique_ptr<AudioFormatReaderSource> clickSample{nullptr};
};