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
  addAndMakeVisible(topScrollerView);
  addAndMakeVisible(bottomScrollerView);

  // Synchronize scrolling on both viewports
  topGridView.linkViewport(&topPianoView);
  topGridView.linkViewport(&bottomPianoView);
  topGridView.linkViewport(&bottomGridView);
  topGridView.linkViewport(&topScrollerView);
  topGridView.linkViewport(&bottomScrollerView);

  bottomGridView.linkViewport(&topPianoView);
  bottomGridView.linkViewport(&bottomPianoView);
  bottomGridView.linkViewport(&topGridView);
  bottomGridView.linkViewport(&topScrollerView);
  bottomGridView.linkViewport(&bottomScrollerView);

  topGridView.setViewedComponent(&topGrid, false);

  bottomGridView.setViewedComponent(&bottomGrid, false);

  topScrollerView.setViewedComponent(&topScroller, false);
  bottomScrollerView.setViewedComponent(&bottomScroller, false);

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
  topScrollerView.setScrollOnDragMode(Viewport::ScrollOnDragMode::never);
  topScrollerView.setScrollBarsShown(false, false);
  bottomScrollerView.setScrollOnDragMode(Viewport::ScrollOnDragMode::never);
  bottomScrollerView.setScrollBarsShown(false, false);

  bottomGrid.modelGrid = &topGrid;
}

void PrecisionAudioProcessorEditor::setupButtons()
{
  quantizeButton.setColour(TextButton::buttonColourId, Colours::grey);
  quantizeButton.setButtonText("Quantize");
  quantizeButton.addListener(this);
  addAndMakeVisible(quantizeButton);

  recordButton.setColour(TextButton::buttonColourId, Colours::red);
  recordButton.setButtonText("Record");
  recordButton.addListener(this);
  addAndMakeVisible(recordButton);

  quantizationSelector.addItem("1/1", 1);
  quantizationSelector.addItem("1/2", 2);
  quantizationSelector.addItem("1/4", 4);
  quantizationSelector.addItem("1/8", 8);
  quantizationSelector.addItem("1/16", 16);
  quantizationSelector.addItem("1/32", 32);

  quantizationSelector.setSelectedId(4);
  quantizationInBeats = 1.0f;
  quantizationSelector.onChange = [this]
  { quantizationChanged(); };

  addAndMakeVisible(quantizationSelector);
}

PrecisionAudioProcessorEditor::~PrecisionAudioProcessorEditor()
{
}

//==============================================================================
void PrecisionAudioProcessorEditor::paint(Graphics &g)
{
  g.fillAll(Colours::transparentBlack);
}

void PrecisionAudioProcessorEditor::quantizationChanged()
{
  quantizationInBeats = 4.0f / (float)quantizationSelector.getSelectedId();
  repaint();
}

void PrecisionAudioProcessorEditor::buttonClicked(Button *button)
{

  if (button == &quantizeButton)
    topGrid.quantize();
  if (button == &recordButton) {
    if (audioProcessor.isRecording) {
      // hide the cursor
      auto bounds = topGrid.cursor.getBounds() ;
      bounds.setX(-5) ;
      topGrid.cursor.setBounds(bounds) ;
      bottomGrid.cursor.setBounds(bounds) ;
    } else {
      // show the cursor
      auto bounds = topGrid.cursor.getBounds() ;
      bounds.setX(0) ;
      topGrid.cursor.setBounds(bounds) ;
      bottomGrid.cursor.setBounds(bounds) ;
    }
    audioProcessor.blockStartTimesteps = 0;
    audioProcessor.isRecording = !audioProcessor.isRecording;
  }

}

void PrecisionAudioProcessorEditor::setTransforms()
{
  topPiano.setTransform(scaler.scale(1.0f, heightMultiplier));
  topGrid.setTransform(scaler.scale(widthMultiplier, heightMultiplier));
  bottomPiano.setTransform(scaler.scale(1.0f, heightMultiplier));
  bottomGrid.setTransform(scaler.scale(widthMultiplier, heightMultiplier));
  topScroller.setTransform(scaler.scale(widthMultiplier, 1.0f));
  bottomScroller.setTransform(scaler.scale(widthMultiplier, 1.0f));
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

  topGrid.setSize(BEAT_LENGTH_PIXELS * 32, 128 * NOTE_HEIGHT);
  bottomGrid.setSize(BEAT_LENGTH_PIXELS * 32, 128 * NOTE_HEIGHT);
  topPiano.setSize(PIANO_WIDTH, NOTE_HEIGHT * 128);
  bottomPiano.setSize(PIANO_WIDTH, NOTE_HEIGHT * 128);
  topScroller.setSize(BEAT_LENGTH_PIXELS * 32, 50);
  bottomScroller.setSize(BEAT_LENGTH_PIXELS * 32, 50);

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
      PIANO_WIDTH,
      250);

  bottomPianoView.setBounds(
      20, 400,
      PIANO_WIDTH,
      250);

  topScrollerView.setBounds(
      PIANO_WIDTH + 20, 20,
      area.getWidth() - 200,
      50);

  bottomScrollerView.setBounds(
      PIANO_WIDTH + 20, 350,
      area.getWidth() - 200,
      50);

  setTransforms();

  // TODO : choose scrollbars on or off
  minHeightMultiplier = static_cast<float>(topGridView.getHeight() - topGridView.getHorizontalScrollBar().getHeight()) / topGrid.getHeight();
  minWidthMultiplier = static_cast<float>(topGridView.getWidth()) / topGrid.getWidth();

  quantizeButton.setBounds(
      topGridView.getRight() + 10,
      topGridView.getBounds().getCentreY(),
      80,
      25);

  recordButton.setBounds(
      topGridView.getRight() + 10,
      topGridView.getY(),
      80,
      25);

  quantizationSelector.setBounds(
      topGridView.getRight() + 10,
      topGridView.getBounds().getCentreY() + 30,
      80,
      25);
}
