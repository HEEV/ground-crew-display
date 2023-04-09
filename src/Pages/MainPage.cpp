#include "Pages/MainPage.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <Packets.h>
#include "Main/ActivePage.h"
#include "DataSources/Sources.h"

#define SPEEDOMETER_WIDTH 290
#define SPEEDOMETER_HEIGHT 350
#define SPEEDOMETER_OFFSET 180 // The offset from the center for EACH speedometer
#define MIDDLE_WIDGET_HEIGHT 100
#define MIDDLE_WIDGET_OFFSET 15 // The offset from the center for EACH middle widget
#define TILT_METER_WIDTH 140
#define TILT_METER_HEIGHT 140

MainPage::MainPage(GroundCrewDisplay::MainWindow *window) : Page(window, ActivePage::MainPage), wind(&Sources::wind, -40.0f, 40.0f),
                                                            map(&Sources::distanceTraveled, "Tracks/ShellTrack.svg", 1.0f),
                                                            speed(&Sources::velocity, 0.0f, 40.0f), lapCounter(&Sources::velocity, 1, 4), tiltMeter(&Sources::tilt, 15)
{
  addAndMakeVisible(wind);
  addAndMakeVisible(speed);
  addAndMakeVisible(map);
  addAndMakeVisible(lapCounter);
  addAndMakeVisible(tiltMeter);
  addAndMakeVisible(rect);
}

void MainPage::resized()
{
  int centerX = getUsableWidth() / 2;
  int centerY = getUsableHeight() / 2;

  wind.setBounds(centerX - SPEEDOMETER_WIDTH - SPEEDOMETER_OFFSET, centerY - SPEEDOMETER_HEIGHT / 2, SPEEDOMETER_WIDTH, SPEEDOMETER_HEIGHT);
  speed.setBounds(centerX + SPEEDOMETER_OFFSET, centerY - SPEEDOMETER_HEIGHT / 2, SPEEDOMETER_WIDTH, SPEEDOMETER_HEIGHT);
  map.setBounds(centerX - SPEEDOMETER_OFFSET - 5, centerY - MIDDLE_WIDGET_HEIGHT - MIDDLE_WIDGET_OFFSET, (SPEEDOMETER_OFFSET - 5) * 2, MIDDLE_WIDGET_HEIGHT);
  lapCounter.setBounds(centerX - SPEEDOMETER_OFFSET - 5, centerY + MIDDLE_WIDGET_OFFSET, (SPEEDOMETER_OFFSET - 5) * 2, MIDDLE_WIDGET_HEIGHT);
  tiltMeter.setBounds(centerX - TILT_METER_WIDTH / 2, centerY + 10 + MIDDLE_WIDGET_OFFSET + MIDDLE_WIDGET_HEIGHT, TILT_METER_WIDTH, TILT_METER_HEIGHT);

  displayRightMargin();
}