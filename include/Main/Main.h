#pragma once

#include <JuceHeader.h>
#include "CommunicationManager.h"
#include <locale>
#include "Packets.h"
#include <string>

class MainPage;

#define DDS_SERVER_IP "163.11.237.241:5000"

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 600;

enum class ActivePage
{
  MainPage
};

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

  private:
    juce::Component *mainPage;
    ActivePage _currentComponent = ActivePage::MainPage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
  };

private:
  CommunicationManager *cmanager;
  std::unique_ptr<MainWindow> mainWindow;
};