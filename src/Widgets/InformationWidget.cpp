#include "Widgets/InformationWidget.h"
#include <chrono>
#include <sstream>
#include <string>
#include "DataSources/Sources.h"

InformationWidget::InformationWidget()
{
}

void InformationWidget::paint(juce::Graphics &g)
{
  static std::string versionStr = juce::JUCEApplication::getInstance()->getApplicationName().toStdString() + " " + juce::JUCEApplication::getInstance()->getApplicationVersion().toStdString();

  g.fillAll(getLookAndFeel().findColour(DocumentWindow::backgroundColourId));

  juce::DrawableText versionText;
  g.drawText(versionStr, 0, 0, getWidth(), 10, juce::Justification::centred);

  time_t t = std::time(nullptr);
  struct tm currentTime = *std::localtime(&t);

  std::ostringstream currSS;
  currSS << std::put_time(&currentTime, "%d/%m/%Y %H:%M:%S %Z");
  std::string currentTimeStr = currSS.str();

  g.drawText(currentTimeStr, 0, 14, getWidth(), 10, juce::Justification::centred);

  std::string startTimeStr;
  if (raceStart != UINT64_MAX)
  {

    std::chrono::duration durationSinceEpoch = std::chrono::milliseconds(raceStart);
    std::chrono::time_point<std::chrono::system_clock> tpAfterDuration(durationSinceEpoch);
    t = std::chrono::system_clock::to_time_t(tpAfterDuration);
    struct tm raceStartTime = *std::localtime(&t);

    std::ostringstream startSS;
    startSS << std::put_time(&raceStartTime, "Race started at: %H:%M:%S");
    startTimeStr = startSS.str();
  }
  else
  {
    startTimeStr = "Race not started";
  }

  g.drawText(startTimeStr, 0, 28, getWidth(), 10, juce::Justification::centred);

  g.drawText("Average latency: " + std::to_string(Sources::latency.avg()) + "ms", 0, 42, getWidth(), 10, juce::Justification::centred);
}

void InformationWidget::checkRaceStart()
{
  std::unique_lock lck(InformationWidget::raceStartMtx);
  if (raceStart == UINT64_MAX)
  {
    raceStart = duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();
  }
}