#pragma once

#include <JuceHeader.h>
#include "Main/Main.h"
#include "ActivePage.h"
#include <list>
#include <string>

class PageSwitchComponent : public juce::Component
{
public:
  PageSwitchComponent(GroundCrewDisplay::MainWindow *window, ActivePage currentPage);
  ~PageSwitchComponent() override;

  void paint(juce::Graphics &) override;
  
private:
  GroundCrewDisplay::MainWindow *mainWindow;
  ActivePage currentPage;

  std::list<juce::TextButton *> buttons;

  void addPage(ActivePage page, std::string name);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PageSwitchComponent)
};