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

using namespace juce;

//==============================================================================
/**
 */
class PrecisionAudioProcessorEditor : public juce::AudioProcessorEditor, public Button::Listener
{
public:
  PrecisionAudioProcessorEditor(PrecisionAudioProcessor &);
  ~PrecisionAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

  void verticalZoom(const MouseWheelDetails wheel);
  void horizontalZoom(const MouseWheelDetails wheel);
  void setTransforms();

  void automaticZoom();
  void buttonClicked(Button *button) override;
  void setupButtons();

  VerticalPiano topPiano;
  VerticalPiano bottomPiano;
  MidiGrid topGrid;
  MidiGrid bottomGrid;

  LinkableViewport topGridView = LinkableViewport(*this);
  LinkableViewport topPianoView = LinkableViewport(*this);
  LinkableViewport bottomGridView = LinkableViewport(*this);
  LinkableViewport bottomPianoView = LinkableViewport(*this);

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  PrecisionAudioProcessor &audioProcessor;

  juce::TextButton quantizeButton;
  juce::TextButton optimizeButton;

  int defaultWidth = 1250;
  int defaultHeight = 700;
  float widthMultiplier = 0.8f;
  float heightMultiplier = 1.2f;
  float minWidthMultiplier ;
  float minHeightMultiplier ;

  AffineTransform scaler = AffineTransform::AffineTransform();

  int horizontalMargin = 50;
  int verticalMargin = 50;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrecisionAudioProcessorEditor)
};
