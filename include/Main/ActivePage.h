#pragma once

#include <string>
#include <ostream>

enum class ActivePage
{
  MainPage,
  SensorPage
};

inline std::ostream &operator<<(std::ostream &str, ActivePage p)
{
  switch(p)
  {
  case ActivePage::MainPage:
    str << "Main";
    break;
  case ActivePage::SensorPage:
    str << "Sensor";
    break;
  default:
    str << "something involving ActivePage is broken lol";
  }

  return str;
}
