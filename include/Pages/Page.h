#pragma once

#include <JuceHeader.h>
#include "Main/Main.h"
#include "Main/PageSwitchComponent.h"
#include <string>

class Page : public juce::AnimatedAppComponent
{
public:
  Page(GroundCrewDisplay::MainWindow *window, ActivePage page);
  ~Page();

  virtual void update() override {}
  virtual void paint(juce::Graphics &) override {}
  virtual void resized() override {}

  void displayPageSwitcher(juce::Graphics &);

  PageSwitchComponent _pageSwitcher; 
private:
  GroundCrewDisplay::MainWindow *mainWindow;
};