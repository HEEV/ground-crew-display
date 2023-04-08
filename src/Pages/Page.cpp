#include "Pages/Page.h"

#define RIGHT_MARGIN_SIZE 250
#define INFORMATION_HEIGHT 60

Page::Page(GroundCrewDisplay::MainWindow *window, ActivePage page) : mainWindow(window), pageSwitcher(window, page), isMainPage(page == ActivePage::MainPage)
{
  addAndMakeVisible(pageSwitcher);
  addAndMakeVisible(information);

  pageSwitcher.setAlwaysOnTop(true);
  information.setAlwaysOnTop(true);

  setSize(getParentWidth(), getParentHeight());
  setFramesPerSecond(FRAMERATE);
}

Page::~Page() {}

void Page::update()
{
  if (isMainPage)
  {
    Sources::commitAllBuffers();
  }
}

void Page::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
}

void Page::displayRightMargin()
{
  pageSwitcher.setBounds(getUsableWidth(), 0, RIGHT_MARGIN_SIZE, getHeight() - INFORMATION_HEIGHT);
  information.setBounds(getUsableWidth(), getHeight() - INFORMATION_HEIGHT, RIGHT_MARGIN_SIZE, INFORMATION_HEIGHT);
}

int Page::getUsableWidth()
{
  return getWidth() - RIGHT_MARGIN_SIZE;
}

int Page::getUsableHeight()
{
  return getHeight();
}