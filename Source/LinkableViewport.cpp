/*
  ==============================================================================

    LinkableViewport.cpp
    Created: 24 Nov 2022 11:05:39pm
    Author:  pierr

  ==============================================================================
*/

#include "LinkableViewport.h"
#include "MidiView.h"

LinkableViewport::LinkableViewport(MidiView &midiView) : Viewport::Viewport("noName"), midiView{midiView}
{
  setSingleStepSizes(8, 8);
}

// Synchronizes the viewports - scrolling on one will move the others
void LinkableViewport::visibleAreaChanged(const Rectangle<int> &newVisibleArea)
{
  Viewport::visibleAreaChanged(newVisibleArea);

  for (auto view : linkedViewports)
  {
    view->setViewPosition(getViewArea().getTopLeft());
  }
}

// Handles mouse wheel messages with and without Ctrl / Alt keys
bool LinkableViewport::useMouseWheelMoveIfNeeded(const MouseEvent &e, const MouseWheelDetails &wheel)
{
  if (e.mods.isCtrlDown() || e.mods.isCommandDown())
  {
    // TODO
    midiView.horizontalZoom(wheel);
    return true;
  }
  else if (e.mods.isAltDown())
  {
    // TODO
    midiView.verticalZoom(wheel);
    return true;
  }
  else
  {
    return Viewport::useMouseWheelMoveIfNeeded(e, wheel);
  }
}

// Adds a viewport to scroll when this one is scrolled
void LinkableViewport::linkViewport(Viewport *view)
{
  auto it = linkedViewports.begin();
  linkedViewports.insert(it, view);
}