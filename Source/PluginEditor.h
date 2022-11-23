/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MidiGrid.h"

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

  void automaticZoom();
  void buttonClicked(Button *button) override;
  void setupButtons();

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  PrecisionAudioProcessor &audioProcessor;
  MidiGrid grid;
  Viewport viewPort;

  juce::TextButton horizontalZoomIn;
  juce::TextButton horizontalZoomOut;
  juce::TextButton verticalZoomIn;
  juce::TextButton verticalZoomOut;
  juce::TextButton quantizeButton;


  int defaultWidth = 1200;
  int defaultHeight = 700;
  float widthMultiplier = 1.0f;
  float heightMultiplier = 1.0f;
  AffineTransform scaler = AffineTransform::AffineTransform();

  int horizontalMargin = 50;
  int verticalMargin = 50;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrecisionAudioProcessorEditor)
};
