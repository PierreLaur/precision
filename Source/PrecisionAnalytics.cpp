/*
  ==============================================================================

    PrecisionAnalytics.cpp
    Created: 25 Dec 2022 2:31:41pm
    Author:  pierr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PrecisionAnalytics.h"

//==============================================================================
PrecisionAnalytics::PrecisionAnalytics()
{
  precisionAnalytics.setText(
      "Precision Analytics : \n\n\n\
    Average Absolute Deviation : " +
          String(averageAbsoluteDeviationMs) + " ms\n\
    Average Deviation : " +
          String(averageDeviationMs) + " ms",
      dontSendNotification);
  precisionAnalytics.setJustificationType(Justification::centredTop);
  addAndMakeVisible(precisionAnalytics);
}

PrecisionAnalytics::~PrecisionAnalytics()
{
}

void PrecisionAnalytics::update(double newAverageAbsoluteDeviationMs, double newAverageDeviationMs,
                                double newAverageAbsoluteDeviationToLengthMs, double newAverageDeviationToLengthMs)
{
  averageAbsoluteDeviationMs = newAverageAbsoluteDeviationMs;
  averageDeviationMs = newAverageDeviationMs;
  averageAbsoluteDeviationToLengthMs = newAverageAbsoluteDeviationToLengthMs;
  averageDeviationToLengthMs = newAverageDeviationToLengthMs;
  repaint();
}

void PrecisionAnalytics::paint(Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); // clear the background

  precisionAnalytics.setText(
      "Precision Analytics : \n\n\n\
    Average Absolute Deviation : " +
          String(averageAbsoluteDeviationMs) + " ms\n\
    Average Deviation : " +
          String(averageDeviationMs) + " ms",
      dontSendNotification);
  precisionAnalytics.setJustificationType(Justification::centredTop);
}

void PrecisionAnalytics::resized()
{
  precisionAnalytics.setBounds(getLocalBounds());
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
