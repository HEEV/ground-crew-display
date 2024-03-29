#pragma once

#include <JuceHeader.h>
#include "Main/Main.h"
#include "Main/PageSwitchComponent.h"
#include <string>
#include "Main/ActivePage.h"
#include "DataSources/Sources.h"
#include "Widgets/InformationWidget.h"

class Page : public juce::AnimatedAppComponent
{
public:
  Page(GroundCrewDisplay::MainWindow *window, ActivePage page);
  ~Page();

  void update() override;
  void paint(juce::Graphics &) override;
  virtual void resized() override {}

protected:
  void displayRightMargin();

  int getUsableWidth();
  int getUsableHeight();

  const GroundCrewDisplay::MainWindow *mainWindow;

private:
  PageSwitchComponent pageSwitcher;
  InformationWidget information;
  
  bool isMainPage;
};