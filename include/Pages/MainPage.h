#pragma once

#include <JuceHeader.h>
#include <CommunicationManager.h>
#include "Widgets/Speedometer.h"
#include "Widgets/TimeGraph.h"
#include "Widgets/MapComponent.h"
#include "Main/Main.h"
#include "Main/PageSwitchComponent.h"
#include "Pages/Page.h"
#include "Widgets/LapCounter.h"
#include "Widgets/InformationWidget.h"

class MainPage : public Page
{
public:
  MainPage(GroundCrewDisplay::MainWindow* window);

  void resized() override;

private:
  GroundCrewDisplay::MainWindow *mainWindow;

  Speedometer _wind;
  TimeGraph _windGraph;

  Speedometer _speed;
  TimeGraph _speedGraph;

  MapComponent _map;

  TimeGraph _engTemp;

  LapCounter _lapCounter;

  InformationWidget _info;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPage)
};
