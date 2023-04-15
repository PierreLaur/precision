/*
  ==============================================================================

    ControlPanel.h
    Created: 11 Apr 2023 5:34:27pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class PrecisionAudioProcessorEditor;

//==============================================================================
/*
 */
class ControlPanel : public Component, public Button::Listener
{
public:
  ControlPanel();
  ~ControlPanel() override;

  void setupButtons();
  void quantizationChanged();
  void buttonClicked(Button *button) override;

  int getBpmFromString(String text);
  int getNumBarsFromString(String text);

  void paint(Graphics &) override;
  void resized() override;

  std::function<void(Button *)> onUpdate;

  TextButton recordButton;
  TextButton clearButton;

  Label numBarsLabel;
  Label numBarsInput;
  Label bpmLabel;
  Label bpmInput;

  ComboBox quantizationSelector;

  TextButton filterButton;
  Slider precisionSlider;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlPanel)
};
