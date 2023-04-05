#pragma once

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MidiGrid.h"
#include "Utils.h"
#include <filesystem>
#include <iostream>
#include <fstream>

//==============================================================================
PrecisionAudioProcessorEditor::PrecisionAudioProcessorEditor(PrecisionAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{

  setSize(defaultWidth, defaultHeight);
  setResizable(true, true);

  setupButtons();

  addAndMakeVisible(transportPanel);

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

int getBpmFromString(String text)
{
  std::string std_text = text.toStdString();
  for (int i = 0; i < text.length(); i++)
  {
    if (!isdigit(std_text.at(i)))
    {
      return -1;
    }
  }
  int newBpm = text.getIntValue();
  if (newBpm < 30 || newBpm > 300)
  {
    return -1;
  }
  else
  {
    return newBpm;
  }
}

int getNumBarsFromString(String text)
{
  std::string std_text = text.toStdString();
  for (int i = 0; i < text.length(); i++)
  {
    if (!isdigit(std_text.at(i)))
    {
      return -1;
    }
  }
  int newNumBars = text.getIntValue();
  if (newNumBars < 1 || newNumBars > 128)
  {
    return -1;
  }
  else
  {
    return newNumBars;
  }
}

void PrecisionAudioProcessorEditor::setupButtons()
{
  quantizeButton.setColour(TextButton::buttonColourId, Colours::grey);
  quantizeButton.setButtonText("Quantize");
  quantizeButton.addListener(this);
  addAndMakeVisible(quantizeButton);

  topRecordButton.setColour(TextButton::buttonColourId, Colours::red);
  topRecordButton.setButtonText("Record");
  topRecordButton.addListener(this);
  addAndMakeVisible(topRecordButton);

  bottomRecordButton.setColour(TextButton::buttonColourId, Colours::red);
  bottomRecordButton.setButtonText("Record");
  bottomRecordButton.addListener(this);
  addAndMakeVisible(bottomRecordButton);

  topClearButton.setColour(TextButton::buttonColourId, Colours::blue);
  topClearButton.setButtonText("Clear");
  topClearButton.addListener(this);
  addAndMakeVisible(topClearButton);

  bottomClearButton.setColour(TextButton::buttonColourId, Colours::blue);
  bottomClearButton.setButtonText("Clear");
  bottomClearButton.addListener(this);
  addAndMakeVisible(bottomClearButton);

  addAndMakeVisible(numBarsLabel);
  numBarsLabel.setText("Bars : ", dontSendNotification);
  numBarsLabel.setJustificationType(Justification::centred);

  addAndMakeVisible(bpmLabel);
  bpmLabel.setText("bpm : ", dontSendNotification);
  bpmLabel.setJustificationType(Justification::centred);

  addAndMakeVisible(precisionAnalytics);

  addAndMakeVisible(numBarsInput);
  numBarsInput.setText(String(numBars), dontSendNotification);
  numBarsInput.setJustificationType(Justification::left);
  numBarsInput.setColour(juce::Label::backgroundColourId, juce::Colours::grey);
  numBarsInput.setEditable(true);
  numBarsInput.onTextChange = [this]
  // TODO : Prevent this to be changed when recording
  {
    int inputInt = getNumBarsFromString(numBarsInput.getText());
    if (inputInt != -1)
    {
      numBars = inputInt;
      minWidthMultiplier = static_cast<float>(topGridView.getWidth()) / (BEAT_LENGTH_PIXELS * numBars * timeSigNumerator);
      widthMultiplier = std::max(widthMultiplier * 0.8f, minWidthMultiplier);

      topGrid.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 128 * NOTE_HEIGHT);
      bottomGrid.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 128 * NOTE_HEIGHT);
      topScroller.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 50);
      bottomScroller.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 50);

      setTransforms();
    }
    else
    {
      numBarsInput.setText(String(numBars), dontSendNotification);
    }
  };

  addAndMakeVisible(bpmInput);
  bpmInput.setText(String(bpm), dontSendNotification);
  bpmInput.setJustificationType(Justification::left);
  bpmInput.setColour(juce::Label::backgroundColourId, juce::Colours::grey);
  bpmInput.setEditable(true);
  bpmInput.onTextChange = [this]
  {
    int inputInt = getBpmFromString(bpmInput.getText());
    if (inputInt != -1 && isStandalone)
    {
      bpm = (float)inputInt;
    }
    else
    {
      bpmInput.setText(String(bpm), dontSendNotification);
    }
  };

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

void PrecisionAudioProcessorEditor::startRecording(GridType grid)
{
  audioProcessor.ppqRecordingStart = ppqPositionOfLastBarStart + numPrecountBeats;
  switch (grid)
  {
  case (GridType::Model):
    audioProcessor.modelRecording = true;
    break;
  case (GridType::Student):
    audioProcessor.studentRecording = true;
    bottomGrid.markNotesAsOld();
    break;
  }
  isRecording = true;
}

