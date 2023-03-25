#pragma once

#include <JuceHeader.h>
#include "Widgets/Speedometer.h"
#include "Widgets/TimeGraph.h"
#include "Widgets/MapComponent.h"
#include "Main/Main.h"
#include "Main/PageSwitchComponent.h"

class SensorPage : public juce::AnimatedAppComponent
{
public:
  SensorPage(GroundCrewDisplay::MainWindow *window);
  ~SensorPage() override;

  void update() override;
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  GroundCrewDisplay::MainWindow *mainWindow;

  Speedometer _wind;
  TimeGraph _windGraph;

  PageSwitchComponent _pageSwitcher;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SensorPage)
};
