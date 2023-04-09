#pragma once

#include <JuceHeader.h>

class DebugRectangle : public juce::Component
{
public:
  DebugRectangle()
  {
  }

  void paint(juce::Graphics &g) override
  {
    g.fillAll(juce::Colours::hotpink);
  }
  void resized() override
  {
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DebugRectangle)
};