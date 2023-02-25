#pragma once
#include <JuceHeader.h>
#include <deque>

class TimeGraph : public juce::AnimatedAppComponent
{
public:
  TimeGraph(size_t size, bool dropBounds = true);
  TimeGraph(size_t size, float min, float max);
  ~TimeGraph() override;

  void paint(juce::Graphics& g) override;
	void resized() override { }
	void update() override { }

  void clear();
  void addData(float newPoint);

private:
  size_t _size;
  std::deque<float> _data = std::deque<float>();

  bool _dropBounds;
  bool _fixedBounds;

  float _fixedMin = -1;
  float _fixedMax = -1;

  float _totalMin = 0;
  float _totalMax = 1;

  float _min = 0;
  float _max = 1;
};