#include "Main/MainComponent.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>

// Main JUCE component
MainComponent::MainComponent() : _wind("Wind MPH", 0.0f, 40.0f, Colour(253, 185, 19)), _windGraph(FRAMERATE * 5, 0.0f, 40.0f),
                                 _map("Tracks/ShellTrack.svg", 1.0f), _engTemp(FRAMERATE * 20), 
                                 _speed("Vehicle MPH", 0.0f, 40.0f, Colour(253, 185, 19), 6), _speedGraph(FRAMERATE * 5, 0.0f, 40.0f)
{
  addAndMakeVisible(_wind);
  addAndMakeVisible(_windGraph);

  addAndMakeVisible(_speed);
  addAndMakeVisible(_speedGraph);

  addAndMakeVisible(_map);
  addAndMakeVisible(_engTemp);

  _wind.setData(20.0f);
  _windGraph.addData(20.0f);

  _speedGraph.addData(20.0f);
  _speed.setData(20.0f);

  setSize(getParentWidth(), getParentHeight());
  setFramesPerSecond(FRAMERATE);

  addMouseListener(&_mouse, true);
}

void MainComponent::update()
{
  static float randWind = 0.0f;
  static float randSpeed = 0.0f;

  _wind.setData(20.0f + randWind);
  _windGraph.addData(20.0f + randWind);

  _speed.setData(20.0f + randSpeed);
  _speedGraph.addData(20.0f + randSpeed);

  Random &rand = Random::getSystemRandom();
  randWind += rand.nextFloat() * -(rand.nextBool() * 2 - 1);
  randSpeed += rand.nextFloat() * -(rand.nextBool() * 2 - 1);
}

MainComponent::~MainComponent()
{
}

// Function executes every frame
void MainComponent::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
}

void MainComponent::resized()
{
  _windGraph.setBounds(0, 5, 230, 150);
  _speedGraph.setBounds(250, 5, 230, 150);
  _engTemp.setBounds(510, 5, 230, 150);

  _wind.setBounds(0, 115, 230, 280);
  _speed.setBounds(250, 115, 230, 280);

  _map.setBounds(0, getHeight() - 150, 300, 150);
}

void MainComponent::MouseEvents::mouseDown(const MouseEvent &e)
{
}

void MainComponent::MouseEvents::mouseDoubleClick(const MouseEvent &e)
{
  JUCEApplicationBase::quit();
}