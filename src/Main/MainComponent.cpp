#include "Main/MainComponent.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>

// Main JUCE component
MainComponent::MainComponent()
{
  setSize(getParentWidth(), getParentHeight());
  setFramesPerSecond(30);

  addMouseListener(&_mouse, true);
}

void MainComponent::update()
{
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
}

void MainComponent::MouseEvents::mouseDoubleClick(const MouseEvent &e)
{
  JUCEApplicationBase::quit();
}