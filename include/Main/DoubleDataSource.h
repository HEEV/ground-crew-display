#pragma once

#include "Main/DataSource.h"
#include <float.h>
#include <string>

class DoubleDataSource : public DataSource<double>
{
public:
  DoubleDataSource(uint64_t maxDuration, std::string name = "<unknown name>", std::string units = "<unknown units>");

  bool canInterpolateAt(uint64_t time);
  double interpolatedAt(uint64_t time);
  double slopeAt(uint64_t time);

  double getMin();
  double getMax();

  double getGlobalMin();
  double getGlobalMax();

private:
  double min = DBL_MAX;
  double max = DBL_MIN;

  double globalMin = DBL_MAX;
  double globalMax = DBL_MIN;

  void whenDataAdded(double value);
  void whenDataDropped(double value);
  void whenDataCleared();

  void updateGlobals();
};