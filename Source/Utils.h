/*
  ==============================================================================

    Utils.h
    Created: 10 Nov 2022 4:46:00pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

const int NOTE_HEIGHT = 16;
const int NOTE_MIN_WIDTH = 10;
const int BEAT_LENGTH_PIXELS = 128;
const int PIANO_WIDTH = 30;
extern float quantizationInBeats;
extern double tempo;
extern int timeSigNumerator;
extern int timeSigDenominator;
extern int numBars;

void setQuantizationInBeats(float newQuantization);
double samplesToSeconds(int samples, double sampleRate);
int secondsToSamples(float samples, double sampleRate);
double samplesToBeats(int samples, double sampleRate);
int beatsToSamples(double beats, double sampleRate);
String noteNumberToName(int noteNumber);
template <typename T>
void print(T const &value)
{
  std::cout << value << std::endl;
}

MidiFile readMidiFile(String address);