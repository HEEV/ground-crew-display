#pragma once

#include "Main/DoubleDataSource.h"

// The amount of data we want to store in milliseconds (for most sources)
constexpr uint64_t DEFAULT_DURATION = 300000;

namespace Sources
{
  inline DoubleDataSource velocity = DoubleDataSource(DEFAULT_DURATION, "Velocity", "MPH");
  inline DoubleDataSource distanceTraveled = DoubleDataSource(1000, "Distance Traveled", "m? ft? idk");
  inline DoubleDataSource battery = DoubleDataSource(DEFAULT_DURATION, "Battery", "V");
  inline DoubleDataSource engTemp = DoubleDataSource(DEFAULT_DURATION, "Engine Temperature", "°C");
  inline DoubleDataSource wind = DoubleDataSource(DEFAULT_DURATION, "Wind", "MPH");
  inline DoubleDataSource tilt = DoubleDataSource(DEFAULT_DURATION, "Tilt", "°");

  inline void
  commitAllBuffers()
  {
    velocity.commitBuffer();
    distanceTraveled.commitBuffer();
    battery.commitBuffer();
    engTemp.commitBuffer();
    wind.commitBuffer();
    tilt.commitBuffer();
  }
}