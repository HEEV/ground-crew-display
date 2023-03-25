#include "Pages/MainPage.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include "Main/ActivePage.h"

// Main JUCE component
MainPage::MainPage(GroundCrewDisplay::MainWindow* window) : mainWindow(window), _wind("Wind MPH", 0.0f, 40.0f, Colour(253, 185, 19)), _windGraph(FRAMERATE * 5, 0.0f, 40.0f),
                                 _map("Tracks/ShellTrack.svg", 1.0f), _engTemp(FRAMERATE * 20), 
                                 _speed("Vehicle MPH", 0.0f, 40.0f, Colour(253, 185, 19), 6), _speedGraph(FRAMERATE * 5, 0.0f, 40.0f), _pageSwitcher(window, ActivePage::MainPage)
{
  addAndMakeVisible(_wind);
  addAndMakeVisible(_windGraph);

  addAndMakeVisible(_speed);
  addAndMakeVisible(_speedGraph);

  addAndMakeVisible(_map);
  addAndMakeVisible(_engTemp);

  addAndMakeVisible(_pageSwitcher);

  _wind.setData(20.0f);
  _windGraph.addData(20.0f);

  _speedGraph.addData(20.0f);
  _speed.setData(20.0f);

  setSize(getParentWidth(), getParentHeight());
  setFramesPerSecond(FRAMERATE);
}

void MainPage::update()
{
  static float randWind = 0.0f;
  static float randSpeed = 0.0f;
  static float randTemp = 0.0f;

  _wind.setData(20.0f + randWind);
  _windGraph.addData(20.0f + randWind);

  _speed.setData(20.0f + randSpeed);
  _speedGraph.addData(20.0f + randSpeed);

  _engTemp.addData(421.0f + randTemp);

  Random &rand = Random::getSystemRandom();
  randWind += rand.nextFloat() * -(rand.nextBool() * 2 - 1);
  randSpeed += rand.nextFloat() * -(rand.nextBool() * 2 - 1);
  randTemp += rand.nextFloat() * -(rand.nextBool() * 2 - 1) * 3;
}

MainPage::~MainPage()
{
}

// Function executes every frame
void MainPage::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
}

void MainPage::resized()
{
  _windGraph.setBounds(0, 5, 230, 150);
  _speedGraph.setBounds(250, 5, 230, 150);
  _engTemp.setBounds(510, 5, 230, 150);

  _wind.setBounds(0, 115, 230, 280);
  _speed.setBounds(250, 115, 230, 280);

  _map.setBounds(0, getHeight() - 150, 300, 150);

  _pageSwitcher.setBounds(getWidth() - 100, 0, 100, 100);
}