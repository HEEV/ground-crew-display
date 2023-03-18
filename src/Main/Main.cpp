#include <locale>
#include "Main/Main.h"
#include "Pages/MainPage.h"
#include "Packets.h"
#include <string>

GroundCrewDisplay::GroundCrewDisplay()
{
}

GroundCrewDisplay::~GroundCrewDisplay()
{
  // delete cmanager;
}

void GroundCrewDisplay::initialise(const juce::String &commandLine)
{
  // cmanager = new CommunicationManager(DDS_SERVER_IP, false);
  mainWindow.reset(new MainWindow(getApplicationName()));
}

void GroundCrewDisplay::shutdown()
{
  mainWindow = nullptr;
}

void GroundCrewDisplay::systemRequestedQuit()
{
  quit();
}

void GroundCrewDisplay::anotherInstanceStarted(const juce::String &commandLine)
{
  /* Do nothing */
}

GroundCrewDisplay::MainWindow::MainWindow(juce::String name)
    : DocumentWindow(name,
                     Colours::grey,
                     DocumentWindow::closeButton)
{
  mainPage = new MainPage(this);

  setUsingNativeTitleBar(true);

  setContentNonOwned(mainPage, true);

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

GroundCrewDisplay::MainWindow::~MainWindow()
{
  delete mainPage;
}

void GroundCrewDisplay::MainWindow::closeButtonPressed()
{
  JUCEApplication::getInstance()->systemRequestedQuit();
}

// Launch the app
START_JUCE_APPLICATION(GroundCrewDisplay)