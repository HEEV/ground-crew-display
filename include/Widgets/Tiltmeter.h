#pragma once
#include <JuceHeader.h>
#include "DataSources/Sources.h"

class Tiltmeter : public Component
{
public:
	Tiltmeter(NumericDataSource<double> *source, double tiltLimit);
	~Tiltmeter() override;

	void paint(Graphics& g) override;
	void resized() override;

private:
	float _tiltLimit;
  double _curTilt;
  Point<float> _guagePos;
  NumericDataSource<double>* _source;

  // Data used for drawing
	Rectangle<float> _travelLine;
	String _labelText;
	Font _font;
	Rectangle<float> _leftArea;
	Rectangle<float> _rightArea;
	ColourGradient _grad;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tiltmeter)

};