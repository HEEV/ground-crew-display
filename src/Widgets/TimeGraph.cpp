#include "Widgets/TimeGraph.h"
#include <algorithm>

#define H_PADDING 60
#define V_PADDING 30

TimeGraph::TimeGraph(size_t size, bool dropBounds) : _size(size), _dropBounds(dropBounds)
{
  setFramesPerSecond(FRAMERATE);
}

TimeGraph::~TimeGraph()
{
}

void TimeGraph::paint(juce::Graphics &g)
{
  juce::Rectangle bounds = getLocalBounds();
  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
  g.setColour(juce::Colours::black);

  g.drawText(std::to_string(_size / FRAMERATE) + "s", H_PADDING, bounds.getHeight() - V_PADDING, (bounds.getWidth() - H_PADDING) / 2, V_PADDING, juce::Justification::centredLeft);
  g.drawText("now", H_PADDING + (bounds.getWidth() - H_PADDING) / 2, bounds.getHeight() - V_PADDING, (bounds.getWidth() - H_PADDING) / 2, V_PADDING, juce::Justification::centredRight);

  g.drawText(std::to_string(_min), 0, (bounds.getHeight() - V_PADDING) / 2, H_PADDING - 2, (bounds.getHeight() - V_PADDING) / 2, juce::Justification::bottomRight);
  g.drawText(std::to_string(_max), 0, 0, H_PADDING - 2, (bounds.getHeight() - V_PADDING) / 2, juce::Justification::topRight);

  g.setColour(juce::Colours::blue);

  // i is for data counting, j is where the point is on the screen, when _data.size() >= _size, i == j
  juce::Point<float> lastPoint;
  size_t j = _size - _data.size();
  for (size_t i = 0; i < _data.size(); ++i, ++j)
  {
    float x = (((float)j) / _size) * (bounds.getWidth() - H_PADDING) + H_PADDING;
    float y = ((_data[i] - _min) / (_max - _min)) * (bounds.getHeight() - V_PADDING);

    Point<float> newPoint = juce::Point<float>(x, y);
    if (i == 0)
    {
      lastPoint = newPoint;
    }
    else
    {
      juce::Line<float> line = juce::Line<float>(lastPoint, newPoint);
      g.drawLine(line, 2);
      lastPoint = newPoint;
    }
  }

    g.setColour(juce::Colours::black);
  juce::Line<float> horizLine(juce::Point<float>(H_PADDING, 0),
                              juce::Point<float>(H_PADDING, bounds.getHeight() - V_PADDING));
  juce::Line<float> vertLine(juce::Point<float>(H_PADDING, bounds.getHeight() - V_PADDING),
                             juce::Point<float>(bounds.getWidth(), bounds.getHeight() - V_PADDING));

  g.drawLine(horizLine, 3);
  g.drawLine(vertLine, 3);
}

void TimeGraph::clear()
{
  _data.erase(_data.begin(), _data.end());
}

void TimeGraph::addData(float newPoint)
{
  bool needsBoundRecalc = newPoint < _min || newPoint > _max;

  if (_data.size() >= _size)
  {
    float front = _data.front();
    _data.pop_front();
    needsBoundRecalc = needsBoundRecalc || front < _min || front > _max;
  }

  if (needsBoundRecalc)
  {
    if (_data.size() >= 2)
    {
      std::deque<float>::iterator minIt = std::min_element(_data.begin(), _data.end());
      _min = *minIt;

      std::deque<float>::iterator maxIt = std::max_element(_data.begin(), _data.end());
      _max = *maxIt;
    }
    else
    {
      _min = newPoint;
      _max = newPoint;
    }

    _min = std::min<float>(_min - 1, _min * .95);
    _max = std::max<float>(_max + 1, _max * 1.05);

    if (_min < _totalMin || _data.size() == 0)
    {
      _totalMin = _min;
    }

    if (_max > _totalMax || _data.size() == 0)
    {
      _totalMax = _max;
    }
    
    if (!_dropBounds){
      _min = _totalMin;
      _max = _totalMax;
    }
  }

  _data.push_back(newPoint);
}