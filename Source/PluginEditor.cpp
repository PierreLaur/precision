/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MidiGrid.h"
#include "Utils.h"
#include <filesystem>
#include <iostream>
#include <fstream>

using namespace juce;

//==============================================================================
PrecisionAudioProcessorEditor::PrecisionAudioProcessorEditor(PrecisionAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
  setSize(defaultWidth, defaultHeight);
  setResizable(true, true);

  setupButtons();

  addAndMakeVisible(viewPort);
  grid.setSize(1200, NOTE_HEIGHT * 128);
  viewPort.setViewedComponent(&grid, false);
  viewPort.setScrollOnDragMode(Viewport::ScrollOnDragMode::all);
}

void PrecisionAudioProcessorEditor::setupButtons()
{
  horizontalZoomIn.setColour(juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
  horizontalZoomOut.setColour(juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
  verticalZoomIn.setColour(juce::TextButton::buttonColourId, juce::Colours::greenyellow);
  verticalZoomOut.setColour(juce::TextButton::buttonColourId, juce::Colours::greenyellow);
  quantizeButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);


  horizontalZoomIn.setButtonText("+");
  horizontalZoomOut.setButtonText("-");
  verticalZoomIn.setButtonText("+");
  verticalZoomOut.setButtonText("-");
  quantizeButton.setButtonText("Q");


  horizontalZoomIn.addListener(this);
  horizontalZoomOut.addListener(this);
  verticalZoomIn.addListener(this);
  verticalZoomOut.addListener(this);
  quantizeButton.addListener(this);


  addAndMakeVisible(horizontalZoomIn);
  addAndMakeVisible(horizontalZoomOut);
  addAndMakeVisible(verticalZoomIn);
  addAndMakeVisible(verticalZoomOut);
  addAndMakeVisible(quantizeButton);
}

PrecisionAudioProcessorEditor::~PrecisionAudioProcessorEditor()
{
}

//==============================================================================
void PrecisionAudioProcessorEditor::paint(Graphics &g)
{

  g.fillAll(Colours::maroon);
  // Reading a MIDI clip
  String dataPath = "C:\\Users\\pierr\\precision_project\\precision\\data";
  MidiFile midiFile = readMidiFile(dataPath + "\\simple.mid");
  grid.storeMidiNotes(midiFile);
}

void PrecisionAudioProcessorEditor::buttonClicked(Button *button)
{
  if (button == &horizontalZoomIn)
    widthMultiplier += 0.1f;
  else if (button == &horizontalZoomOut)
    widthMultiplier -= 0.1f;
  else if (button == &verticalZoomIn)
    heightMultiplier += 0.1f;
  else if (button == &verticalZoomOut)
    heightMultiplier -= 0.1f;
  else if (button == &quantizeButton)
    grid.quantize();
  scaler = scaler.scale(widthMultiplier, heightMultiplier);
  grid.setTransform(scaler);

  // TODO : limit zoom so that the view stays the same size
}

void PrecisionAudioProcessorEditor::automaticZoom()
{
  // TODO
}

void PrecisionAudioProcessorEditor::resized()
{
  auto area = getLocalBounds();

  viewPort.setBounds(
      20, 20,
      area.getWidth() - 150,
      200);

  horizontalZoomIn.setBounds(
      viewPort.getRight() + 50,
      viewPort.getY() + 20,
      30,
      30);

  horizontalZoomOut.setBounds(
      viewPort.getRight() + 90,
      viewPort.getY() + 20,
      30,
      30);

  verticalZoomIn.setBounds(
      viewPort.getRight() + 50,
      viewPort.getY() + 60,
      30,
      30);

  verticalZoomOut.setBounds(
      viewPort.getRight() + 90,
      viewPort.getY() + 60,
      30,
      30);

  quantizeButton.setBounds(
      viewPort.getRight() + 50,
      viewPort.getY() + 120,
      30,
      30);
}
