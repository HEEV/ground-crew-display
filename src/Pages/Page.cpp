#include "Pages/Page.h"
#include "Main/ActivePage.h"

Page::Page(GroundCrewDisplay::MainWindow *window, ActivePage page) : mainWindow(window), _pageSwitcher(window, page) {
}

Page::~Page() {}

void Page::displayPageSwitcher(juce::Graphics &g) {
  
  _pageSwitcher.setBounds(getWidth() - 100, 0, 100, 100);
}