void PrecisionAudioProcessorEditor::stopRecording()
{
  audioProcessor.studentRecording = false;
  audioProcessor.modelRecording = false;
  isRecording = false;
}

void PrecisionAudioProcessorEditor::buttonClicked(Button *button)
{
  if (button == &quantizeButton)
    topGrid.quantize();
  if (button == &topRecordButton)
  {
    if (audioProcessor.studentRecording && audioProcessor.modelRecording)
    {
      print("Error : two grids recording at the same time");
      stopRecording();
      startRecording(GridType::Model);
    }
    else if (audioProcessor.modelRecording)
    {
      stopRecording();
    }
    else if (audioProcessor.studentRecording)
    {
      stopRecording();
      startRecording(GridType::Model);
    }
    else
    {
      startRecording(GridType::Model);
    }
  }
  if (button == &bottomRecordButton)
  {
    if (audioProcessor.studentRecording && audioProcessor.modelRecording)
    {
      print("Error : two grids recording at the same time");
      stopRecording();
      startRecording(GridType::Student);
    }
    else if (audioProcessor.studentRecording)
    {
      stopRecording();
    }
    else if (audioProcessor.modelRecording)
    {
      stopRecording();
      startRecording(GridType::Student);
    }
    else
    {
      startRecording(GridType::Student);
    }
  }
  // TODO : update analytics on grid repaint (make it a separate component & add a reference to it in midigrid)
  if (button == &topClearButton)
  {
    topGrid.clearNotes();
  }
  if (button == &bottomClearButton)
  {
    bottomGrid.clearNotes();
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
// TODO : zooming/unzooming with touchpad
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
  // TODO : adapt zoom to the notes on the grid
  // TODO : follow the cursor
}

void PrecisionAudioProcessorEditor::resized()
{
  auto area = getLocalBounds();

  topGrid.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 128 * NOTE_HEIGHT);
  bottomGrid.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 128 * NOTE_HEIGHT);
  topScroller.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 50);
  bottomScroller.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 50);

  topPiano.setSize(PIANO_WIDTH, NOTE_HEIGHT * 128);
  bottomPiano.setSize(PIANO_WIDTH, NOTE_HEIGHT * 128);

  topGridView.setBounds(
      50, 70,
      area.getWidth() - 400,
      250 + topGridView.getHorizontalScrollBar().getHeight());

  bottomGridView.setBounds(
      50, 400,
      area.getWidth() - 400,
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
      area.getWidth() - 400,
      50);

  bottomScrollerView.setBounds(
      PIANO_WIDTH + 20, 350,
      area.getWidth() - 400,
      50);

  transportPanel.setBounds(
      bottomScrollerView.getX(),
      getLocalBounds().getBottom() - 30,
      80,
      25);

  setTransforms();

  // TODO : fix scrollbar weirdly disappearing
  minHeightMultiplier = static_cast<float>(topGridView.getHeight() - topGridView.getHorizontalScrollBar().getHeight()) / topGrid.getHeight();
  minWidthMultiplier = static_cast<float>(topGridView.getWidth()) / topGrid.getWidth();

  numBarsLabel.setBounds(topGridView.getRight() + 10, topGridView.getBounds().getY() + 40, 50, 20);
  numBarsInput.setBounds(topGridView.getRight() + 60, topGridView.getBounds().getY() + 40, 30, 20);
  bpmLabel.setBounds(topGridView.getRight() + 10, topGridView.getBounds().getY() + 80, 50, 20);
  bpmInput.setBounds(topGridView.getRight() + 60, topGridView.getBounds().getY() + 80, 30, 20);

  precisionAnalytics.setBounds(bottomGridView.getRight() + 10, bottomGridView.getBounds().getY() + 10,
                               getWidth() - bottomGridView.getRight() - 20, bottomGridView.getHeight() - 35);

  quantizeButton.setBounds(
      topGridView.getRight() + 10,
      topGridView.getBounds().getCentreY(),
      80,
      25);

  topRecordButton.setBounds(
      topScrollerView.getRight() + 10,
      topScrollerView.getY(),
      80,
      25);

  bottomRecordButton.setBounds(
      bottomScrollerView.getRight() + 10,
      bottomScrollerView.getY(),
      80,
      25);

  topClearButton.setBounds(
      topScrollerView.getRight() + 10,
      topScrollerView.getY() + 30,
      80,
      25);

  bottomClearButton.setBounds(
      bottomScrollerView.getRight() + 10,
      bottomScrollerView.getY() + 30,
      80,
      25);

  quantizationSelector.setBounds(
      topGridView.getRight() + 10,
      topGridView.getBounds().getCentreY() + 30,
      80,
      25);
}
