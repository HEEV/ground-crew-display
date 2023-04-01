#pragma once
#include <JuceHeader.h>
#include <deque>
#include "DataSources/NumericDataSource.h"
#include <stdint.h>

class TimeGraph : public juce::AnimatedAppComponent
{
public:
  TimeGraph(NumericDataSource<double>* source, bool dropBounds = true, uint64_t duration = UINT64_MAX);
  TimeGraph(NumericDataSource<double>* source, float min, float max, uint64_t duration = UINT64_MAX);
  ~TimeGraph() override;

  void paint(juce::Graphics& g) override;
	void resized() override { }
	void update() override { }

private:
  NumericDataSource<double>* source;

  uint64_t duration;

  bool dropBounds;

  bool fixedBounds;
  float fixedMin;
  float fixedMax;

  void drawText(juce::Graphics& g);
};