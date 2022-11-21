/*
  ==============================================================================

    Utils.cpp
    Created: 10 Nov 2022 4:45:47pm
    Author:  pierr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Utils.h"
using namespace juce ;

double samplesToSeconds(int samples, double sampleRate) 
{
    return samples/sampleRate ;
}

double samplesToBeats(int samples, double sampleRate, float tempo) 
{
    double bps = tempo / 60 ;
    double seconds = samples/sampleRate ;
    double beats = seconds/bps ;
    return beats ;
}

MidiFile readMidiFile(String address)
{
  File myFile(address);
  FileInputStream myStream(myFile);
  MidiFile midiClip = MidiFile::MidiFile();
  midiClip.readFrom(myStream);
  return midiClip;
}