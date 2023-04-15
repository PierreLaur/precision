/*
  ==============================================================================

    MidiView.cpp
    Created: 5 Apr 2023 7:37:37pm
    Author:  pierr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MidiView.h"
#include "MidiGrid.h"

//==============================================================================
MidiView::MidiView(PrecisionAnalytics &analytics) : analytics{analytics}
{

  addAndMakeVisible(gridView);
  addAndMakeVisible(pianoView);
  addAndMakeVisible(scrollerView);

  // Synchronize scrolling on the viewports
  gridView.linkViewport(&pianoView);
  gridView.linkViewport(&scrollerView);

  // Set viewed components
  gridView.setViewedComponent(&grid, false);
  scrollerView.setViewedComponent(&scroller, false);
  pianoView.setViewedComponent(&piano, false);

  // set the view in the center of the grid
  gridView.setViewPosition(Point(0, grid.getHeight() / 2));

  // Hide the vertical scrollbar, show the horizontal one
  gridView.setScrollOnDragMode(Viewport::ScrollOnDragMode::all);
  gridView.setScrollBarsShown(false, true, true);
  pianoView.setScrollOnDragMode(Viewport::ScrollOnDragMode::never);
  pianoView.setScrollBarsShown(false, false);
  scrollerView.setScrollOnDragMode(Viewport::ScrollOnDragMode::never);
  scrollerView.setScrollBarsShown(false, false);
}

MidiView::~MidiView()
{
}

void MidiView::verticalZoom(const MouseWheelDetails wheel)
{
  if (wheel.deltaY > 0)
  {
    heightMultiplier = std::min(1.2f * heightMultiplier, maxHeightMultiplier);
  }
  else
  {
    heightMultiplier = std::max(heightMultiplier * 0.8f, minHeightMultiplier);
  }
  scaleView();
}

void MidiView::horizontalZoom(const MouseWheelDetails wheel)
// TODO : zooming/unzooming with touchpad
{
  if (wheel.deltaY > 0)
  {
    widthMultiplier = std::min(1.2f * widthMultiplier, maxWidthMultiplier);
  }
  else
  {
    widthMultiplier = std::max(widthMultiplier * 0.8f, minWidthMultiplier);
  }
  scaleView();
}

void MidiView::automaticZoom()
{
  // TODO : adapt zoom to the notes on the grid
  // TODO : follow the cursor
}

void MidiView::paint(juce::Graphics &g)
{
  g.setColour(juce::Colours::black);
  // g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
  // g.fillRect (getLocalBounds());
}

void MidiView::setMinMaxMultipliers()
{
  minWidthMultiplier = (float)(gridView.getWidth()) / grid.getWidth();
  maxWidthMultiplier = (float)numBars * 4;
  minHeightMultiplier = (float)(gridView.getHeight() - gridView.getHorizontalScrollBar().getHeight()) / grid.getHeight();
  maxHeightMultiplier = 10.0f;
}

void MidiView::resized()
{

  auto area = getLocalBounds();

  grid.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 128 * NOTE_HEIGHT);
  scroller.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 30);
  piano.setSize(30, NOTE_HEIGHT * 128);

  scrollerView.setBounds(
      piano.getWidth(), 0,
      area.getWidth() - piano.getWidth(),
      scroller.getHeight());

  pianoView.setBounds(
      0, scrollerView.getBottom(),
      piano.getWidth(),
      area.getHeight() - scroller.getHeight());

  gridView.setBounds(
      pianoView.getRight(),
      scrollerView.getBottom(),
      scrollerView.getWidth(),
      pianoView.getHeight());

  setMinMaxMultipliers();
}
