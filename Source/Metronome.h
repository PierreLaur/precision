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

  int count(AudioBuffer<float> &writePointer, int totalNumOutputChannels, int blockStart, int numNewSamples, double sampleRate);

  int samplesPerBeat{0};

  AudioFormatManager formatManager;
  std::unique_ptr<AudioFormatReaderSource> clickSample{nullptr};
};