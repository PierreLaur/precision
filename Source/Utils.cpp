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

// PlayHead information from host
double bpm = 120.0;
int timeSigNumerator = 4;
int timeSigDenominator = 4;
int timeInSamples = 0;
double timeInSeconds = 0.0;
double ppqPosition = 0.0;
double ppqPositionOfLastBarStart = 0.0;
double relativePpqPosition = 0.0;
bool isStandalone = false;

int numPrecountBeats = 0;

int numBars = 8;
float quantizationInBeats = 1.0;

bool isRecording = false;

void updatePlayHeadInfo(Optional<AudioPlayHead::PositionInfo> optInfo)
{
  if (!optInfo.hasValue())
  {
    return;
  }
  auto info = *optInfo;

  if (info.getBpm().hasValue())
  {
    isStandalone = false;
    bpm = *info.getBpm();

    auto timeSig = *info.getTimeSignature();
    timeSigNumerator = timeSig.numerator;
    timeSigDenominator = timeSig.denominator;

    timeInSamples = (int)*info.getTimeInSamples();
    timeInSeconds = *info.getTimeInSeconds();
    ppqPosition = *info.getPpqPosition();
    ppqPositionOfLastBarStart = *info.getPpqPositionOfLastBarStart();
  }
  else
  {
    isStandalone = true;
  }
}

double msToBeats(double ms)
{
  auto bps = (double)(bpm) / 60;
  return bps * ms / 1000;
}

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
  double bps = bpm / 60.0;
  double seconds = samples / sampleRate;
  double beats = seconds * bps;
  return beats;
}

int beatsToSamples(double beats, double sampleRate)
{
  double bps = bpm / 60.0;
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
