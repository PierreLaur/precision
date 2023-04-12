#pragma once

#include "PluginProcessor.h"
#include "PluginEditor.h"
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

  addAndMakeVisible(precisionAnalytics);
  addAndMakeVisible(midiView);
  addAndMakeVisible(transportPanel);
  addAndMakeVisible(controlPanel);

  // Setup std functions for the control panel
  controlPanel.onUpdate = [this](Button *button)
  { panelUpdate(button); };
  controlPanel.bpmInput.onTextChange = [this]
  {
    int inputInt = controlPanel.getBpmFromString(controlPanel.bpmInput.getText());
    if (inputInt != -1 && isStandalone)
    {
      bpm = (float)inputInt;
    }
    else
    {
      controlPanel.bpmInput.setText(String(bpm), dontSendNotification);
    }
  };
  controlPanel.numBarsInput.onTextChange = [this]
  {
    int inputInt = controlPanel.getNumBarsFromString(controlPanel.numBarsInput.getText());
    if (inputInt != -1)
    {
      numBars = inputInt;

      midiView.setNumBars();
      midiView.scaleView();
    }
    else
    {
      controlPanel.numBarsInput.setText(String(numBars), dontSendNotification);
    }
  };
}

PrecisionAudioProcessorEditor::~PrecisionAudioProcessorEditor()
{
}

//==============================================================================

void PrecisionAudioProcessorEditor::paint(Graphics &g)
{
  g.fillAll(Colours::transparentBlack);
}

void PrecisionAudioProcessorEditor::panelUpdate(Button *button)
{
  if (button == &controlPanel.quantizeButton)
    midiView.quantizeModelGrid();
  if (button == &controlPanel.topRecordButton)
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
  if (button == &controlPanel.bottomRecordButton)
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
  if (button == &controlPanel.topClearButton)
  {
    midiView.clearNotes(GridType::Model);
  }
  if (button == &controlPanel.bottomClearButton)
  {
    midiView.clearNotes(GridType::Student);
  }
  if (button == &controlPanel.filterButton)
  {
    if (controlPanel.filterButton.getToggleState())
    {
      controlPanel.filterButton.setColour(TextButton::buttonColourId, Colours::darkmagenta);
      controlPanel.filterButton.setButtonText("Filtering...");
    }
    else
    {
      controlPanel.filterButton.setColour(TextButton::buttonColourId, Colours::purple);
      controlPanel.filterButton.setButtonText("Filter");
    };
  }
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
    midiView.markStudentNotesAsOld();
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

void PrecisionAudioProcessorEditor::resized()
{

  auto area = getLocalBounds();
  int horizontalMargin = 20;
  int verticalMargin = 20;

  midiView.setBounds(
      horizontalMargin, verticalMargin,
      (int)((area.getWidth() - 2 * horizontalMargin) * 0.7),
      (int)((area.getHeight() - verticalMargin * 2) * 0.98));
  midiView.scaleView();

  transportPanel.setBounds(
      midiView.getX(),
      midiView.getBottom() + verticalMargin,
      midiView.getWidth(),
      area.getHeight() - midiView.getBottom() - verticalMargin * 2);

  controlPanel.setBounds(
      midiView.getRight() + horizontalMargin,
      midiView.getY(),
      area.getWidth() - midiView.getRight() - horizontalMargin * 2,
      midiView.bottomScrollerView.getY() - midiView.getY() + 2 * (25 + verticalMargin));

  precisionAnalytics.setBounds(
      midiView.getRight() + horizontalMargin,
      controlPanel.getBottom() + verticalMargin,
      getWidth() - midiView.getRight() - 2 * horizontalMargin,
      midiView.getBottom() - controlPanel.getBottom() - verticalMargin);
}
