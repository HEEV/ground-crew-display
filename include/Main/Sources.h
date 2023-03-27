#pragma once

#include "Main/DoubleDataSource.h"

// The amount of data we want to store in milliseconds
constexpr uint64_t MAX_DURATION = 30000;

namespace Sources
{
  inline DoubleDataSource wind = DoubleDataSource(MAX_DURATION, "Wind", "MPH");
  inline DoubleDataSource speed = DoubleDataSource(MAX_DURATION, "Speed", "MPH");
  inline DoubleDataSource engTemp = DoubleDataSource(MAX_DURATION, "Engine Temperature", "°c");
}