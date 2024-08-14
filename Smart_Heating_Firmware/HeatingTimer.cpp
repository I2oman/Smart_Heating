#include "HeatingTimer.h"

HeatingTimer::HeatingTimer(TimePair time) {
  this->id = micros();
  this->timer = time;
}

void HeatingTimer::modify(TimePair time) {
  this->timer = time;
}

unsigned long HeatingTimer::getId() const {
  return id;
}

HeatingTimer::TimePair HeatingTimer::getTimer() const {
  return timer;
}

std::array<uint16_t, 2> HeatingTimer::getValue() const {
  return { timer.startTime.hours * 60 + timer.startTime.minutes, timer.endTime.hours * 60 + timer.endTime.minutes };
}
