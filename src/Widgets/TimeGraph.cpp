#include "Widgets/TimeGraph.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <utility>
#include <vector>

#define H_PADDING 40
#define V_PADDING 20

TimeGraph::TimeGraph(DoubleDataSource *source, bool dropBounds, uint64_t duration) : source(source), dropBounds(dropBounds), fixedBounds(false)
{
  setFramesPerSecond(FRAMERATE);

  if (duration == UINT64_MAX || duration == 0)
  {
    this->duration = source->getMaxDuration();
  }
  else
  {
    this->duration = duration;
  }
}

TimeGraph::TimeGraph(DoubleDataSource *source, float min, float max, uint64_t duration) : source(source), dropBounds(false), fixedMin(min), fixedMax(max), fixedBounds(true)
{
  setFramesPerSecond(FRAMERATE);

  if (duration == UINT64_MAX || duration == 0)
  {
    this->duration = source->getMaxDuration();
  }
  else
  {
    this->duration = duration;
  }
}

TimeGraph::~TimeGraph()
{
}

void TimeGraph::paint(juce::Graphics &g)
{
  float min;
  float max;

  if (fixedBounds)
  {
    min = fixedMin;
    max = fixedMax;
  }
  else
  {
    if (dropBounds)
    {
      min = source->getMin();
      max = source->getMax();
    }
    else
    {
      min = source->getGlobalMin();
      max = source->getGlobalMax();
    }

    if (min == max)
    {
      min -= 1;
      max += 1;
    }
  }

  juce::Rectangle bounds = getLocalBounds();
  int usableHeight = bounds.getHeight() - V_PADDING;
  int usableWidth = bounds.getWidth() - H_PADDING;

  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));

  if (source->empty())
  {
    g.setColour(juce::Colours::darkred);
    g.drawText("<no data>", H_PADDING, 0, usableWidth, usableHeight, juce::Justification::centred);
  }
  else
  {
    // Its important to remember that the data source does not guarantee that the data is from the last n seconds, but only that the data stored does not exceed n seconds
    uint64_t now = duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();
    uint64_t endTime = now;
    uint64_t startTime = endTime - duration;

    g.setColour(juce::Colours::blue);

    std::vector<juce::Point<float>> points;

    juce::Point<float> lastPoint;
    for (size_t i = 0; i < source->size(); ++i)
    {
      std::pair<uint64_t, double> pair = source->atIndex(i);
      uint64_t time = pair.first;
      double value = pair.second;

      // If there is a point to the left we should be able to interpolate, unless of course there is not a point to the right, meaning there's only one point, and we can't draw it anyway since it's off screen
      if (time < startTime)
      {
        if (source->size() == 1)
        {
          break;
        }
        else if (source->atIndex(i + 1).first < startTime)
        {
          continue;
        }
        time = startTime;
        value = source->interpolatedAt(startTime);
      }

      float progressX = (time - startTime) / ((float)(endTime - startTime));
      float x = std::lerp((float)H_PADDING, (float)bounds.getWidth(), progressX);

      float progressY = (value - min) / (max - min);
      float y = std::lerp((float)bounds.getHeight() - V_PADDING, 0.0f, progressY);

      juce::Point<float> newPoint(x, y);
      if (i > 0)
      {
        juce::Line<float> line = juce::Line<float>(lastPoint, newPoint);
        g.drawLine(line, 2);
      }
      lastPoint = newPoint;
      points.push_back(newPoint);
    }

    for (auto p : points)
    {
      g.setColour(juce::Colours::green);
      g.fillEllipse(p.getX() - 1, p.getY() - 1, 2, 2);
    }
  }

  // We redraw the backgorund color so that we can trim off the parts of the graph we don't need
  g.setColour(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
  g.fillRect(0, 0, H_PADDING, bounds.getHeight());
  g.fillRect(H_PADDING, usableHeight, usableWidth, V_PADDING);

  g.setColour(juce::Colours::black);

  g.drawText(std::to_string(duration / 1000) + "s", H_PADDING, usableHeight, usableWidth / 2, V_PADDING, juce::Justification::centredLeft);
  g.drawText("now", H_PADDING + usableWidth / 2, usableHeight, usableWidth / 2, V_PADDING, juce::Justification::centredRight);

  g.setColour(juce::Colours::blue);
  g.drawText(source->getName(), H_PADDING, usableHeight, usableWidth, V_PADDING, juce::Justification::centred);
  g.setColour(juce::Colours::black);

  min = round(min * 100) / 100;
  max = round(max * 100) / 100;

  std::stringstream minStream;
  std::stringstream maxStream;
  minStream << std::fixed << std::setprecision(0) << min;
  maxStream << std::fixed << std::setprecision(0) << max;
  g.drawText(minStream.str(), 0, usableHeight / 2, H_PADDING - 2, usableHeight / 2, juce::Justification::bottomRight);
  g.drawText(maxStream.str(), 0, 0, H_PADDING - 2, usableHeight / 2, juce::Justification::topRight);

  g.setColour(juce::Colours::blue);
  g.saveState();
  g.addTransform(AffineTransform::rotation(-MathConstants<float>::halfPi, 0, 0));

  g.drawText(source->getUnits(), -usableHeight, 0, usableHeight, H_PADDING, Justification::centred);
  g.restoreState();
  g.setColour(juce::Colours::black);

  g.setColour(juce::Colours::black);
  juce::Line<float> horizLine(juce::Point<float>(H_PADDING + 1.5, 0),
                              juce::Point<float>(H_PADDING + 1.5, bounds.getHeight() - V_PADDING - 1.5));
  juce::Line<float> vertLine(juce::Point<float>(H_PADDING, bounds.getHeight() - V_PADDING - 1.5),
                             juce::Point<float>(bounds.getWidth(), bounds.getHeight() - V_PADDING));

  g.drawLine(horizLine, 3);
  g.drawLine(vertLine, 3);
}