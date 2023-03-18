#pragma once

#include <JuceHeader.h>
#include "Widgets/Speedometer.h"
#include "Widgets/TimeGraph.h"
#include "Widgets/MapComponent.h"
#include "Main/Main.h"
#include "Main/PageSwitchComponent.h"

class MainPage : public juce::AnimatedAppComponent
{
public:
  MainPage(GroundCrewDisplay::MainWindow* window);
  ~MainPage() override;

  void update() override;
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  GroundCrewDisplay::MainWindow *mainWindow;

  Speedometer _wind;
  TimeGraph _windGraph;

  Speedometer _speed;
  TimeGraph _speedGraph;

  MapComponent _map;

  TimeGraph _engTemp;

  PageSwitchComponent _pageSwitcher; 

  // See https://docs.juce.com/master/classMouseListener.html for mouse events
  class MouseEvents : public MouseListener
  {
  public:
    void mouseDown(const MouseEvent &e) override;
    void mouseDoubleClick(const MouseEvent &e) override;
  } _mouse;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPage)
};
