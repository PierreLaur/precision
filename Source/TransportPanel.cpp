/*
  ==============================================================================

    TransportPanel.cpp
    Created: 5 Mar 2023 9:28:30pm
    Author:  pierr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TransportPanel.h"

//==============================================================================
TransportPanel::TransportPanel()
{
  startTimer(60);
  addAndMakeVisible(label);
}

TransportPanel::~TransportPanel()
{
}

void TransportPanel::paint(juce::Graphics &g)
{
  g.fillAll(Colours::grey);
}

void TransportPanel::resized()
{
  label.setBounds(getLocalBounds());
}
