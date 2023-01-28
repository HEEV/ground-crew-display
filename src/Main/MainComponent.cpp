#include "Main/MainComponent.h"
#include <string>
#include <chrono>
#include <iostream>

//==============================================================================
MainComponent::MainComponent() {

    addMouseListener(&_mouse, true);
    
    setSize(getParentWidth(), getParentHeight());
    setFramesPerSecond(30);
}

void MainComponent::update()
{
}

MainComponent::~MainComponent()
{
}

//==============================================================================
//Function exicuts every frame
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
}

void MainComponent::resized()
{
  
}

void MainComponent::MouseEvents::mouseDoubleClick(const MouseEvent& e)
{
    JUCEApplicationBase::quit();
}