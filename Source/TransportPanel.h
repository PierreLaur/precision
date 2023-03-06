/*
  ==============================================================================

    TransportPanel.h
    Created: 5 Mar 2023 9:28:29pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Utils.h"
using namespace juce;

//==============================================================================
/*
*/
class TransportPanel  : public Component, Timer
{
public:
    TransportPanel();
    ~TransportPanel() override;

    void paint (Graphics&) override;
    void resized() override;

     // TODO : revert this
    void timerCallback() override {
        // label.setText(String(ppqPosition), dontSendNotification);
        label.setText(myMessage, dontSendNotification);
    }

    Label label;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportPanel)
};
