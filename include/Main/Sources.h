#pragma once

#include "Main/DataSource.h"
#include "Main/NumericDataSource.h"

// The amount of data we want to store in milliseconds (for most sources)
constexpr uint64_t DEFAULT_DURATION = 300000;

namespace Sources
{
  inline NumericDataSource<double> velocity = NumericDataSource<double>(DEFAULT_DURATION, "Velocity", "MPH");
  inline NumericDataSource<double> distanceTraveled = NumericDataSource<double>(1000, "Distance Traveled", "m? ft? idk");
  inline NumericDataSource<double> battery = NumericDataSource<double>(DEFAULT_DURATION, "Battery", "V");
  inline NumericDataSource<double> engTemp = NumericDataSource<double>(DEFAULT_DURATION, "Engine Temperature", "°C");
  inline NumericDataSource<double> wind = NumericDataSource<double>(DEFAULT_DURATION, "Wind", "MPH");
  inline NumericDataSource<double> tilt = NumericDataSource<double>(DEFAULT_DURATION, "Tilt", "°");

  inline NumericDataSource<double> latency = NumericDataSource<double>(5000, "Latency", "ms");

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