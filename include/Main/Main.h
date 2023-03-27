#pragma once

#include <JuceHeader.h>
#include <CommunicationManager.h>
#include <locale>
#include "Packets.h"
#include <string>
#include "ActivePage.h"
#include "DoubleDataSource.h"

class MainPage;
class SensorPage;

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 600;

void buffTester(DoubleDataSource* source, double start = 20);

/**
 * Main application handler.
 */
class GroundCrewDisplay : public juce::JUCEApplication
{
public:
  GroundCrewDisplay();
  ~GroundCrewDisplay();

  const juce::String getApplicationName() override { return ProjectInfo::projectName; }
  const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
  bool moreThanOneInstanceAllowed() override { return false; }

  void initialise(const juce::String &commandLine) override;
  void shutdown() override;
  void systemRequestedQuit() override;
  void anotherInstanceStarted(const juce::String &commandLine) override;

  class MainWindow : public juce::DocumentWindow
  {
  public:
    MainWindow(juce::String name);
    ~MainWindow();

    void closeButtonPressed() override;

    void setPage(ActivePage page);

  private:
    juce::Component *mainPage;
    juce::Component *sensorPage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
  };

private:
  CommunicationManager cmanager;
  std::unique_ptr<MainWindow> mainWindow;
};