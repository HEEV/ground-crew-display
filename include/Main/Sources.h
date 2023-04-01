#pragma once

#include "Main/DoubleDataSource.h"

// The amount of data we want to store in milliseconds
constexpr uint64_t MAX_DURATION = 300000;

namespace Sources
{
  inline DoubleDataSource wind = DoubleDataSource(MAX_DURATION, "Wind", "MPH");
  inline DoubleDataSource speed = DoubleDataSource(MAX_DURATION, "Speed", "MPH");
  inline DoubleDataSource engTemp = DoubleDataSource(MAX_DURATION, "Engine Temperature", "°C");

  inline void commitAllBuffers() {
    wind.commitBuffer();
    speed.commitBuffer();
    engTemp.commitBuffer();
  }
}