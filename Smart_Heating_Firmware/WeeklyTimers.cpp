#include "WeeklyTimers.h"

WeeklyTimers::WeeklyTimers() {}

WeeklyTimers::WeeklyTimers(std::initializer_list<DayTimer> timers)
  : timers(timers) {
  sortTimers();
}

void WeeklyTimers::addDayTimer(const DayTimer& timer) {
  timers.push_back(timer);

  sortTimers();
}

void WeeklyTimers::deleteDayTimer(unsigned long id) {
  auto it = std::remove_if(timers.begin(), timers.end(), [&id](const DayTimer& t) {
    return t.getId() == id;
  });

  timers.erase(it, timers.end());

  sortTimers();
}

DayTimer& WeeklyTimers::getDayTimer(unsigned long id) {
  for (size_t i = 0; i < 7; ++i) {
    if (timers[i].getId() == id) {
      return timers[i];
    }
  }
}

DayTimer* WeeklyTimers::getDayTimersArr() const {
  DayTimer* timersArray = new DayTimer[timers.size()];
  std::copy(timers.begin(), timers.end(), timersArray);
  return timersArray;
}

const std::vector<DayTimer>& WeeklyTimers::getDayTimersList() const {
  return timers;
}

size_t WeeklyTimers::getDayTimerCount() const {
  return timers.size();
}

void WeeklyTimers::sortTimers() {
  std::sort(timers.begin(), timers.end(), [](const DayTimer& a, const DayTimer& b) {
    const auto a_values = a.getDays();
    const auto b_values = b.getDays();

    if (a_values.startDay != b_values.startDay) {
      return a_values.startDay < b_values.startDay;
    }

    return a_values.endDay < b_values.endDay;
  });
}
