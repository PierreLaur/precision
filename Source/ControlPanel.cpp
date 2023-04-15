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
  recordButton.setColour(TextButton::buttonColourId, Colours::red);
  recordButton.setButtonText("Record");
  recordButton.addListener(this);
  addAndMakeVisible(recordButton);

  clearButton.setColour(TextButton::buttonColourId, Colours::blue);
  clearButton.setButtonText("Clear");
  clearButton.addListener(this);
  addAndMakeVisible(clearButton);

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
  quantizationSelector.addItem("1/2 t", 3);
  quantizationSelector.addItem("1/4", 4);
  quantizationSelector.addItem("1/4 t", 6);
  quantizationSelector.addItem("1/8", 8);
  quantizationSelector.addItem("1/8 t", 12);
  quantizationSelector.addItem("1/16", 16);
  quantizationSelector.addItem("1/16 t", 24);
  quantizationSelector.addItem("1/32", 32);

  addAndMakeVisible(quantizationSelector);
  quantizationSelector.setSelectedId(4);
  quantizationInBeats = 1.0f;
  quantizationSelector.onChange = [this]
  { quantizationChanged(); };

  filterButton.setColour(TextButton::buttonColourId, Colours::purple);
  filterButton.setButtonText("Filter");
  filterButton.addListener(this);
  filterButton.setToggleable(true);
  filterButton.setClickingTogglesState(true);
  addAndMakeVisible(filterButton);

  precisionSlider.setSliderStyle(Slider::Rotary);
  precisionSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
  precisionSlider.setNumDecimalPlacesToDisplay(0);
  precisionSlider.setTextValueSuffix(String(" ms"));
  precisionSlider.setRange(0.0, 200.0);
  precisionSlider.onValueChange = [this]
  { precisionLimitMs = precisionSlider.getValue(); };
  addAndMakeVisible(precisionSlider);
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

  recordButton.setBounds(
      horizontalMargin,
      verticalMargin,
      (int)(area.getWidth() * 0.3),
      (int)(area.getWidth() * 0.06));
  clearButton.setBounds(
      horizontalMargin,
      recordButton.getBottom() + verticalMargin,
      recordButton.getWidth(),
      (int)(area.getWidth() * 0.06));

  filterButton.setBounds(
      (int)((area.getWidth() + recordButton.getRight()) / 2) - (int)(area.getWidth() * 0.15),
      recordButton.getY(),
      (int)(area.getWidth() * 0.3),
      (int)(area.getWidth() * 0.06));
  precisionSlider.setBounds(
      recordButton.getRight() + horizontalMargin,
      recordButton.getBottom(),
      (int)(area.getWidth() * 0.7 - horizontalMargin * 2),
      (int)(area.getHeight() * 0.5));

  numBarsLabel.setBounds(
      horizontalMargin,
      (int)(area.getHeight() * 0.3),
      (int)(area.getWidth() * 0.18), 20);
  numBarsInput.setBounds(
      numBarsLabel.getRight() + horizontalMargin,
      numBarsLabel.getBounds().getY(),
      (int)(area.getWidth() * 0.12),
      (int)(area.getHeight() * 0.06));

  bpmLabel.setBounds(
      horizontalMargin,
      numBarsLabel.getBottom() + verticalMargin,
      (int)(area.getWidth() * 0.18), (int)(area.getHeight() * 0.06));
  bpmInput.setBounds(
      bpmLabel.getRight() + horizontalMargin,
      bpmLabel.getBounds().getY(),
      (int)(area.getWidth() * 0.12), (int)(area.getHeight() * 0.06));

  quantizationSelector.setBounds(
      horizontalMargin,
      bpmInput.getBounds().getBottom() + verticalMargin,
      (int)(area.getWidth() * 0.3),
      (int)(area.getHeight() * 0.06));
}
