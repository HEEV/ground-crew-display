#pragma once

#include <JuceHeader.h>
#include "Widgets/Speedometer.h"
#include "Widgets/TimeGraph.h"
#include "Widgets/MapComponent.h"
#include "Main/Main.h"
#include "Main/PageSwitchComponent.h"
#include "Pages/Page.h"

class MainPage : public Page
{
public:
  MainPage(GroundCrewDisplay::MainWindow* window);
  ~MainPage() override;

  void resized() override;

private:
  GroundCrewDisplay::MainWindow *mainWindow;

  Speedometer _wind;
  TimeGraph _windGraph;

  Speedometer _speed;
  TimeGraph _speedGraph;

  MapComponent _map;

  TimeGraph _engTemp;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPage)
};
