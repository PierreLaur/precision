/*
  ==============================================================================

    Utils.cpp
    Created: 10 Nov 2022 4:45:47pm
    Author:  pierr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Utils.h"
using namespace juce;

float quantizationInBeats = 1.0;
int timeSigNumerator = 4;
int timeSigDenominator = 4;
int numBars = 8;
double tempo = 120.0;

double samplesToSeconds(int samples, double sampleRate)
{
  return samples / sampleRate;
}

int secondsToSamples(float seconds, double sampleRate)
{
  return (int)(seconds * sampleRate);
}

double samplesToBeats(int samples, double sampleRate)
{
  double bps = tempo / 60.0;
  double seconds = samples / sampleRate;
  double beats = seconds * bps;
  return beats;
}

int beatsToSamples(double beats, double sampleRate)
{
  double bps = tempo / 60.0;
  double seconds = beats / bps;
  int samples = (int)(seconds * sampleRate);
  return samples;
}

void setQuantizationInBeats(float newQuantization)
{
  quantizationInBeats = newQuantization;
}

MidiFile readMidiFile(String address)
{
  File myFile(address);
  FileInputStream myStream(myFile);
  MidiFile midiClip = MidiFile::MidiFile();
  midiClip.readFrom(myStream);
  return midiClip;
}

String noteNumberToName(int noteNumber)
{
  String noteName = "";
  switch (noteNumber % 12)
  {
  case 0:
    noteName += "C";
    break;
  case 1:
    noteName += "C#";
    break;
  case 2:
    noteName += "D";
    break;
  case 3:
    noteName += "D#";
    break;
  case 4:
    noteName += "E";
    break;
  case 5:
    noteName += "F";
    break;
  case 6:
    noteName += "F#";
    break;
  case 7:
    noteName += "G";
    break;
  case 8:
    noteName += "G#";
    break;
  case 9:
    noteName += "A";
    break;
  case 10:
    noteName += "A#";
    break;
  case 11:
    noteName += "B";
    break;
  }
  noteName += String(noteNumber / 12 - 2);
  return noteName;
}
