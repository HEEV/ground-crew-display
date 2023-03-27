#include "Pages/MainPage.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <Packets.h>
#include "Main/ActivePage.h"
#include "Main/Sources.h"

// Main JUCE component
MainPage::MainPage(GroundCrewDisplay::MainWindow *window) : Page(window, ActivePage::MainPage), _wind("Wind MPH", 0.0f, 40.0f, Colour(253, 185, 19)), _windGraph(&Sources::wind, 0.0f, 40.0f),
                                                            _map("Tracks/ShellTrack.svg", 1.0f), _engTemp(&Sources::engTemp),
                                                            _speed("Vehicle MPH", 0.0f, 40.0f, Colour(253, 185, 19), 6), _speedGraph(&Sources::speed, false, 3000), _man("163.11.237.241:5001")
{
  addAndMakeVisible(_wind);
  addAndMakeVisible(_windGraph);

  addAndMakeVisible(_speed);
  addAndMakeVisible(_speedGraph);

  addAndMakeVisible(_map);
  addAndMakeVisible(_engTemp);

  setSize(getParentWidth(), getParentHeight());
}

MainPage::~MainPage()
{
}

void MainPage::resized()
{
  _windGraph.setBounds(0, 5, 230, 150);
  _speedGraph.setBounds(250, 5, 230, 150);
  _engTemp.setBounds(510, 5, 230, 150);

  _wind.setBounds(0, 115, 230, 280);
  _speed.setBounds(250, 115, 230, 280);

  _map.setBounds(0, getHeight() - 150, 300, 150);

  displayPageSwitcher();
}