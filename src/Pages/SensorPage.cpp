#include "Pages/SensorPage.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include "Main/ActivePage.h"

// Main JUCE component
SensorPage::SensorPage(GroundCrewDisplay::MainWindow *window) : mainWindow(window), _wind("Wind MPH", 0.0f, 40.0f, Colour(253, 185, 19)), _windGraph(FRAMERATE * 5, 0.0f, 40.0f),
                                                            _pageSwitcher(window, ActivePage::SensorPage)
{
  addAndMakeVisible(_wind);
  addAndMakeVisible(_windGraph);

  _wind.setData(20.0f);
  _windGraph.addData(20.0f);

  setSize(getParentWidth(), getParentHeight());
  setFramesPerSecond(FRAMERATE);
}

SensorPage::~SensorPage()
{
}

void SensorPage::update()
{
  static float randWind = 0.0f;

  _wind.setData(20.0f + randWind);
  _windGraph.addData(20.0f + randWind);

  Random &rand = Random::getSystemRandom();
  randWind += rand.nextFloat() * -(rand.nextBool() * 2 - 1);
}

// Function executes every frame
void SensorPage::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
}

void SensorPage::resized()
{
  _windGraph.setBounds(0, 5, 230, 150);

  _wind.setBounds(0, 115, 230, 280);

  _pageSwitcher.setBounds(getWidth() - 100, 0, 100, 100);
}