#pragma once

#include <mutex>
#include <JuceHeader.h>
#include <stdint.h>

class InformationWidget : public juce::AnimatedAppComponent
{
public:
  InformationWidget();

  void paint(juce::Graphics &g) override;
  void resized() override {}
  void update() override {}

  static void checkRaceStart();

private:
  inline static std::mutex raceStartMtx;
  inline static uint64_t raceStart = UINT64_MAX;
};