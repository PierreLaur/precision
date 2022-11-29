/*
  ==============================================================================

    NoteBorder.cpp
    Created: 29 Nov 2022 3:51:31pm
    Author:  pierr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "NoteBorder.h"
#include "Utils.h"
#include "MidiNote.h"

using namespace juce;
class MidiNote;

NoteBorder::NoteBorder(Component *componentToResize, ComponentBoundsConstrainer *constrainer, MidiNote & parentNote)
    : ResizableBorderComponent(componentToResize, constrainer), parentNote{parentNote}
{
}

void NoteBorder::setBorderThickness(BorderSize<int> newBorderSize)
{
  if (onlySides)
  {
    newBorderSize.setTop(0);
    newBorderSize.setBottom(0);
  }
  ResizableBorderComponent::setBorderThickness(newBorderSize);
}

void NoteBorder::mouseDrag(const MouseEvent &e)
{

  int horizontalSnap = static_cast<int>(BEAT_LENGTH_TIMESTEPS * quantizationInBeats);
  ResizableBorderComponent::mouseDrag(e, horizontalSnap);
  parentNote.updateConstraints();
  std::cout << constrainer->getMaximumWidth() << std::endl;
}