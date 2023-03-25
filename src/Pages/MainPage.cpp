#include "Pages/MainPage.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include "Main/ActivePage.h"
#include "Main/Sources.h"

// Main JUCE component
MainPage::MainPage(GroundCrewDisplay::MainWindow *window) : mainWindow(window), _wind("Wind MPH", 0.0f, 40.0f, Colour(253, 185, 19)), _windGraph(&Sources::wind, 0.0f, 40.0f),
                                                            _map("Tracks/ShellTrack.svg", 1.0f), _engTemp(&Sources::engTemp, true),
                                                            _speed("Vehicle MPH", 0.0f, 40.0f, Colour(253, 185, 19), 6), _speedGraph(&Sources::speed, false, 20000), _pageSwitcher(window, ActivePage::MainPage)
{
  addAndMakeVisible(_wind);
  addAndMakeVisible(_windGraph);

  addAndMakeVisible(_speed);
  addAndMakeVisible(_speedGraph);

  addAndMakeVisible(_map);
  addAndMakeVisible(_engTemp);

  addAndMakeVisible(_pageSwitcher);

  Sources::speed.addData(duration_cast<std::chrono::milliseconds>(
                             std::chrono::system_clock::now().time_since_epoch())
                             .count(),
                         20.0f);
  _speed.setData(Sources::speed.last());

  Sources::engTemp.addData(duration_cast<std::chrono::milliseconds>(
                               std::chrono::system_clock::now().time_since_epoch())
                               .count(),
                           451.0f);

  Sources::wind.addData(duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count(),
                        20.0f);
  _wind.setData(Sources::wind.last());

  setSize(getParentWidth(), getParentHeight());
  setFramesPerSecond(FRAMERATE);
}

void MainPage::update()
{
  Random &rand = Random::getSystemRandom();

  static float randWind = 0.0f;
  static float randSpeed = 0.0f;
  static float randTemp = 0.0f;

  uint64_t now = duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now().time_since_epoch())
                     .count();

  if (rand.nextFloat() < 0.26)
  {
    Sources::wind.addData(now, 20.0f + randWind);
    _wind.setData(Sources::wind.last());
  }

  if (rand.nextFloat() < 0.26)
  {
    Sources::speed.addData(now, 20.0f + randWind);
    _speed.setData(Sources::speed.last());
  }

  if (rand.nextFloat() < 0.26)
  {
    Sources::engTemp.addData(now, 451.0f + randTemp);
  }

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