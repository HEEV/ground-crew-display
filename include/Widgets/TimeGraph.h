#pragma once
#include <JuceHeader.h>
#include <deque>

class TimeGraph : public juce::AnimatedAppComponent
{
public:
  TimeGraph(size_t size, bool dropBounds = true);
  ~TimeGraph() override;

	void paint(juce::Graphics& g) override;
	void resized() override { }
	void update() override { }

  void clear();
  void addData(float newPoint);

private:
  size_t _size;
  std::deque<float> _data = std::deque<float>();

  bool _dropBounds = false;

  float _totalMin = 0;
  float _totalMax = 1;

  float _min = 0;
  float _max = 1;
};