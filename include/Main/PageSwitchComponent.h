#pragma once

#include <JuceHeader.h>
#include "Main/Main.h"

class PageSwitchComponent : public juce::Component
{
public:
  PageSwitchComponent(GroundCrewDisplay::MainWindow *window);
  ~PageSwitchComponent() override;

  void paint(juce::Graphics &) override;
  
private:
  GroundCrewDisplay::MainWindow *mainWindow;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PageSwitchComponent)
};