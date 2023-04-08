#pragma once

#include <JuceHeader.h>
#include "Widgets/TimeGraph.h"
#include "Main/Main.h"
#include "Pages/Page.h"

class SensorPage : public Page
{
public:
  SensorPage(GroundCrewDisplay::MainWindow* window);

  void resized() override;

private:

  TimeGraph windGraph;
  TimeGraph voltageGraph;
  TimeGraph engTempGraph;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SensorPage)
};