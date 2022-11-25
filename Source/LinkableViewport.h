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

class LinkableViewport : public Viewport
{
public:
  LinkableViewport() ;

  void visibleAreaChanged (const Rectangle<int>& newVisibleArea) override ;

  void linkViewport(Viewport * view) ;
  std::list<Viewport *> linkedViewports ;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinkableViewport)
};
