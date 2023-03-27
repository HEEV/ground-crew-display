#include "Main/DoubleDataSource.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>

DoubleDataSource::DoubleDataSource(uint64_t maxDuration, std::string name, std::string units) : DataSource<double>(maxDuration, name, units)
{
}

bool DoubleDataSource::canInterpolateAt(uint64_t time)
{
  return hasLeft(time) && hasRight(time);
}

double DoubleDataSource::interpolatedAt(uint64_t time)
{
  if (!canInterpolateAt(time))
  {
    throw std::runtime_error("Can not interpolate at " + std::to_string(time));
  }

  double left = leftOf(time);
  double right = rightOf(time);

  uint64_t startTime = timeAtLeftOf(time);
  uint64_t endTime = timeAtRightOf(time);

  double progress = ((double) (time - startTime)) / ((double) (endTime - startTime));

  return std::lerp(left, right, progress);
}

double DoubleDataSource::slopeAt(uint64_t time)
{
  if (!canInterpolateAt(time))
  {
    throw std::runtime_error("Can not get slope at " + std::to_string(time) + " since we can not interpolate at that time");
  }

  double left = leftOf(time);
  double right = rightOf(time);

  uint64_t startTime = timeAtLeftOf(time);
  uint64_t endTime = timeAtRightOf(time);

  return (right - left) / (endTime - startTime);
}

double DoubleDataSource::getMin()
{
  return min;
}

double DoubleDataSource::getMax()
{
  return max;
}

double DoubleDataSource::getGlobalMin()
{
  return globalMin;
}

double DoubleDataSource::getGlobalMax()
{
  return globalMax;
}

void DoubleDataSource::onDataAdded(double value)
{
  bool globalsNeedUpdate = false;

  if (value < min)
  {
    min = value;
    globalsNeedUpdate = true;
  }

  if (value > max)
  {
    max = value;
    globalsNeedUpdate = true;
  }

  updateGlobals();
}

void DoubleDataSource::onDataDropped(double value)
{
  if (size() == 0)
  {
    min = DBL_MAX;
    max = DBL_MIN;
    return;
  }

  bool globalsNeedUpdate = false;

  // If we drop the bounds we have to recalculate
  if (min == value)
  {
    min = *std::min_element(data.begin(), data.end());
    globalsNeedUpdate = true;
  }

  if (max == value)
  {
    max = *std::max_element(data.begin(), data.end());
    globalsNeedUpdate = true;
  }

  if (globalsNeedUpdate)
  {
    updateGlobals();
  }
}

void DoubleDataSource::onClear()
{
  min = DBL_MAX;
  max = DBL_MIN;

  globalMin = min;
  globalMax = max;
}

void DoubleDataSource::updateGlobals()
{
  if (min < globalMin)
  {
    globalMin = min;
  }

  if (max > globalMax)
  {
    globalMax = max;
  }
}