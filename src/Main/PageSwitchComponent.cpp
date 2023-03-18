#include "Main/PageSwitchComponent.h"
#include <JuceHeader.h>
#include <stdio.h>
#include <iostream>

PageSwitchComponent::PageSwitchComponent(GroundCrewDisplay::MainWindow *window) : mainWindow(window)
{
}

PageSwitchComponent::~PageSwitchComponent()
{
}

void PageSwitchComponent::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));

  juce::PathStrokeType borderStroke(2,juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded);
  juce::Path borderPath;
  borderPath.addRectangle(0, 0, getWidth(), getHeight());
  g.strokePath(borderPath, borderStroke);
}