#include "Heating.h"

Heating::Heating(uint8_t btnPin, uint8_t photoresistorPin, uint8_t ledPin, uint8_t servoPin, uint16_t monitoringFrequency) {
  this->btnPin = btnPin;
  this->photoresistorPin = photoresistorPin;
  this->ledPin = ledPin;
  this->servoPin = servoPin;
  this->monitoringFrequency = monitoringFrequency;

  this->heatingError = 0;
  this->heatingStatus = 0;
  this->heatingStatusPr = 0;

  this->needToToggle = 0;
}

void Heating::begin(bool winterMode, bool userSelect) {
  this->userSelect = userSelect;
  this->winterMode = winterMode;

  pinMode(btnPin, INPUT);

  pinMode(ledPin, OUTPUT);
  heatingStatus = analogRead(photoresistorPin) > 1000;
  heatingStatusPr = heatingStatus;
  digitalWrite(ledPin, heatingStatus);

  myservo.attach(servoPin);
  myservo.write(0);
}

void Heating::heatingMonitor() {
  buttonMonitor();
  if (needToToggle) {
    togglingHeating();
  }

  static unsigned long systemMonitorTimer;
  if (millis() - systemMonitorTimer >= monitoringFrequency) {
    systemMonitorTimer = millis();

    heatingStatus = analogRead(photoresistorPin) > 1000;

    if (winterMode) {
      digitalWrite(ledPin, heatingStatus);
    }

    statusChange = heatingStatus != heatingStatusPr;
    heatingStatusPr = heatingStatus;

    if (statusChange) {
      heatingError = 0;
    }
  }
}

bool Heating::statusChanded() {
  if (statusChange) {
    statusChange = 0;
    return true;
  }
  return statusChange;
}

bool Heating::togglingError() {
  return heatingError;
}

bool Heating::status() {
  return heatingStatus;
}

void Heating::buttonMonitor() {
  if (winterMode) {
    static bool btnPr = 0;
    static unsigned long btnTimer;
    if (digitalRead(btnPin) != btnPr && millis() - btnTimer >= 250) {
      btnPr = digitalRead(btnPin);
      btnTimer = millis();

      if (btnPr) {
        userToggleHeating();
      }
    }
  }
}

void Heating::setMode(bool mode) {
  winterMode = mode;
  if (!winterMode) userSelect = 0;
}

bool Heating::getMode() {
  return winterMode;
}

void Heating::userToggleHeating() {
  userSelect = !(analogRead(photoresistorPin) > 1000);
  toggleHeating();
}

bool Heating::getUserSelect() {
  return userSelect;
}

void Heating::toggleHeating() {
  needToToggle = 1;
  togglingHeating();
}

void Heating::togglingHeating() {
  static bool clicked = 0;
  static unsigned long toggleTimer;
  static bool btnBf;
  if (millis() - toggleTimer > 1000 && !clicked) {
    toggleTimer = millis();

    heatingStatus = analogRead(photoresistorPin) > 1000;
    btnBf = heatingStatus;

    myservo.write(35);
    clicked = 1;
  }
  if (millis() - toggleTimer >= 500 && millis() - toggleTimer < 1000 && needToToggle) {
    myservo.write(0);

    heatingStatus = analogRead(photoresistorPin) > 1000;
    digitalWrite(ledPin, heatingStatus);
    heatingError = heatingStatus == btnBf;

    if (heatingStatus != heatingStatusPr || heatingError || heatingStatus != btnBf) {
      statusChange = 1;
      heatingStatusPr = heatingStatus;
    }

    clicked = 0;
    needToToggle = 0;
  }
}
