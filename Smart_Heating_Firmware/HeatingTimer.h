#pragma once
#include <Arduino.h>

class HeatingTimer {
public:
  struct Time {
    uint8_t hours;
    uint8_t minutes;
  };

  struct TimePair {
    Time startTime;
    Time endTime;
  };

  HeatingTimer(TimePair time = { { 0, 0 }, { 0, 0 } });
  void modify(TimePair time);
  unsigned long getId() const;
  TimePair getTimer() const;
  std::array<uint16_t, 2> getValue() const;

private:
  unsigned long id;
  TimePair timer;
};
