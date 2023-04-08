#include "Pages/MainPage.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <Packets.h>
#include "Main/ActivePage.h"
#include "DataSources/Sources.h"

MainPage::MainPage(GroundCrewDisplay::MainWindow *window) : Page(window, ActivePage::MainPage), _wind(&Sources::wind, -40.0f, 40.0f), 
                                                            _map(&Sources::distanceTraveled, "Tracks/ShellTrack.svg", 1.0f),
                                                            _speed(&Sources::velocity, 0.0f, 40.0f), _lapCounter(&Sources::velocity, 1, 4)
{
  addAndMakeVisible(_wind);
  addAndMakeVisible(_speed);
  addAndMakeVisible(_map);
  addAndMakeVisible(_lapCounter);
}

void MainPage::resized()
{
  _wind.setBounds(0, 115, 230, 280);
  _speed.setBounds(250, 115, 230, 280);

  _map.setBounds(0, getUsableHeight() - 150, 300, 150);

  _lapCounter.setBounds(400, getUsableHeight() - 150, 300, 150);

  displayRightMargin();
}