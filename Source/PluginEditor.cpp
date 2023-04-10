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

  setupButtons();

  addAndMakeVisible(transportPanel);
  addAndMakeVisible(midiView);
}

PrecisionAudioProcessorEditor::~PrecisionAudioProcessorEditor()
{
}

//==============================================================================

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

      midiView.setNumBars();
      midiView.scaleView();
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

void PrecisionAudioProcessorEditor::buttonClicked(Button *button)
{
  if (button == &quantizeButton)
    midiView.quantizeModelGrid();
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
    midiView.clearNotes(GridType::Model);
  }
  if (button == &bottomClearButton)
  {
    midiView.clearNotes(GridType::Student);
  }
}

void PrecisionAudioProcessorEditor::resized()
{
  auto area = getLocalBounds();
  int horizontalMargin = 20;
  int verticalMargin = 20;

  midiView.setBounds(
      horizontalMargin, verticalMargin,
      (int)((area.getWidth() - 2 * horizontalMargin) * 0.8),
      (int)((area.getHeight() - verticalMargin * 2) * 0.98));
  midiView.scaleView();
  
  transportPanel.setBounds(
      midiView.getX(),
      area.getBottom() - verticalMargin,
      80,
      50);

  // Right Panel
  topRecordButton.setBounds(
      midiView.getRight() + horizontalMargin,
      midiView.getY(),
      80,
      25);
  topClearButton.setBounds(
      midiView.getRight() + horizontalMargin,
      topRecordButton.getBottom() + verticalMargin,
      80,
      25);

  numBarsLabel.setBounds(
      midiView.getRight() + horizontalMargin,
      topClearButton.getBottom() + verticalMargin,
      50, 20);
  numBarsInput.setBounds(
      numBarsLabel.getRight() + horizontalMargin,
      numBarsLabel.getBounds().getY(),
      30, 20);

  bpmLabel.setBounds(
      midiView.getRight() + horizontalMargin, 
      numBarsLabel.getBottom() + verticalMargin, 
      50, 20);
  bpmInput.setBounds(
      bpmLabel.getRight() + horizontalMargin,
      bpmLabel.getBounds().getY(),
      30, 20);

  quantizeButton.setBounds(
      midiView.getRight() + horizontalMargin, 
      bpmLabel.getBottom() + verticalMargin,
      80,
      25);

  quantizationSelector.setBounds(
      quantizeButton.getRight() + horizontalMargin, 
      quantizeButton.getBounds().getY(),
      80,
      25);

  bottomRecordButton.setBounds(
      midiView.getRight() + horizontalMargin, 
      midiView.bottomScrollerView.getBounds().getY() + verticalMargin,
      80,
      25);
  bottomClearButton.setBounds(
      midiView.getRight() + horizontalMargin, 
      bottomRecordButton.getBottom() + verticalMargin,
      80,
      25);

  precisionAnalytics.setBounds(
    midiView.getRight() + horizontalMargin, 
    bottomClearButton.getBottom() + verticalMargin,
    getWidth() - midiView.getRight() - 2*horizontalMargin, 
    midiView.getBottom() - bottomClearButton.getBottom() - verticalMargin);
}
