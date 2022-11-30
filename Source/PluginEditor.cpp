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

  addAndMakeVisible(topGridView);
  addAndMakeVisible(bottomGridView);
  addAndMakeVisible(topPianoView);
  addAndMakeVisible(bottomPianoView);

  // Synchronize scrolling on both viewports
  topGridView.linkViewport(&topPianoView);
  topGridView.linkViewport(&bottomPianoView);
  topGridView.linkViewport(&bottomGridView);
  bottomGridView.linkViewport(&topPianoView);
  bottomGridView.linkViewport(&bottomPianoView);
  bottomGridView.linkViewport(&topGridView);

  topGridView.setViewedComponent(&topGrid, false);
  bottomGridView.setViewedComponent(&bottomGrid, false);
  topPianoView.setViewedComponent(&topPiano, false);
  bottomPianoView.setViewedComponent(&bottomPiano, false);

  // centers of the grids
  topGridView.setViewPosition(Point(0, topGrid.getHeight() / 2));
  bottomGridView.setViewPosition(Point(0, bottomGrid.getHeight() / 2));

  // Hide the vertical scrollbar, show the horizontal one
  topGridView.setScrollOnDragMode(Viewport::ScrollOnDragMode::all);
  topGridView.setScrollBarsShown(false, true, true);
  bottomGridView.setScrollOnDragMode(Viewport::ScrollOnDragMode::all);
  bottomGridView.setScrollBarsShown(false, true, true);

  topPianoView.setScrollOnDragMode(Viewport::ScrollOnDragMode::never);
  topPianoView.setScrollBarsShown(false, false);
  bottomPianoView.setScrollOnDragMode(Viewport::ScrollOnDragMode::never);
  bottomPianoView.setScrollBarsShown(false, false);

  bottomGrid.modelGrid = &topGrid ;
}

void PrecisionAudioProcessorEditor::setupButtons()
{
  quantizeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
  quantizeButton.setButtonText("Q");
  quantizeButton.addListener(this);
  addAndMakeVisible(quantizeButton);
}

PrecisionAudioProcessorEditor::~PrecisionAudioProcessorEditor()
{
}

//==============================================================================
void PrecisionAudioProcessorEditor::paint(Graphics &g)
{
  g.fillAll(Colours::transparentBlack);
}

void PrecisionAudioProcessorEditor::buttonClicked(Button *button)
{

  if (button == &quantizeButton)
    topGrid.quantize();

  // TODO : limit zoom so that the view stays the same size
}

void PrecisionAudioProcessorEditor::setTransforms()
{
  topPiano.setTransform(scaler.scale(1.0f, heightMultiplier));
  topGrid.setTransform(scaler.scale(widthMultiplier, heightMultiplier));
  bottomPiano.setTransform(scaler.scale(1.0f, heightMultiplier));
  bottomGrid.setTransform(scaler.scale(widthMultiplier, heightMultiplier));
}

void PrecisionAudioProcessorEditor::verticalZoom(const MouseWheelDetails wheel)
{
  if (wheel.deltaY > 0)
  {
    heightMultiplier = 1.2f * heightMultiplier;
  }
  else
  {
    heightMultiplier = std::max(heightMultiplier * 0.8f, minHeightMultiplier);
  }
  setTransforms();
}

void PrecisionAudioProcessorEditor::horizontalZoom(const MouseWheelDetails wheel)
{
  if (wheel.deltaY > 0)
  {
    widthMultiplier = 1.2f * widthMultiplier;
  }
  else
  {
    widthMultiplier = std::max(widthMultiplier * 0.8f, minWidthMultiplier);
  }
  setTransforms();
}

void PrecisionAudioProcessorEditor::automaticZoom()
{
  // TODO
}

void PrecisionAudioProcessorEditor::resized()
{
  auto area = getLocalBounds();

  topGrid.setSize(BEAT_LENGTH_TIMESTEPS * 32, 128 * NOTE_HEIGHT);
  bottomGrid.setSize(BEAT_LENGTH_TIMESTEPS * 32, 128 * NOTE_HEIGHT);
  topPiano.setSize(30, NOTE_HEIGHT * 128);
  bottomPiano.setSize(30, NOTE_HEIGHT * 128);

  topGridView.setBounds(
      50, 70,
      area.getWidth() - 200,
      250 + topGridView.getHorizontalScrollBar().getHeight());

  bottomGridView.setBounds(
      50, 400,
      area.getWidth() - 200,
      250 + bottomGridView.getHorizontalScrollBar().getHeight());

  topPianoView.setBounds(
      20, 70,
      30,
      250);

  bottomPianoView.setBounds(
      20, 400,
      30,
      250);

  topPiano.setTransform(scaler.scale(1.0f, heightMultiplier));
  topGrid.setTransform(scaler.scale(widthMultiplier, heightMultiplier));
  bottomPiano.setTransform(scaler.scale(1.0f, heightMultiplier));
  bottomGrid.setTransform(scaler.scale(widthMultiplier, heightMultiplier));

  // TODO : choose scrollbars on or off
  minHeightMultiplier = static_cast<float>(topGridView.getHeight() - topGridView.getHorizontalScrollBar().getHeight()) / topGrid.getHeight();
  minWidthMultiplier = static_cast<float>(topGridView.getWidth()) / topGrid.getWidth();

  quantizeButton.setBounds(
      topGridView.getRight() + 50,
      topGridView.getY() + 120,
      30,
      30);
}
