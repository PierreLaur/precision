/*
  ==============================================================================

    ControlPanel.cpp
    Created: 11 Apr 2023 5:34:27pm
    Author:  pierr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ControlPanel.h"
#include "PluginEditor.h"

//==============================================================================
ControlPanel::ControlPanel()
{
  setupButtons();
}

ControlPanel::~ControlPanel()
{
}

int ControlPanel::getBpmFromString(String text)
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

int ControlPanel::getNumBarsFromString(String text)
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

void ControlPanel::quantizationChanged()
{
  quantizationInBeats = 4.0f / (float)quantizationSelector.getSelectedId();
  repaint();
}

void ControlPanel::setupButtons()
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

  addAndMakeVisible(numBarsInput);
  numBarsInput.setText(String(numBars), dontSendNotification);
  numBarsInput.setJustificationType(Justification::left);
  numBarsInput.setColour(juce::Label::backgroundColourId, juce::Colours::grey);
  numBarsInput.setEditable(true);

  addAndMakeVisible(bpmInput);
  bpmInput.setText(String(bpm), dontSendNotification);
  bpmInput.setJustificationType(Justification::left);
  bpmInput.setColour(juce::Label::backgroundColourId, juce::Colours::grey);
  bpmInput.setEditable(true);

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

void ControlPanel::buttonClicked(Button *button)
{
  onUpdate(button);
}

void ControlPanel::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background
  g.setColour(juce::Colours::black);
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component
}

void ControlPanel::resized()
{

  auto area = getLocalBounds();
  int horizontalMargin = 10;
  int verticalMargin = 10;

  topRecordButton.setBounds(
      horizontalMargin,
      verticalMargin,
      80,
      25);
  topClearButton.setBounds(
      horizontalMargin,
      topRecordButton.getBottom() + verticalMargin,
      80,
      25);

  numBarsLabel.setBounds(
      horizontalMargin,
      topClearButton.getBottom() + verticalMargin,
      50, 20);
  numBarsInput.setBounds(
      numBarsLabel.getRight() + horizontalMargin,
      numBarsLabel.getBounds().getY(),
      30, 20);

  bpmLabel.setBounds(
      horizontalMargin,
      numBarsLabel.getBottom() + verticalMargin,
      50, 20);
  bpmInput.setBounds(
      bpmLabel.getRight() + horizontalMargin,
      bpmLabel.getBounds().getY(),
      30, 20);

  quantizeButton.setBounds(
      horizontalMargin,
      bpmLabel.getBottom() + verticalMargin,
      80,
      25);

  quantizationSelector.setBounds(
      quantizeButton.getRight() + horizontalMargin,
      quantizeButton.getBounds().getY(),
      80,
      25);

  bottomClearButton.setBounds(
      horizontalMargin,
      area.getBottom() - verticalMargin - 25,
      80,
      25);

  bottomRecordButton.setBounds(
      horizontalMargin,
      bottomClearButton.getY() - 25 - verticalMargin,
      80,
      25);
}
