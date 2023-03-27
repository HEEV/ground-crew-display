#include "Pages/Page.h"

Page::Page(GroundCrewDisplay::MainWindow *window, ActivePage page) : mainWindow(window), pageSwitcher(window, page), isMainPage(page == ActivePage::MainPage)
{
  addAndMakeVisible(pageSwitcher);

  setSize(getParentWidth(), getParentHeight());
  setFramesPerSecond(FRAMERATE);
}

Page::~Page() {}

void Page::update()
{
  if (isMainPage)
  {
    Sources::wind.commitBuffer();
    Sources::speed.commitBuffer();
    Sources::engTemp.commitBuffer();
  }
}

void Page::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
}

void Page::displayPageSwitcher()
{
  pageSwitcher.setBounds(getWidth() - 100, 0, 100, 100);
}