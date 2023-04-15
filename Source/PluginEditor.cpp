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
  if (button == &controlPanel.recordButton)
  {
    if (recording)
    {
      stopRecording();
    }
    else
    {
      startRecording();
    }
  }
  if (button == &controlPanel.clearButton)
  {
    midiView.clearNotes();
  }
  if (button == &controlPanel.filterButton)
  {
    if (controlPanel.filterButton.getToggleState())
    {
      controlPanel.filterButton.setColour(TextButton::buttonColourId, Colours::darkmagenta);
      controlPanel.filterButton.setButtonText("Filtering...");
      filtering = true;
    }
    else
    {
      controlPanel.filterButton.setColour(TextButton::buttonColourId, Colours::purple);
      controlPanel.filterButton.setButtonText("Filter");
      filtering = false;
    };
  }
}

void PrecisionAudioProcessorEditor::startRecording()
{
  audioProcessor.ppqRecordingStart = ppqPositionOfLastBarStart + numPrecountBeats;
  midiView.markNotesAsOld();
  recording = true;
}

void PrecisionAudioProcessorEditor::stopRecording()
{
  recording = false;
  relativePpqPosition = 0.0;
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
      (int)(area.getHeight() / 2));

  precisionAnalytics.setBounds(
      midiView.getRight() + horizontalMargin,
      controlPanel.getBottom() + verticalMargin,
      getWidth() - midiView.getRight() - 2 * horizontalMargin,
      midiView.getBottom() - controlPanel.getBottom() - verticalMargin);
}
