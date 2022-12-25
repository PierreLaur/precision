/*
  ==============================================================================

    PrecisionAnalytics.h
    Created: 25 Dec 2022 2:31:41pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;
//==============================================================================
/*
 */
class PrecisionAnalytics : public Component
{
public:
  PrecisionAnalytics();
  ~PrecisionAnalytics() override;

  void update(double newAverageAbsoluteDeviationMs, double newAverageDeviationMs,
              double newAverageAbsoluteDeviationToLengthMs, double newAverageDeviationToLengthMs);

  Label precisionAnalytics;

  double averageAbsoluteDeviationMs = 0.0;
  double averageDeviationMs = 0.0;
  double averageAbsoluteDeviationToLengthMs = 0.0;
  double averageDeviationToLengthMs = 0.0;

  void paint(Graphics &) override;
  void resized() override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrecisionAnalytics)
};
