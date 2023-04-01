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

GroundCrewDisplay::GroundCrewDisplay() : cmanager("163.11.237.241:5001")
{
}

GroundCrewDisplay::~GroundCrewDisplay()
{
}

void GroundCrewDisplay::initialise(const juce::String &commandLine)
{
  // Register all packet types and channels we are interested in
  REGISTER_TYPE_TO_MANAGER(WheelData, "vel", cmanager);
  REGISTER_TYPE_TO_MANAGER(BatteryVoltage, "bat", cmanager);
  REGISTER_TYPE_TO_MANAGER(EngineTemp, "enTemp", cmanager);
  REGISTER_TYPE_TO_MANAGER(GPSPosition, "gps", cmanager);
  REGISTER_TYPE_TO_MANAGER(WindSpeed, "wind", cmanager);
  REGISTER_TYPE_TO_MANAGER(CarTilt, "tilt", cmanager);

  // Add callbacks for when data is recieved on a specified channel.
  // These callbacks run on a seperate thread, so be careful with data races
  cmanager.addDataReader("vel", std::function([this](WheelData *data)
                                              {
                                               Sources::distanceTraveled.bufferData(data->head().timeSent(), data->distTravelled());
                                               Sources::velocity.bufferData(data->head().timeSent(), data->velocity()); }));
  cmanager.addDataReader("bat", std::function([this](BatteryVoltage *data)
                                              { Sources::battery.bufferData(data->head().timeSent(), data->volt()); }));
  cmanager.addDataReader("enTemp", std::function([this](EngineTemp *data)
                                                 { Sources::engTemp.bufferData(data->head().timeSent(), data->temp()); }));
  cmanager.addDataReader("wind", std::function([this](WindSpeed *data)
                                               { Sources::wind.bufferData(data->head().timeSent(), data->headSpeed()); }));
  cmanager.addDataReader("tilt", std::function([this](CarTilt *data)
                                               { Sources::tilt.bufferData(data->head().timeSent(), data->angle()); }));

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