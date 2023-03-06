/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MidiGrid.h"
#include "VerticalPiano.h"
#include "LinkableViewport.h"
#include "BeatScroller.h"
#include "PrecisionAnalytics.h"
#include "TransportPanel.h"

using namespace juce;

//==============================================================================
/**
 */
class PrecisionAudioProcessorEditor : public AudioProcessorEditor, public Button::Listener
{
public:
  PrecisionAudioProcessorEditor(PrecisionAudioProcessor &);
  ~PrecisionAudioProcessorEditor() override;

  //==============================================================================
  void paint(Graphics &) override;
  void resized() override;

  void verticalZoom(const MouseWheelDetails wheel);
  void horizontalZoom(const MouseWheelDetails wheel);
  void setTransforms();

  void quantizationChanged();

  void automaticZoom();
  void buttonClicked(Button *button) override;
  void setupButtons();

  void startRecording(GridType grid);
  void stopRecording();

  VerticalPiano topPiano;
  VerticalPiano bottomPiano;
  PrecisionAnalytics precisionAnalytics;
  MidiGrid topGrid{GridType::Model, precisionAnalytics};
  MidiGrid bottomGrid{GridType::Student, precisionAnalytics};
  BeatScroller topScroller;
  BeatScroller bottomScroller;

  TransportPanel transportPanel;
  double ha = 2.0;

  // TODO : check which pointers i can replace with references
  LinkableViewport topGridView = LinkableViewport(*this);
  LinkableViewport topPianoView = LinkableViewport(*this);
  LinkableViewport topScrollerView = LinkableViewport(*this);

  LinkableViewport bottomGridView = LinkableViewport(*this);
  LinkableViewport bottomPianoView = LinkableViewport(*this);
  LinkableViewport bottomScrollerView = LinkableViewport(*this);

  PrecisionAudioProcessor &audioProcessor;
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.

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
  float widthMultiplier = 1.0f;
  float heightMultiplier = 1.0f;
  float minWidthMultiplier;
  float minHeightMultiplier;

  AffineTransform scaler = AffineTransform::AffineTransform();

  int horizontalMargin = 50;
  int verticalMargin = 50;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrecisionAudioProcessorEditor)
};
