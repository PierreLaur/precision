/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PrecisionAnalytics.h"
#include "TransportPanel.h"
#include "MidiView.h"
#include "ControlPanel.h"

using namespace juce;

//==============================================================================
class PrecisionAudioProcessorEditor : public AudioProcessorEditor
{
public:
  PrecisionAudioProcessorEditor(PrecisionAudioProcessor &);
  ~PrecisionAudioProcessorEditor() override;

  //==============================================================================
  void paint(Graphics &) override;
  void resized() override;
  void panelUpdate(Button *button);

  void startRecording();
  void stopRecording();

  void processMidiMessage(MidiMessage *message, double position, double maxPpqPosition)
  {
    midiView.grid.processMidiMessage(message, position, maxPpqPosition);
  }

  PrecisionAnalytics precisionAnalytics;
  MidiView midiView = MidiView(precisionAnalytics);
  TransportPanel transportPanel;
  ControlPanel controlPanel = ControlPanel();

  PrecisionAudioProcessor &audioProcessor;

private:
  int defaultWidth = 1400;
  int defaultHeight = 700;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrecisionAudioProcessorEditor)
};
