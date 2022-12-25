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

  void startRecording(GridType grid) ;
  void stopRecording() ;


  VerticalPiano topPiano;
  VerticalPiano bottomPiano;
  MidiGrid topGrid {GridType::Model};
  MidiGrid bottomGrid{GridType::Student};
  BeatScroller topScroller;
  BeatScroller bottomScroller;

  LinkableViewport topGridView = LinkableViewport(*this);
  LinkableViewport topPianoView = LinkableViewport(*this);
  LinkableViewport topScrollerView = LinkableViewport(*this);

  LinkableViewport bottomGridView = LinkableViewport(*this);
  LinkableViewport bottomPianoView = LinkableViewport(*this);
  LinkableViewport bottomScrollerView = LinkableViewport(*this);

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  PrecisionAudioProcessor &audioProcessor;

  TextButton quantizeButton;
  TextButton topRecordButton;
  TextButton bottomRecordButton;
  TextButton topClearButton;
  TextButton bottomClearButton;
  TextButton updateButton;

  Label numBarsLabel;
  Label numBarsInput;
  Label tempoLabel;
  Label tempoInput;

  Label precisionAnalytics;

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
