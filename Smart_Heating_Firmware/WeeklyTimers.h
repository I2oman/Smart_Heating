#pragma once
#include <Arduino.h>
#include "DayTimer.h"
#include "HeatingTimer.h"

class WeeklyTimers {
public:
  WeeklyTimers();
  template<size_t N>
  WeeklyTimers(DayTimer (&timers)[N]);
  WeeklyTimers(std::initializer_list<DayTimer> timers);

  void addDayTimer(const DayTimer& timer);
  void deleteDayTimer(unsigned long id);

  DayTimer& getDayTimer(unsigned long id);
  DayTimer* getDayTimersArr() const;
  const std::vector<DayTimer>& getDayTimersList() const;
  size_t getDayTimerCount() const;

private:
  std::vector<DayTimer> timers;

  void sortTimers();
};

template<size_t N>
WeeklyTimers::WeeklyTimers(DayTimer (&timers)[N])
  : timers(timers, timers + N) {
  sortTimers();
}
