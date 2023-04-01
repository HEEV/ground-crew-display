#pragma once

#include "Main/DataSource.h"
#include <float.h>
#include <string>
#include <type_traits>

template <typename T>
concept arithmetic = std::integral<T> or std::floating_point<T>;
template <typename T>
  requires arithmetic<T>
class NumericDataSource : public DataSource<T>
{
public:
  NumericDataSource(uint64_t maxDuration, std::string name = "<unknown name>", std::string units = "<unknown units>") : DataSource<T>(maxDuration, name, units)
  {
    this->onDataAdded(std::function([this](T value)
                              { whenDataAdded(value); }));
    this->onDataDropped(std::function([this](T value)
                                { whenDataDropped(value); }));
    this->onDataCleared(std::function([this]()
                                { whenDataCleared(); }));
  }

  bool canInterpolateAt(uint64_t time)
  {
    return this->hasLeft(time) && this->hasRight(time);
  }

  T interpolatedAt(uint64_t time)
  {
    if (!canInterpolateAt(time))
    {
      throw std::runtime_error("Can not interpolate at " + std::to_string(time));
    }

    T left = this->leftOf(time);
    T right = this->rightOf(time);

    uint64_t startTime = this->timeAtLeftOf(time);
    uint64_t endTime = this->timeAtRightOf(time);

    T progress = ((T)(time - startTime)) / ((T)(endTime - startTime));

    return std::lerp(left, right, progress);
  }

  T slopeAt(uint64_t time)
  {
    if (!canInterpolateAt(time))
    {
      throw std::runtime_error("Can not get slope at " + std::to_string(time) + " since we can not interpolate at that time");
    }

    T left = this->leftOf(time);
    T right = this->rightOf(time);

    uint64_t startTime = this->timeAtLeftOf(time);
    uint64_t endTime = this->timeAtRightOf(time);

    return (right - left) / (endTime - startTime);
  }

  T getMin()
  {
    return min;
  }

  T getMax()
  {
    return max;
  }

  T avg()
  {
    if (this->empty())
    {
      return 0;
    }

    T sum;
    for (T value : this->data)
    {
      sum += value;
    }
    return sum / this->size();
  }

  T getGlobalMin()
  {
    return globalMin;
  }

  T getGlobalMax()
  {
    return globalMax;
  }

private:
  T min = DBL_MAX;
  T max = DBL_MIN;

  T globalMin = DBL_MAX;
  T globalMax = DBL_MIN;

  void whenDataAdded(T value)
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

  void whenDataDropped(T value)
  {
    if (this->size() == 0)
    {
      min = DBL_MAX;
      max = DBL_MIN;
      return;
    }

    bool globalsNeedUpdate = false;

    // If we drop the bounds we have to recalculate
    if (min == value)
    {
      min = *std::min_element(this->data.begin(), this->data.end());
      globalsNeedUpdate = true;
    }

    if (max == value)
    {
      max = *std::max_element(this->data.begin(), this->data.end());
      globalsNeedUpdate = true;
    }

    if (globalsNeedUpdate)
    {
      updateGlobals();
    }
  }

  void whenDataCleared()
  {
    min = DBL_MAX;
    max = DBL_MIN;

    globalMin = min;
    globalMax = max;
  }

  void updateGlobals()
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
};