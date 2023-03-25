#pragma once

#include <vector>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <utility>

// this is not threadsafe and its gonna break lol
template <typename T>
class DataSource
{
public:
  DataSource(uint64_t maxDuration) : maxDuration(maxDuration)
  {
  }

  T at(uint64_t time)
  {
    std::vector<uint64_t>::iterator loc = std::find(times.begin(), times.end(), time);
    if (loc == times.end() || times.empty())
    {
      throw std::runtime_error("No data at " + std::to_string(time));
    }
    return data[loc - times.begin()];
  }

  bool hasPointAt(uint64_t time)
  {
    if (times.empty())
    {
      return false;
    }
    return std::binary_search(times.begin(), times.end(), time);
  }

  bool hasLeft(uint64_t time)
  {
    if (times.empty())
    {
      return false;
    }

    return times[0] < time;
  }

  bool hasRight(uint64_t time)
  {
    if (times.empty())
    {
      return false;
    }

    return times.back() > time;
  }

  T leftOf(uint64_t time)
  {
    if (!hasLeft(time))
    {
      throw std::runtime_error("No time to the left of " + std::to_string(time));
    }

    return data[std::distance(timeAtLeftIt(time), times.begin())];
  }

  T rightOf(uint64_t time)
  {
    if (!hasRight(time))
    {
      throw std::runtime_error("No time to the right of " + std::to_string(time));
    }

    return data[std::distance(times.begin(), timeAtRightIt(time))];
  }

  uint64_t timeAtLeftOf(uint64_t time)
  {
    return *timeAtLeftIt(time);
  }

  uint64_t timeAtRightOf(uint64_t time)
  {
    return *timeAtRightIt(time);
  }

  void addData(uint64_t time, T newDataPoint)
  {
    if (times.empty() || times.back() > time)
    {

      times.push_back(time);
      data.push_back(newDataPoint);

      if (times.empty())
      {
        onDataAdded(newDataPoint);
        return;
      }
    }
    else
    {

      // The position we want to insert the value before
      std::vector<uint64_t>::iterator beforeIt = std::lower_bound(times.begin(), times.end(), time);
      if (*beforeIt == time)
      {
        throw std::runtime_error("Already has data at " + std::to_string(time));
      }

      ptrdiff_t dist = std::distance(times.begin(), beforeIt);
      times.insert(beforeIt, time);
      data.insert(data.begin() + dist, newDataPoint);
    }

    onDataAdded(newDataPoint);

    while (!times.empty() && times.back() - times.front() > maxDuration)
    {
      T erasedValue = data.front();
      times.erase(times.begin());
      data.erase(data.begin());

      onDataDropped(erasedValue);
    }
  }

  uint64_t totalDuration()
  {
    return times.back() - times.front();
  }

  uint64_t getMaxDuration()
  {
    return maxDuration;
  }

  size_t size()
  {
    return times.size();
  }

  std::pair<uint64_t, T> atIndex(size_t i) {
    return std::pair<uint64_t, T>(times[i], data[i]);
  }

  bool empty()
  {
    return times.empty();
  }

  uint64_t startTime()
  {
    return times.front();
  }

  uint64_t endTime()
  {
    return times.back();
  }

  T first() {
    return data.front();
  }

  T last() {
    return data.back();
  }

protected:
  virtual void onDataAdded(T value) {}
  virtual void onDataDropped(T value) {}
  virtual void onClear() {}

  uint64_t maxDuration;
  std::vector<uint64_t> times;
  std::vector<T> data;

private:
  std::vector<uint64_t>::iterator timeAtLeftIt(uint64_t time)
  {
    return std::upper_bound(times.begin(), times.end(), time, std::greater<double>());
  }

  std::vector<uint64_t>::iterator timeAtRightIt(uint64_t time)
  {
    return std::upper_bound(times.begin(), times.end(), time);
  }
};
