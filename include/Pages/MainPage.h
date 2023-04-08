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
  Speedometer _wind;
  Speedometer _speed;
  MapComponent _map;
  LapCounter _lapCounter;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPage)
};
