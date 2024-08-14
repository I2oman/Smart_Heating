#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>

class Heating {
public:
  Heating(uint8_t btnPin, uint8_t photoresistorPin, uint8_t ledPin, uint8_t servoPin, uint16_t monitoringFrequency = 1000);
  void begin(bool winterMode = 0, bool userSelect = 0);
  void heatingMonitor();
  void setMode(bool mode);
  bool getMode();
  void userToggleHeating();
  bool getUserSelect();
  void toggleHeating();
  bool statusChanded();
  bool togglingError();
  bool status();

private:
  uint8_t btnPin;
  uint8_t photoresistorPin;
  uint8_t ledPin;
  uint8_t servoPin;

  bool userSelect;
  bool winterMode;

  bool needToToggle;
  bool heatingError;
  bool heatingStatus;
  bool heatingStatusPr;
  bool statusChange;

  Servo myservo;

  uint16_t monitoringFrequency;

  void buttonMonitor();
  void togglingHeating();
};
