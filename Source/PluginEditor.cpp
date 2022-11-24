/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MidiGrid.h"
#include "Utils.h"
#include <filesystem>
#include <iostream>
#include <fstream>

using namespace juce;

//==============================================================================
PrecisionAudioProcessorEditor::PrecisionAudioProcessorEditor(PrecisionAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
  setSize(defaultWidth, defaultHeight);
  setResizable(true, true);

  setupButtons();

  addAndMakeVisible(viewPort);
  viewPort.setViewedComponent(&gridDisplayer, false);
  grid = &gridDisplayer.grid ;
  // viewPort.setScrollBarsShown() // TODO
  viewPort.setScrollOnDragMode(Viewport::ScrollOnDragMode::all);
}

void PrecisionAudioProcessorEditor::setupButtons()
{
  horizontalZoomIn.setColour(juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
  horizontalZoomOut.setColour(juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
  verticalZoomIn.setColour(juce::TextButton::buttonColourId, juce::Colours::greenyellow);
  verticalZoomOut.setColour(juce::TextButton::buttonColourId, juce::Colours::greenyellow);
  quantizeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);


  horizontalZoomIn.setButtonText("H+");
  horizontalZoomOut.setButtonText("H-");
  verticalZoomIn.setButtonText("V+");
  verticalZoomOut.setButtonText("V-");
  quantizeButton.setButtonText("Q");


  horizontalZoomIn.addListener(this);
  horizontalZoomOut.addListener(this);
  verticalZoomIn.addListener(this);
  verticalZoomOut.addListener(this);
  quantizeButton.addListener(this);


  addAndMakeVisible(horizontalZoomIn);
  addAndMakeVisible(horizontalZoomOut);
  addAndMakeVisible(verticalZoomIn);
  addAndMakeVisible(verticalZoomOut);
  addAndMakeVisible(quantizeButton);
}

PrecisionAudioProcessorEditor::~PrecisionAudioProcessorEditor()
{
}

//==============================================================================
void PrecisionAudioProcessorEditor::paint(Graphics &g)
{

  g.fillAll(Colours::maroon);
  // Reading a MIDI clip

}

void PrecisionAudioProcessorEditor::buttonClicked(Button *button)
{

  if (button == &horizontalZoomIn)
    widthMultiplier += 0.1f;
  else if (button == &horizontalZoomOut)
    widthMultiplier = std::max(widthMultiplier - 0.1f, minWidthMultiplier) ;
  else if (button == &verticalZoomIn)
    heightMultiplier += 0.1f ;
  else if (button == &verticalZoomOut)
    heightMultiplier = std::max(heightMultiplier - 0.1f, minHeightMultiplier) ;
  else if (button == &quantizeButton)
    grid->quantize();
  scaler = scaler.scale(widthMultiplier, heightMultiplier);
  gridDisplayer.setTransform(scaler);

  // TODO : limit zoom so that the view stays the same size
}

void PrecisionAudioProcessorEditor::automaticZoom()
{
  // TODO
}

void PrecisionAudioProcessorEditor::resized()
{
  auto area = getLocalBounds();

  gridDisplayer.setSize(1200, NOTE_HEIGHT * 128);
  viewPort.setBounds(
      20, 20,
      area.getWidth() - 150,
      300);

    // TODO : choose scrollbars on or off
  minHeightMultiplier = static_cast<float>(viewPort.getHeight()-viewPort.getHorizontalScrollBar().getHeight()) / gridDisplayer.getHeight() ;
  minWidthMultiplier = static_cast<float>(viewPort.getWidth()-viewPort.getVerticalScrollBar().getWidth()) / gridDisplayer.getWidth() ;

  horizontalZoomIn.setBounds(
      viewPort.getRight() + 50,
      viewPort.getY() + 20,
      40,
      30);

  horizontalZoomOut.setBounds(
      viewPort.getRight() + 100,
      viewPort.getY() + 20,
      40,
      30);

  verticalZoomIn.setBounds(
      viewPort.getRight() + 50,
      viewPort.getY() + 60,
      40,
      30);

  verticalZoomOut.setBounds(
      viewPort.getRight() + 100,
      viewPort.getY() + 60,
      40,
      30);

  quantizeButton.setBounds(
      viewPort.getRight() + 50,
      viewPort.getY() + 120,
      30,
      30);
}
