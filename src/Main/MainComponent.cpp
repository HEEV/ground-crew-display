#include "Main/MainComponent.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>

// Main JUCE component
MainComponent::MainComponent() : _wind("Wind MPH", 0.0f, 40.0f, Colour(253, 185, 19)), _windGraph(FRAMERATE * 5)
{
  addAndMakeVisible(_wind);
  addAndMakeVisible(_windGraph);
  _wind.setData(20.0f);
  _windGraph.addData(20.0f);

  setSize(getParentWidth(), getParentHeight());
  setFramesPerSecond(FRAMERATE);

  addMouseListener(&_mouse, true);
}

void MainComponent::update()
{
  static float randWind = 0.0f;

  _wind.setData(20.0f + randWind);
  _windGraph.addData(20.0f + randWind);

  Random &rand = Random::getSystemRandom();
  randWind += rand.nextFloat() * -(rand.nextBool() * 2 - 1);
  _wind.repaint();
}

MainComponent::~MainComponent()
{
}

// Function executes every frame
void MainComponent::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
}

void MainComponent::resized()
{
  // _wind.setBounds (0, 0, getWidth(), getHeight() / 2);
  _windGraph.setBounds(0, 0, getWidth(), getHeight());
}

void MainComponent::MouseEvents::mouseDown(const MouseEvent &e)
{
}

void MainComponent::MouseEvents::mouseDoubleClick(const MouseEvent &e)
{
  JUCEApplicationBase::quit();
}