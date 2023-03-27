#include <locale>
#include <thread>
#include <string>
#include <chrono>
#include <cstdlib>

#include "Main/Main.h"
#include "Main/Sources.h"
#include "Packets.h"

#include "Pages/MainPage.h"
#include "Pages/SensorPage.h"

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

  std::thread windThread([]()
                         { buffTester(&Sources::wind); });
  std::thread speedThread([]()
                          { buffTester(&Sources::speed); });
  std::thread engThread([]()
                        { buffTester(&Sources::engTemp, 421); });
  windThread.detach();
  speedThread.detach();
  engThread.detach();

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
  sensorPage = new SensorPage(this);

  setUsingNativeTitleBar(true);

  auto &llf = getLookAndFeel();
  llf.setColour(DocumentWindow::backgroundColourId, getBackgroundColour());
  llf.setColour(ColourIds::textColourId, Colours::black);

  setPage(ActivePage::MainPage);

  setVisible(true);
}

GroundCrewDisplay::MainWindow::~MainWindow()
{
  delete mainPage;
}

void GroundCrewDisplay::MainWindow::setPage(ActivePage page)
{
  juce::Component *newPage;
  switch (page)
  {
  case ActivePage::MainPage:
    newPage = mainPage;
    break;
  case ActivePage::SensorPage:
    newPage = sensorPage;
    break;
  default:
    newPage = mainPage;
    break;
  }

  setContentNonOwned(newPage, true);
  setResizable(true, true);

  // Forces GUI to be fullscreen when not debugging, but remain windowed for development
#ifdef DEBUG
  setResizable(true, true);
  centreWithSize(WIDTH, HEIGHT);
#else
  setResizable(false, false);
  Desktop::getInstance().setKioskModeComponent(this, false);
#endif

  getContentComponent()->setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
}

void GroundCrewDisplay::MainWindow::closeButtonPressed()
{
  JUCEApplication::getInstance()->systemRequestedQuit();
}

void buffTester(DoubleDataSource *source, double start)
{
  static Random &rand = Random::getSystemRandom();
  double randVal = 0;

  while (true)
  {
    uint64_t now = duration_cast<std::chrono::milliseconds>(
                       std::chrono::system_clock::now().time_since_epoch())
                       .count();

    source->bufferData(now, start + randVal);

    randVal += rand.nextDouble() * -(rand.nextBool() * 2 - 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(rand.nextInt(100) + 2));
  }
}

// Launch the app
START_JUCE_APPLICATION(GroundCrewDisplay)