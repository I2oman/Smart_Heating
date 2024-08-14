#include "DayTimer.h"

DayTimer::DayTimer()
  : dayPair({ 0, 0 }) {
  this->id = micros();
}

DayTimer::DayTimer(DayPair dayPair) {
  initialize(dayPair);
}

DayTimer::DayTimer(DayPair dayPair, std::initializer_list<HeatingTimer> timers)
  : timers(timers) {
  initialize(dayPair);
}

void DayTimer::addTimer(const HeatingTimer& timer) {
  timers.push_back(timer);
  sortTimers();
}

void DayTimer::modifyDays(DayPair dayPair) {
  this->dayPair = dayPair;
}

void DayTimer::deleteTimer(unsigned long id) {
  auto it = std::remove_if(timers.begin(), timers.end(), [&id](const HeatingTimer& t) {
    return t.getId() == id;
  });

  timers.erase(it, timers.end());

  sortTimers();
}

HeatingTimer& DayTimer::getTimer(unsigned long id) {
  for (size_t i = 0; i < timers.size(); ++i) {
    if (timers[i].getId() == id) {
      return timers[i];
    }
  }
}

unsigned long DayTimer::getId() const {
  return id;
}

DayTimer::DayPair DayTimer::getDays() const {
  return dayPair;
}

HeatingTimer* DayTimer::getTimersArr() const {
  HeatingTimer* timersArray = new HeatingTimer[timers.size()];
  std::copy(timers.begin(), timers.end(), timersArray);
  return timersArray;
}

const std::vector<HeatingTimer>& DayTimer::getTimersList() const {
  return timers;
}

size_t DayTimer::getTimerCount() const {
  return timers.size();
}

void DayTimer::initialize(DayPair dayPair) {
  this->id = micros();

  this->dayPair = dayPair;

  sortTimers();
}

void DayTimer::sortTimers() {
  std::sort(timers.begin(), timers.end(), [](const HeatingTimer& a, const HeatingTimer& b) {
    const auto a_values = a.getValue();
    const auto b_values = b.getValue();

    if (a_values[0] != b_values[0]) {
      return a_values[0] < b_values[0];
    }

    return a_values[1] < b_values[1];
  });
}
