#pragma once

#include "Main/DataSource.h"
#include <float.h>

class DoubleDataSource : public DataSource<double>
{
public:
  DoubleDataSource(uint64_t maxDuration);

  bool canInterpolateAt(uint64_t time);
  double interpolatedAt(uint64_t time);

  double getMin();
  double getMax();

  double getGlobalMin();
  double getGlobalMax();

protected:
  void onDataAdded(double value) override;
  void onDataDropped(double value) override;
  void onClear() override;

private:
  double min = DBL_MAX;
  double max = DBL_MIN;

  double globalMin = DBL_MAX;
  double globalMax = DBL_MIN;

  void updateGlobals();
};