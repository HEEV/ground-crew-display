#pragma once

#include "DataSources/DataSource.h"
#include "DataSources/NumericDataSource.h"

// The amount of data we want to store in milliseconds (for most sources)
constexpr uint64_t DEFAULT_DURATION = 300000;

namespace Sources
{
  inline NumericDataSource<double> velocity = NumericDataSource<double>(DEFAULT_DURATION, "Velocity", "MPH");
  inline NumericDataSource<double> distanceTraveled = NumericDataSource<double>(1000, "Distance Traveled", "m? ft? idk");
  inline NumericDataSource<double> battery = NumericDataSource<double>(DEFAULT_DURATION, "Battery", "Volts");
  inline NumericDataSource<double> engTemp = NumericDataSource<double>(DEFAULT_DURATION, "Engine Temperature", "Degrees Celsius");
  inline NumericDataSource<double> wind = NumericDataSource<double>(DEFAULT_DURATION, "Wind", "MPH");
  inline NumericDataSource<double> tilt = NumericDataSource<double>(DEFAULT_DURATION, "Tilt", "Degrees");

  inline NumericDataSource<unsigned int> latency = NumericDataSource<unsigned int>(5000, "Latency", "ms");

  inline void
  commitAllBuffers()
  {
    velocity.commitBuffer();
    distanceTraveled.commitBuffer();
    battery.commitBuffer();
    engTemp.commitBuffer();
    wind.commitBuffer();
    tilt.commitBuffer();

    latency.commitBuffer();
  }
}