/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Metronome.h"

using namespace juce;

class PrecisionAudioProcessorEditor;

//==============================================================================
/**
 */
class PrecisionAudioProcessor : public AudioProcessor
#if JucePlugin_Enable_ARA
    ,
                                public AudioProcessorARAExtension
#endif
{
public:
  //==============================================================================
  PrecisionAudioProcessor();
  ~PrecisionAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  void processBlock(AudioBuffer<float> &, MidiBuffer &) override;

  //==============================================================================
  AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;
  PrecisionAudioProcessorEditor *editor;

  //==============================================================================
  const String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const String getProgramName(int index) override;
  void changeProgramName(int index, const String &newName) override;

  //==============================================================================
  void getStateInformation(MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  bool studentRecording = false;
  bool modelRecording = false;

  double ppqRecordingStart = 0.0;
  Metronome metronome;

private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrecisionAudioProcessor)
};
