/*
  ==============================================================================

    LinkableViewport.h
    Created: 24 Nov 2022 11:05:39pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <list>


using namespace juce;
class MidiView;

class LinkableViewport : public Viewport
{
public:
  LinkableViewport(MidiView&);

  void visibleAreaChanged(const Rectangle<int> &newVisibleArea) override;
  bool useMouseWheelMoveIfNeeded(const MouseEvent &, const MouseWheelDetails &) override;

  void linkViewport(Viewport *view);
  std::list<Viewport *> linkedViewports;

  MidiView &midiView;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinkableViewport)
};
