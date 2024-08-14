#pragma once
#include <Arduino.h>
#include "HeatingTimer.h"

class DayTimer {
public:
  struct DayPair {
    uint8_t startDay;
    uint8_t endDay;
  };

  DayTimer();
  DayTimer(DayPair dayPair);
  template<size_t N>
  DayTimer(DayPair dayPair, HeatingTimer (&timers)[N]);
  DayTimer(DayPair dayPair, std::initializer_list<HeatingTimer> timers);

  void addTimer(const HeatingTimer& timer);
  void modifyDays(DayPair dayPair);
  void deleteTimer(unsigned long id);

  HeatingTimer& getTimer(unsigned long id);
  unsigned long getId() const;
  DayPair getDays() const;
  HeatingTimer* getTimersArr() const;
  const std::vector<HeatingTimer>& getTimersList() const;
  size_t getTimerCount() const;

private:
  unsigned long id;
  DayPair dayPair;
  std::vector<HeatingTimer> timers;

  void initialize(DayPair dayPair);
  void sortTimers();
};

template<size_t N>
DayTimer::DayTimer(DayPair dayPair, HeatingTimer (&timers)[N])
  : timers(timers, timers + N) {
  initialize(dayPair);
  sortTimers();
}
