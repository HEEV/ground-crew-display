/*
 * This file contains 
 */

#include <JuceHeader.h>
#include "Main/MainComponent.h"

//==============================================================================
class MainWindowTutorialApplication : public juce::JUCEApplication
{
public:
  //...

  //==============================================================================
  class MainWindow : public juce::DocumentWindow
  {
  public:
    MainWindow(juce::String name) : DocumentWindow(name,
                                                   juce::Colours::lightgrey,
                                                   DocumentWindow::allButtons)
    {
      centreWithSize(300, 200);
      setVisible(true);
    }

    void closeButtonPressed() override
    {
      juce::JUCEApplication::getInstance()->systemRequestedQuit();
    }

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
  };

private:
  std::unique_ptr<MainWindow> mainWindow;
};