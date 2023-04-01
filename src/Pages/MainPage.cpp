#include "Pages/MainPage.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <Packets.h>
#include "Main/ActivePage.h"
#include "DataSources/Sources.h"

// Main JUCE component
MainPage::MainPage(GroundCrewDisplay::MainWindow *window) : Page(window, ActivePage::MainPage), _wind(&Sources::wind, -40.0f, 40.0f), _windGraph(&Sources::wind, -40.0f, 40.0f, 250000),
                                                            _map(&Sources::distanceTraveled, "Tracks/ShellTrack.svg", 1.0f), _engTemp(&Sources::engTemp, false, 250000),
                                                            _speed(&Sources::velocity, 0.0f, 40.0f), _speedGraph(&Sources::velocity, false, 30000), _lapCounter(&Sources::velocity, 1, 4)
{
  addAndMakeVisible(_wind);
  addAndMakeVisible(_windGraph);

  addAndMakeVisible(_speed);
  addAndMakeVisible(_speedGraph);

  addAndMakeVisible(_map);
  addAndMakeVisible(_engTemp);

  addAndMakeVisible(_lapCounter);

  addAndMakeVisible(_info);
}

void MainPage::resized()
{
  _windGraph.setBounds(0, 5, 230, 150);
  _speedGraph.setBounds(250, 5, 230, 150);
  _engTemp.setBounds(510, 5, 230, 150);

  _wind.setBounds(0, 115, 230, 280);
  _speed.setBounds(250, 115, 230, 280);

  _map.setBounds(0, getHeight() - 150, 300, 150);

  _lapCounter.setBounds(400, getHeight() - 150, 300, 150);

  _info.setBounds(750, getHeight() - 150, 350, 150);

  displayPageSwitcher();
}      