/*
  ==============================================================================

    LinkableViewport.cpp
    Created: 24 Nov 2022 11:05:39pm
    Author:  pierr

  ==============================================================================
*/

#include "LinkableViewport.h"
#include "PluginEditor.h"

using namespace juce;
class PrecisionAudioProcessorEditor;

LinkableViewport::LinkableViewport(PrecisionAudioProcessorEditor &editor) : Viewport::Viewport("noName"), editor{editor}
{
  setSingleStepSizes(5, 5);
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
    editor.horizontalZoom(wheel);
    return true;
  }
  else if (e.mods.isAltDown())
  {
    editor.verticalZoom(wheel);
    return true;
  }
  else
  {
    return Viewport::useMouseWheelMoveIfNeeded(e, wheel);
  }
}

// Add a viewport to scroll when this one is scrolled
void LinkableViewport::linkViewport(Viewport *view)
{
  auto it = linkedViewports.begin();
  linkedViewports.insert(it, view);
}