#pragma once
#include <JuceHeader.h>
#include <deque>
#include "Main/DoubleDataSource.h"
#include <stdint.h>

class TimeGraph : public juce::AnimatedAppComponent
{
public:
  TimeGraph(DoubleDataSource* source, bool dropBounds = true, uint64_t duration = UINT64_MAX);
  TimeGraph(DoubleDataSource* source, float min, float max, uint64_t duration = UINT64_MAX);
  ~TimeGraph() override;

  void paint(juce::Graphics& g) override;
	void resized() override { }
	void update() override { }

private:
  DoubleDataSource* source;

  uint64_t duration;

  bool dropBounds;

  bool fixedBounds;
  float fixedMin;
  float fixedMax;

  void drawText(juce::Graphics& g);
};