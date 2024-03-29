#pragma once

#include <vector>
#include <queue>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <utility>
#include <mutex>
#include <chrono>

template <typename P>
using valueCallback = std::function<void(P)>;
using noValueCallback = std::function<void()>;

template <typename T>
class DataSource
{
public:
  DataSource(uint64_t maxDuration, std::string name = "<unknown name>", std::string units = "<unknown units>") : maxDuration(maxDuration), name(name), units(units)
  {
  }

  ~DataSource()
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

    return data[std::distance(times.begin(), timeAtLeftIt(time))];
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

  void bufferData(uint64_t time, T newDataPoint)
  {
    std::unique_lock lck(buffMtx);
    buff.emplace(time, newDataPoint);
  }

  void commitBuffer()
  {
    std::unique_lock lck(buffMtx);
    while (!buff.empty())
    {
      auto &front = buff.front();
      addData(front.first, front.second);
      buff.pop();
    }
  }

  void addData(uint64_t time, T newDataPoint)
  {
    if (times.empty() || times.back() > time)
    {

      times.push_back(time);
      data.push_back(newDataPoint);

      if (times.empty())
      {
        notifyDataAdded(newDataPoint);
        return;
      }
    }
    else
    {
      // The position we want to insert the value before
      std::vector<uint64_t>::iterator beforeIt = std::lower_bound(times.begin(), times.end(), time);
      if (beforeIt != times.end() && *beforeIt == time)
      {
        throw std::runtime_error("Already has data at " + std::to_string(time));
      }

      ptrdiff_t dist = std::distance(times.begin(), beforeIt);
      times.insert(beforeIt, time);
      data.insert(data.begin() + dist, newDataPoint);
    }

    notifyDataAdded(newDataPoint);

    while (!times.empty() && times.back() - times.front() > maxDuration)
    {
      T erasedValue = data.front();
      times.erase(times.begin());
      data.erase(data.begin());

      notifyDataDropped(erasedValue);
    }
  }

  void clearData()
  {
    std::unique_lock lck(buffMtx);
    while (!buff.empty())
    {
      buff.pop();
    }
    times.clear();
    data.clear();
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

  std::pair<uint64_t, T> atIndex(size_t i)
  {
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

  T first()
  {
    return data.front();
  }

  T last()
  {
    return data.back();
  }

  std::string getName()
  {
    return name;
  }

  std::string getUnits()
  {
    return units;
  }

  void onDataAdded(valueCallback<T> cb)
  {
    dataAddCallbacks.push_back(cb);
  }

  void onDataDropped(valueCallback<T> cb)
  {
    dataDropCallbacks.push_back(cb);
  }

  void onDataCleared(noValueCallback cb)
  {
    dataClearCallbacks.push_back(cb);
  }

protected:
  virtual void onClear() {}

  uint64_t maxDuration;
  std::vector<uint64_t> times;
  std::vector<T> data;

private:
  std::queue<std::pair<uint64_t, T>> buff;
  std::mutex buffMtx;

  std::string name;
  std::string units;

  std::vector<valueCallback<T>> dataAddCallbacks;
  std::vector<valueCallback<T>> dataDropCallbacks;
  std::vector<noValueCallback> dataClearCallbacks;

  void notifyDataAdded(T data)
  {
    for (auto cb : dataAddCallbacks)
    {
      cb(data);
    }
  }

  void notifyDataDropped(T data)
  {
    for (auto cb : dataDropCallbacks)
    {
      cb(data);
    }
  }

  void notifyDataCleared()
  {
    for (auto cb : dataClearCallbacks)
    {
      cb();
    }
  }

  std::vector<uint64_t>::iterator timeAtLeftIt(uint64_t time)
  {
    return std::lower_bound(times.begin(), times.end(), time) - 1;
  }

  std::vector<uint64_t>::iterator timeAtRightIt(uint64_t time)
  {
    return std::upper_bound(times.begin(), times.end(), time);
  }
};
