/*
  ==============================================================================

    LinkableViewport.cpp
    Created: 24 Nov 2022 11:05:39pm
    Author:  pierr

  ==============================================================================
*/

#include "LinkableViewport.h"

using namespace juce;

LinkableViewport::LinkableViewport() : Viewport::Viewport("noName") {
}

void LinkableViewport::visibleAreaChanged (const Rectangle<int>& newVisibleArea) {
  Viewport::visibleAreaChanged(newVisibleArea) ;
  for (auto view : linkedViewports) {
    view->setViewPosition(getViewArea().getTopLeft()) ;
  }
}

void LinkableViewport::linkViewport(Viewport * view) {
  auto it = linkedViewports.begin() ;
  linkedViewports.insert(it,view) ;
}