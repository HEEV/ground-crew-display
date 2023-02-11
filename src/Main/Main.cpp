#include <JuceHeader.h>
#include <locale>
#include "Main/MainComponent.h"
#include "CommunicationManager.h"
#include "Packets.h"

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 600;

/**
 * Main application handler.
 */
class GroundCrewDisplay : public juce::JUCEApplication
{
public:

  GroundCrewDisplay() {}
  const juce::String getApplicationName() override { return ProjectInfo::projectName; }
  const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
  bool moreThanOneInstanceAllowed() override { return false; }

  // This method is where you should put your application's initialization code
  void initialise(const juce::String &commandLine) override
  {
    // cmanager = CommunicationManager("", false);

    mainWindow.reset(new MainWindow(getApplicationName()));
  }

  void shutdown() override
  {
    mainWindow = nullptr;
  }

  void systemRequestedQuit() override
  {
    quit();
  }

  void anotherInstanceStarted(const juce::String &commandLine) override
  { /*Do nothing*/
  }

  class MainWindow : public juce::DocumentWindow
  {
  public:
    MainWindow(juce::String name)
        : DocumentWindow(name,
                         Colours::grey,
                         DocumentWindow::closeButton)
    {
      setUsingNativeTitleBar(false);
      setContentOwned(new MainComponent(), true);

      auto &llf = getLookAndFeel();
      llf.setColour(DocumentWindow::backgroundColourId, getBackgroundColour());
      llf.setColour(ColourIds::textColourId, Colours::black);

      // Forces GUI to be fullscreen when not debugging, but remain windowed for development
#ifdef DEBUG
      setResizable(true, true);
      centreWithSize(WIDTH, HEIGHT);
#else
      setResizable(false, false);
      Desktop::getInstance().setKioskModeComponent(this, false);
#endif

      getContentComponent()->setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);

      Desktop::getInstance().setScreenSaverEnabled(false);

      setVisible(true);
    }

    void closeButtonPressed() override
    {
      // This is called when the user tries to close this window. Here, we'll just
      // ask the app to quit when this happens, but you can change this to do
      // whatever you need.
      JUCEApplication::getInstance()->systemRequestedQuit();
    }

    /* Note: Be careful if you override any DocumentWindow methods - the base
       class uses a lot of them, so by overriding you might break its functionality.
       It's best to do all your work in your content component instead, but if
       you really have to override any DocumentWindow methods, make sure your
       subclass also calls the superclass's method.
    */

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
  };

private:
  CommunicationManager* cmanager;
  std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(GroundCrewDisplay)