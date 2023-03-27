#include <stdexcept>
#include <fmt/format.h>
#include <string>

#include "Widgets/Speedometer.h"
#include "Widgets/Constants.h"

Speedometer::Speedometer(DoubleDataSource *source, int subdivisions, int lineWidth) : source(source), _subdivisions(subdivisions), _lineWidth(lineWidth), _lc(nullptr)
{
  _dataMin = source->getMin();
  _dataMax = source->getMax();

  _name = source->getName() + " (" + source->getUnits() + ") ";
} 

Speedometer::Speedometer(DoubleDataSource *source, double minData, double maxData, int subdivisions, int lineWidth) : source(source), _dataMin(minData), _dataMax(maxData), _subdivisions(subdivisions), _lineWidth(lineWidth), _lc(nullptr)
{
  setFramesPerSecond(FRAMERATE);

  _name = source->getName() + " (" + source->getUnits() + ") ";
}

Speedometer::~Speedometer()
{
}

void Speedometer::paint(juce::Graphics &g)
{
  auto bounds = getLocalBounds();
  // explicit background color for animated components
  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));

  // Debug rect
  // g.setColour(Colours::red);
  // g.drawRect(bounds);

  Font f("Consolas", FONT_HEIGHT, juce::Font::bold);
  g.setFont(f);
  g.setColour(Colours::black);
  PathStrokeType stroke(LINE_WEIGHT);

  // Find largest area with the correct proportians widget's draw space
  if (7 * bounds.getWidth() / 8 < bounds.getHeight())
  {
    // Space is taller than is usable due to width constraints
    int heightToRemove = (bounds.getHeight() - 7 * bounds.getWidth() / 8) / 2;
    // center widget verticly
    bounds.removeFromTop(heightToRemove);
    auto label = bounds.removeFromBottom(heightToRemove);

    // Use extra space to add label
    if (label.getHeight() >= FONT_HEIGHT)
      g.drawText(_name, label, Justification::centredBottom);
  }
  else if (7 * bounds.getWidth() / 8 > bounds.getHeight())
  {
    // Space is wider than is usable due to height constraints
    int widthToRemove = (7 * bounds.getWidth() / 8 - bounds.getHeight()) / 2;
    // center widget horizontally
    bounds.removeFromLeft(widthToRemove);
    bounds.removeFromRight(widthToRemove);
  }

  // Draw semicircle
  Path arc;
  // Since JUCE widgets start from the top-left, all custom drawing should start there for consistency
  Point start = bounds.getTopLeft().toFloat();
  start.addXY(LINE_WEIGHT / 2.0f, LINE_WEIGHT / 2.0f);
  float diameter = bounds.getWidth() - LINE_WEIGHT;

  g.drawEllipse(start.x, start.y, diameter, diameter, LINE_WEIGHT / 2);
  // arc.addArc(start.x, start.y, diameter, diameter, -3 * PI / 4, 3 * PI / 4, true);
  // arc.addArc(start.x, start.y, diameter, diameter, -PI, PI, true);
  g.strokePath(arc, stroke);

  // Draw labels
  String largestLabel = fmt::format("{:.1f}", _dataMax);
  float maxSize = std::max(FONT_HEIGHT, f.getStringWidthFloat(largestLabel) / 2.0f);
  Point labelStart = start;
  labelStart.addXY(maxSize, maxSize);
  float labelDiameter = diameter - 2.0 * maxSize;
  Point labelCenter = labelStart + Point(labelDiameter / 2.0f, labelDiameter / 2.0f);
  float labelMultiple = (_dataMax - _dataMin) / _subdivisions;
  for (int i = 0; i < _subdivisions + 1; i++)
  {
    Point<float> p(
        labelDiameter / 2.0f * cos(i * (3 * PI / 2) / _subdivisions + 3 * PI / 4) + labelCenter.x,
        labelDiameter / 2.0f * sin(i * (3 * PI / 2) / _subdivisions + 3 * PI / 4) + labelCenter.y + 5);

    String label = fmt::format("{:.0f}", i * labelMultiple + _dataMin);
    float width = f.getStringWidthFloat(label);
    Rectangle<float> textArea;
    textArea.setY(p.y - FONT_HEIGHT);
    textArea.setLeft(p.x - width / 2.0f);
    textArea.setSize(width, FONT_HEIGHT);

    g.drawText(label, textArea, Justification::centred);
  }

  // Draw digital readout
  Rectangle<float> readoutArea;
  std::string readout;
  if (source->empty())
  {
    readout = "0.0";
  }
  else
  {
    readout = fmt::format("{:.1f}", source->last());
  }
  readout += " " + source->getUnits();

  float readoutWidth = f.getStringWidthFloat(readout);
  readoutArea.setSize(readoutWidth, FONT_HEIGHT);
  readoutArea.setCentre(labelCenter.x, labelCenter.y + FONT_HEIGHT + 5);
  g.drawText(readout, readoutArea, Justification::centredBottom);

  // Draw hand
  constexpr float baseWidth = 20.0f;
  float length = 9.0f * diameter / 16.0f;
  Path hand;
  Point top(labelCenter.x, labelCenter.y - length);
  Point bottomLeft = labelCenter - Point(baseWidth / 2.0f, -baseWidth / 2.0f);
  Point bottomRight = labelCenter + Point(baseWidth / 2.0f, baseWidth / 2.0f);
  hand.startNewSubPath(labelCenter);
  hand.lineTo(bottomLeft);
  hand.lineTo(top);
  hand.lineTo(bottomRight);
  hand.closeSubPath();
  hand.applyTransform(AffineTransform::rotation(_rotation, labelCenter.x, labelCenter.y));
  g.fillPath(hand);
}

void Speedometer::update()
{
  updateData();
  if (_lc)
    _lc->incDistanceTraveled((1.0f / 30.0f) / 3600.0f * _data);
}

void Speedometer::addLapCounter(LapCounter *lc)
{
  _lc = lc;
}

void Speedometer::setDataRange(float min, float max)
{
  _dataMin = min;
  _dataMax = max;

  // Recalculate the new rotation for the data point
  updateData();
}

void Speedometer::updateData()
{
  double value = std::min(_dataMax, std::max(_dataMin, source->empty() ? 0 : source->last()));

  _data = value;

  // Range the data around 0
  double weight = _dataMax - _dataMin;
  value = value - _dataMin;

  // Set the rotation
  _rotation = value / weight * (3 * PI / 2) - 3 * PI / 4;
}

float Speedometer::getData() const
{
  return _data;
}

void Speedometer::setName(std::string name)
{
  _name = name;
}