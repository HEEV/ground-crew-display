#include "Pages/SensorPage.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <Packets.h>
#include "Main/ActivePage.h"
#include "DataSources/Sources.h"

SensorPage::SensorPage(GroundCrewDisplay::MainWindow *window) : Page(window, ActivePage::SensorPage), windGraph(&Sources::wind, true, 60 * 1000),
                                                                voltageGraph(&Sources::battery, true, 60 * 1000), engTempGraph(&Sources::engTemp, true, 60 * 1000)
{
  addAndMakeVisible(windGraph);
  addAndMakeVisible(voltageGraph);
  addAndMakeVisible(engTempGraph);
}

void SensorPage::resized()
{
  int thirdOfHeight = getUsableHeight() / 3;
  int width = getUsableWidth() - 5;

  windGraph.setBounds(0, 0, width, thirdOfHeight);
  voltageGraph.setBounds(0, thirdOfHeight, width, thirdOfHeight);
  engTempGraph.setBounds(0, 2 * thirdOfHeight, width, thirdOfHeight);

  displayRightMargin();
}