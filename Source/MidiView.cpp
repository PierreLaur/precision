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
MidiView::MidiView(PrecisionAnalytics & analytics) : analytics{analytics}
{

  addAndMakeVisible(topGridView);
  addAndMakeVisible(bottomGridView);
  addAndMakeVisible(topPianoView);

  addAndMakeVisible(bottomPianoView);
  addAndMakeVisible(topScrollerView);
  addAndMakeVisible(bottomScrollerView);

  // Synchronize scrolling on both viewports
  topGridView.linkViewport(&topPianoView);
  topGridView.linkViewport(&bottomPianoView);
  topGridView.linkViewport(&bottomGridView);
  topGridView.linkViewport(&topScrollerView);
  topGridView.linkViewport(&bottomScrollerView);
  bottomGridView.linkViewport(&topPianoView);
  bottomGridView.linkViewport(&bottomPianoView);
  bottomGridView.linkViewport(&topGridView);
  bottomGridView.linkViewport(&topScrollerView);
  bottomGridView.linkViewport(&bottomScrollerView);

  // Set viewed components
  topGridView.setViewedComponent(&topGrid, false);
  bottomGridView.setViewedComponent(&bottomGrid, false);
  topScrollerView.setViewedComponent(&topScroller, false);
  bottomScrollerView.setViewedComponent(&bottomScroller, false);
  topPianoView.setViewedComponent(&topPiano, false);
  bottomPianoView.setViewedComponent(&bottomPiano, false);

  // centers of the grids
  topGridView.setViewPosition(Point(0, topGrid.getHeight() / 2));
  bottomGridView.setViewPosition(Point(0, bottomGrid.getHeight() / 2));

  // Hide the vertical scrollbar, show the horizontal one
  topGridView.setScrollOnDragMode(Viewport::ScrollOnDragMode::all);
  topGridView.setScrollBarsShown(false, true, true);
  bottomGridView.setScrollOnDragMode(Viewport::ScrollOnDragMode::all);
  bottomGridView.setScrollBarsShown(false, true, true);

  topPianoView.setScrollOnDragMode(Viewport::ScrollOnDragMode::never);
  topPianoView.setScrollBarsShown(false, false);
  bottomPianoView.setScrollOnDragMode(Viewport::ScrollOnDragMode::never);
  bottomPianoView.setScrollBarsShown(false, false);
  topScrollerView.setScrollOnDragMode(Viewport::ScrollOnDragMode::never);
  topScrollerView.setScrollBarsShown(false, false);
  bottomScrollerView.setScrollOnDragMode(Viewport::ScrollOnDragMode::never);
  bottomScrollerView.setScrollBarsShown(false, false);

  bottomGrid.modelGrid = &topGrid;

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


void MidiView::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::black);
    // g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    // g.fillRect (getLocalBounds());
}

void MidiView::resized()
{

  auto area = getLocalBounds();

  topGrid.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 128 * NOTE_HEIGHT);
  bottomGrid.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 128 * NOTE_HEIGHT);
  topScroller.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 30);
  bottomScroller.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 30);
  topPiano.setSize(30, NOTE_HEIGHT * 128);
  bottomPiano.setSize(30, NOTE_HEIGHT * 128);

  topScrollerView.setBounds(
      topPiano.getWidth(), 0,
      area.getWidth() - topPiano.getWidth(),
      topScroller.getHeight());

  topPianoView.setBounds(
      0, topScrollerView.getBottom(),
      topPiano.getWidth(),
      (int)(area.getHeight()*0.4-topScroller.getHeight()));

  topGridView.setBounds(
      topPianoView.getRight(), topScrollerView.getBottom(),
      topScrollerView.getWidth(),
      topPianoView.getHeight());

  bottomScrollerView.setBounds(
      topPiano.getWidth(), topPianoView.getBottom(),
      topScrollerView.getWidth(),
      topScrollerView.getHeight());

  bottomPianoView.setBounds(
      0, bottomScrollerView.getBottom(),
      topPianoView.getWidth(),
      area.getBottom() - bottomScrollerView.getBottom());

  bottomGridView.setBounds(
      bottomPianoView.getRight(), 
      bottomScrollerView.getBottom(),
      topGridView.getWidth(),
      bottomPianoView.getHeight());

  minWidthMultiplier = (float)(bottomGridView.getWidth()) / bottomGrid.getWidth();
  maxWidthMultiplier = (float)numBars*4;
  minHeightMultiplier = (float)(bottomGridView.getHeight() - bottomGridView.getHorizontalScrollBar().getHeight()) / bottomGrid.getHeight();
  maxHeightMultiplier = 10.0f;
}
