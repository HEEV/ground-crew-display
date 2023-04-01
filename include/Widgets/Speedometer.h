#pragma once

#include <JuceHeader.h>
#include <atomic>
#include <thread>
#include <string_view>

#include "LapCounter.h"
#include <float.h>
#include "Main/NumericDataSource.h"

/**
 * A speedometer JUCE widget, with which to display car data in a
 * speedometer-type format.
 *
 * @implements AnimatedAppComponent
 */
class Speedometer : public juce::AnimatedAppComponent
{
public:
  Speedometer(NumericDataSource<double> *source, double minData, double maxData, int subdivisions = 8, int lineWidth = 5);
  Speedometer(NumericDataSource<double> *source, int subdivisions = 8, int lineWidth = 5);
  ~Speedometer();

  // # JUCE METHODS

  void paint(juce::Graphics &g) override;
  void resized() override
  { /*do nothing*/
  }
  void update() override;

  void addLapCounter(LapCounter *lc);

  // # SPEEDOMETER INTERFACING

  /**
   * @brief Sets the minimum and maximum value which the speedometer
   * can display.
   *
   * @param min The minimum display value.
   * @param max The maximum display value.
   */
  void setDataRange(float min, float max);

  void updateData(); // throws std::out_of_range

  /**
   * @brief The data currently displaying on the speedometer.
   *
   *
   * @return data now displaying on the speedometer (set using
   * Speedometer::setData(float)).
   */
  float getData() const;

  /**
   * @brief Sets the name of the speedometer.
   *
   * @param name The new name the speedometer.
   */
  void setName(std::string name);

private:
  NumericDataSource<double> *source;

  // # DISPLAY DATA
  int _lineWidth;
  int _subdivisions;

  LapCounter *_lc;

  /*
   * The rotation value of the speedometer, in radians.
   */
  std::atomic<float> _rotation;

  // # SPEEDOMETER DATA

  /*
   * The minimum of the data range of the speedometer.
   */
  double _dataMin;

  /*
   * The maximum of the data range of the speedometer.
   */
  double _dataMax;

  /*
   * The data displaying on the speedometer.
   */
  std::string _name;

  /*
   * The data displaying on the speedometer.
   */
  double _data;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Speedometer)
};