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

using namespace juce;

//==============================================================================
class PrecisionAudioProcessorEditor : public AudioProcessorEditor, public Button::Listener
{
public:
  PrecisionAudioProcessorEditor(PrecisionAudioProcessor &);
  ~PrecisionAudioProcessorEditor() override;

  //==============================================================================
  void paint(Graphics &) override;
  void resized() override;

  void quantizationChanged();

  void buttonClicked(Button *button) override;
  void setupButtons();

  void startRecording(GridType grid);
  void stopRecording();

  void processMidiMessage(MidiMessage *message, double position, double maxPpqPosition, GridType grid) {
    if (grid == GridType::Student) {
      midiView.bottomGrid.processMidiMessage(message, position, maxPpqPosition);
    } else {
      midiView.topGrid.processMidiMessage(message, position, maxPpqPosition);
    }
  }

  PrecisionAnalytics precisionAnalytics;
  MidiView midiView = MidiView(precisionAnalytics);

  TransportPanel transportPanel;
  PrecisionAudioProcessor &audioProcessor;

private:

  TextButton quantizeButton;
  TextButton topRecordButton;
  TextButton bottomRecordButton;
  TextButton topClearButton;
  TextButton bottomClearButton;

  Label numBarsLabel;
  Label numBarsInput;
  Label bpmLabel;
  Label bpmInput;

  ComboBox quantizationSelector;

  int defaultWidth = 1400;
  int defaultHeight = 700;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrecisionAudioProcessorEditor)
};
