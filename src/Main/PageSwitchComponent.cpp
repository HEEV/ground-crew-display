#include "Main/PageSwitchComponent.h"
#include <stdio.h>
#include <iostream>

PageSwitchComponent::PageSwitchComponent(GroundCrewDisplay::MainWindow *window, ActivePage currentPage) : mainWindow(window), currentPage(currentPage)
{
  addPage(ActivePage::MainPage, "Main");
  addPage(ActivePage::SensorPage, "Sensors");
}

PageSwitchComponent::~PageSwitchComponent()
{
  for (juce::TextButton *button : buttons)
  {
    delete button;
  }
}

void PageSwitchComponent::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));

  g.drawLine(0, 0, 0, getHeight(), 2);

  std::list<juce::TextButton *>::iterator it = buttons.begin();
  int y = 10;
  while (it != buttons.end())
  {
    (*it)->setBounds(8, y, getWidth() - 16, 32);
    y += 42;
    ++it;
  }
}

void PageSwitchComponent::addPage(ActivePage page, std::string name)
{
  juce::TextButton *btn = new juce::TextButton(name);

  if (currentPage == page)
  {
    btn->setEnabled(false);
    btn->setColour(TextButton::buttonOnColourId, Colours::darkcyan);
  }
  else
  {
    btn->setColour(TextButton::buttonOnColourId, Colours::cyan);
  }

  btn->onClick = [this, page]()
  { mainWindow->setPage(page); };

  addAndMakeVisible(*btn);
  buttons.push_back(btn);
}