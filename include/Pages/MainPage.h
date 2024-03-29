#pragma once

#include <JuceHeader.h>
#include <CommunicationManager.h>
#include "Widgets/Speedometer.h"
#include "Widgets/MapComponent.h"
#include "Main/Main.h"
#include "Main/PageSwitchComponent.h"
#include "Pages/Page.h"
#include "Widgets/LapCounter.h"
#include "Widgets/InformationWidget.h"
#include "Widgets/DebugRectangle.h"
#include "Widgets/Tiltmeter.h"

class MainPage : public Page
{
public:
  MainPage(GroundCrewDisplay::MainWindow* window);

  void resized() override;

private:
  Speedometer wind;
  Speedometer speed;
  MapComponent map;
  LapCounter lapCounter;
  Tiltmeter tiltMeter;

  DebugRectangle rect;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPage)
};
