#include "Pages/MainPage.h"
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <Packets.h>
#include "Main/ActivePage.h"
#include "Main/Sources.h"

// Main JUCE component
MainPage::MainPage(GroundCrewDisplay::MainWindow *window) : mainWindow(window), _wind("Wind MPH", 0.0f, 40.0f, Colour(253, 185, 19)), _windGraph(&Sources::wind, 0.0f, 40.0f),
                                                            _map("Tracks/ShellTrack.svg", 1.0f), _engTemp(&Sources::engTemp, true),
                                                            _speed("Vehicle MPH", 0.0f, 40.0f, Colour(253, 185, 19), 6), _speedGraph(&Sources::speed, false, 20000), _pageSwitcher(window, ActivePage::MainPage),
                                                            _man("163.11.237.241:5001")
{
  addAndMakeVisible(_wind);
  addAndMakeVisible(_windGraph);

  addAndMakeVisible(_speed);
  addAndMakeVisible(_speedGraph);

  addAndMakeVisible(_map);
  addAndMakeVisible(_engTemp);

  addAndMakeVisible(_pageSwitcher);

  // Register all packet types and channels we are interested in
  REGISTER_TYPE_TO_MANAGER(WheelData, "vel", _man);
  REGISTER_TYPE_TO_MANAGER(BatteryVoltage, "bat", _man);
  REGISTER_TYPE_TO_MANAGER(EngineTemp, "enTemp", _man);
  REGISTER_TYPE_TO_MANAGER(GPSPosition, "gps", _man);
  REGISTER_TYPE_TO_MANAGER(WindSpeed, "wind", _man);
  REGISTER_TYPE_TO_MANAGER(CarTilt, "tilt", _man);

  // Add callbacks for when data is recieved on a specified channel.
  // These callbacks run on a seperate thread, so be careful with data races
  _man.addDataReader("vel", std::function([this](WheelData* data){
    Sources::speed.addData(data->head().timeOcc(), data->velocity());
    _speed.setData(data->velocity());
  }));
  _man.addDataReader("enTemp", std::function([this](EngineTemp* data){
    Sources::engTemp.addData(data->head().timeOcc(), data->temp());
  }));
  _man.addDataReader("wind", std::function([this](WindSpeed* data){
    Sources::wind.addData(-data->head().timeOcc(), data->headSpeed());
    _wind.setData(data->headSpeed());
  }));

  setSize(getParentWidth(), getParentHeight());
}

MainPage::~MainPage()
{
}

// Function executes every frame
void MainPage::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));
}

void MainPage::resized()
{
  _windGraph.setBounds(0, 5, 230, 150);
  _speedGraph.setBounds(250, 5, 230, 150);
  _engTemp.setBounds(510, 5, 230, 150);

  _wind.setBounds(0, 115, 230, 280);
  _speed.setBounds(250, 115, 230, 280);

  _map.setBounds(0, getHeight() - 150, 300, 150);

  _pageSwitcher.setBounds(getWidth() - 100, 0, 100, 100);
}