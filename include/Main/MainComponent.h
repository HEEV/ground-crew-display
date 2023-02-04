#pragma once

#include <JuceHeader.h>
#include "Widgets/Speedometer.h"

class MainComponent : public juce::Component
{
public:
  MainComponent();
  ~MainComponent() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  Speedometer _wind;

  // See https://docs.juce.com/master/classMouseListener.html for mouse events
  class MouseEvents : public MouseListener
  {
  public:
    void mouseDown(const MouseEvent &e) override;
    void mouseDoubleClick(const MouseEvent &e) override;
  } _mouse;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
