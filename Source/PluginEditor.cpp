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

  addAndMakeVisible(viewPort1);
  addAndMakeVisible(viewPort2);
  viewPort1.linkViewport(&viewPort2) ;
  viewPort1.setViewedComponent(&grid, false);
  viewPort2.setViewedComponent(&verticalPiano, false);
  viewPort1.setViewPosition(Point(0, viewPort1.getHeight() / 2));
  viewPort1.setScrollOnDragMode(Viewport::ScrollOnDragMode::all);
  viewPort2.setScrollOnDragMode(Viewport::ScrollOnDragMode::all);
  viewPort2.setScrollBarsShown(false,false) ;
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
  g.fillAll(Colours::darkred);
}

void PrecisionAudioProcessorEditor::buttonClicked(Button *button)
{

  if (button == &horizontalZoomIn)
    widthMultiplier += 0.1f;
  else if (button == &horizontalZoomOut)
    widthMultiplier = std::max(widthMultiplier - 0.1f, minWidthMultiplier);
  else if (button == &verticalZoomIn)
    heightMultiplier += 0.1f;
  else if (button == &verticalZoomOut)
    heightMultiplier = std::max(heightMultiplier - 0.1f, minHeightMultiplier);
  else if (button == &quantizeButton)
    grid.quantize();
  scaler = scaler.scale(widthMultiplier, heightMultiplier);
  verticalPiano.setTransform(scaler);
  grid.setTransform(scaler);

  // TODO : limit zoom so that the view stays the same size
}

void PrecisionAudioProcessorEditor::automaticZoom()
{
  // TODO
}

void PrecisionAudioProcessorEditor::resized()
{
  auto area = getLocalBounds();

  grid.setSize(BEAT_LENGTH_TIMESTEPS*16, 128 * NOTE_HEIGHT);
  verticalPiano.setSize(30, NOTE_HEIGHT * 128);

  viewPort1.setBounds(
      50, 20,
      area.getWidth() - 200,
      300+viewPort1.getHorizontalScrollBar().getHeight());

  viewPort2.setBounds(
      20, 20,
      30,
      300);

  // TODO : choose scrollbars on or off
  minHeightMultiplier = static_cast<float>(viewPort1.getHeight() - viewPort1.getHorizontalScrollBar().getHeight()) / verticalPiano.getHeight();
  minWidthMultiplier = static_cast<float>(viewPort1.getWidth() - viewPort1.getVerticalScrollBar().getWidth()) / verticalPiano.getWidth();

  horizontalZoomIn.setBounds(
      viewPort1.getRight() + 50,
      viewPort1.getY() + 20,
      40,
      30);

  horizontalZoomOut.setBounds(
      viewPort1.getRight() + 100,
      viewPort1.getY() + 20,
      40,
      30);

  verticalZoomIn.setBounds(
      viewPort1.getRight() + 50,
      viewPort1.getY() + 60,
      40,
      30);

  verticalZoomOut.setBounds(
      viewPort1.getRight() + 100,
      viewPort1.getY() + 60,
      40,
      30);

  quantizeButton.setBounds(
      viewPort1.getRight() + 50,
      viewPort1.getY() + 120,
      30,
      30);
}
