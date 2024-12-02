#include <Arduino.h>
#include <time.h>
#include <esp_sntp.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#include "WiFiCredentials.h"

// #define MAINTENANCE

#ifdef MAINTENANCE
#include "maintenance.h"
#endif

#include "Heating.h"
#include "WeeklyTimers.h"

#define LGRS 36
#define LED 16
#define BTN 4
#define SERVO 17

const char *time_zone = "GMT0BST,M3.5.0/1,M10.5.0";
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";

IPAddress local_IP(192, 168, 0, 225);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

Heating heating(BTN, LGRS, LED, SERVO);
WeeklyTimers heatingTimers;

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }

#ifdef MAINTENANCE
  delay(5000);
  refreshFiles();
  while (1) {}
#endif

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("WiFi Failed!\n");
      return;
    }
  }

  configTzTime(time_zone, ntpServer1, ntpServer2);

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  loadWeeklyTimers();

  setupServer();
}

void setupServer() {
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("controlpanel.html");
  server.serveStatic("/controlpanel", SPIFFS, "/").setDefaultFile("controlpanel.html");
  server.serveStatic("/timers", SPIFFS, "/").setDefaultFile("timers.html");
  server.serveStatic("/about", SPIFFS, "/").setDefaultFile("about.html");
  server.onNotFound(notFound);

  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  server.begin();
  Serial.println("HTTP server started");
}

void notFound(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/notFound.html", "text/html");
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("WebSocket client connected");

    DynamicJsonDocument systemStatus(1024);
    systemStatus["action"] = "init_update";
    systemStatus["heatingStatus"] = !heating.togglingError() ? heating.status() ? "heatingOn" : "heatingOff" : "toggle_error";
    systemStatus["heatingMode"] = heating.getMode();

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      char dateBuffer[11];
      snprintf(dateBuffer, sizeof(dateBuffer), "%02d/%02d/%04d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
      systemStatus["date"] = dateBuffer;

      char timeBuffer[6];
      snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
      systemStatus["time"] = timeBuffer;
    } else {
      Serial.println("Failed to obtain time");
    }

    String status;
    serializeJson(systemStatus, status);
    client->text(status);

  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
  } else if (type == WS_EVT_DATA) {
    DynamicJsonDocument requestJson(1024);
    DeserializationError error = deserializeJson(requestJson, data, len);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    String output;
    serializeJson(requestJson, output);
    Serial.println(output);

    const char *action = requestJson["action"];

    if (strcmp(action, "switchHeatingMode") == 0) {
      if (requestJson["heatingMode"].as<bool>()) {
        heating.setMode(1);
        if (heating.togglingError()) return;

        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
          uint16_t currentTimeValue = timeinfo.tm_hour * 60 + timeinfo.tm_min;

          if (heatingTimers.getDayTimerCount() == 0) {
            if (heating.status()) heating.toggleHeating();
            return;
          }

          static bool dayHaveTimer = 0;

          for (DayTimer dayTimer : heatingTimers.getDayTimersList()) {
            DayTimer::DayPair days = dayTimer.getDays();
            if ((days.startDay <= (timeinfo.tm_wday + 6) % 7) && ((timeinfo.tm_wday + 6) % 7 <= days.endDay)) {
              dayHaveTimer = 1;
              for (HeatingTimer heatingTimer : dayTimer.getTimersList()) {
                if ((heatingTimer.getValue()[0] <= currentTimeValue) && (currentTimeValue <= heatingTimer.getValue()[1])) {
                  if (heating.togglingError()) return;
                  heating.userToggleHeating();
                }
              }
            }
          }

          if (!dayHaveTimer) {
            if (heating.togglingError()) return;
            if (heating.status()) heating.toggleHeating();
          }
        } else {
          if (heating.togglingError()) return;
          if (heating.status()) heating.toggleHeating();
        }

      } else {
        if (!heating.togglingError() && heating.status()) heating.userToggleHeating();
        heating.setMode(0);
      }
      Serial.println(heating.getMode());

      saveWeeklyTimers();
    } else if (strcmp(action, "togleHeating") == 0) {
      if (heating.getMode()) {
        heating.userToggleHeating();
      }
    } else if (strcmp(action, "addDayTimer") == 0) {
      DayTimer dayTimer;
      JsonObject jsonDayTimer = requestJson["dayTimer"].as<JsonObject>();

      DayTimer::DayPair dayPair;
      uint8_t startDay = jsonDayTimer["startDay"];
      uint8_t endDay = jsonDayTimer["endDay"];
      dayPair = { startDay, endDay };

      dayTimer.modifyDays(dayPair);

      JsonArray timersArray = jsonDayTimer["timers"].as<JsonArray>();
      for (JsonObject jsonTimer : timersArray) {
        HeatingTimer heatingTimer;

        HeatingTimer::Time startTime;
        uint8_t startTimeHours = jsonTimer["startTime"]["hours"];
        uint8_t startTimeMinutes = jsonTimer["startTime"]["minutes"];
        startTime = { startTimeHours, startTimeMinutes };

        HeatingTimer::Time endTime;
        uint8_t endTimeHours = jsonTimer["endTime"]["hours"];
        uint8_t endTimeMinutes = jsonTimer["endTime"]["minutes"];
        endTime = { endTimeHours, endTimeMinutes };

        HeatingTimer::TimePair timePair = { startTime, endTime };

        heatingTimer.modify(timePair);

        dayTimer.addTimer(heatingTimer);
      }

      heatingTimers.addDayTimer(dayTimer);

      ws.textAll(getDayTimersSJson());

      saveWeeklyTimers();
    } else if (strcmp(action, "modifyDayTimer") == 0) {
      unsigned long dayTimerId = requestJson["dayTimerId"].as<unsigned long>();
      JsonArray changes = requestJson["changes"].as<JsonArray>();
      for (JsonObject change : changes) {
        const char *changeAction = change["action"];
        if (strcmp(changeAction, "modifyDays") == 0) {
          JsonObject jsonDayTimer = change["dayTimer"].as<JsonObject>();

          DayTimer::DayPair dayPair;
          uint8_t startDay = jsonDayTimer["startDay"];
          uint8_t endDay = jsonDayTimer["endDay"];
          dayPair = { startDay, endDay };

          heatingTimers.getDayTimer(dayTimerId).modifyDays(dayPair);
        } else if (strcmp(changeAction, "addTimers") == 0) {
          JsonArray timersArray = change["timers"].as<JsonArray>();

          for (JsonObject jsonTimer : timersArray) {
            uint8_t startTimeHours = jsonTimer["startTime"]["hours"];
            uint8_t startTimeMinutes = jsonTimer["startTime"]["minutes"];
            HeatingTimer::Time startTime = { startTimeHours, startTimeMinutes };

            uint8_t endTimeHours = jsonTimer["endTime"]["hours"];
            uint8_t endTimeMinutes = jsonTimer["endTime"]["minutes"];
            HeatingTimer::Time endTime = { endTimeHours, endTimeMinutes };

            HeatingTimer heatingTimer({ startTime, endTime });

            heatingTimers.getDayTimer(dayTimerId).addTimer(heatingTimer);
          }
        } else if (strcmp(changeAction, "modifyTimers") == 0) {
          JsonArray timersArray = change["timers"].as<JsonArray>();
          for (JsonObject jsonTimer : timersArray) {
            unsigned long timerId = jsonTimer["timerId"].as<unsigned long>();

            JsonObject timerJson = jsonTimer["timer"].as<JsonObject>();

            uint8_t startTimeHours = timerJson["startTime"]["hours"];
            uint8_t startTimeMinutes = timerJson["startTime"]["minutes"];
            HeatingTimer::Time startTime = { startTimeHours, startTimeMinutes };

            uint8_t endTimeHours = timerJson["endTime"]["hours"];
            uint8_t endTimeMinutes = timerJson["endTime"]["minutes"];
            HeatingTimer::Time endTime = { endTimeHours, endTimeMinutes };

            heatingTimers.getDayTimer(dayTimerId).getTimer(timerId).modify({ startTime, endTime });
          }
        } else if (strcmp(changeAction, "deleteTimers") == 0) {
          JsonArray jsonTimerIds = change["timerIds"].as<JsonArray>();
          for (unsigned long id : jsonTimerIds) {
            heatingTimers.getDayTimer(dayTimerId).deleteTimer(id);
          }
        }
      }

      ws.textAll(getDayTimersSJson());

      saveWeeklyTimers();
    } else if (strcmp(action, "deleteDayTimer") == 0) {
      heatingTimers.deleteDayTimer(requestJson["dayTimerId"].as<unsigned long>());

      ws.textAll(getDayTimersSJson());

      saveWeeklyTimers();
    } else if (strcmp(action, "getDayTimers") == 0) {
      client->text(getDayTimersSJson());
    }

    displayHeatingTimers();

    DynamicJsonDocument responseJson(1024);
    responseJson["status"] = "success";
    responseJson["message"] = "Command received";
    String response;
    serializeJson(responseJson, response);
    client->text(response);
  }
}

String getDayTimersSJson() {
  DynamicJsonDocument dayTimersJson(4096);
  dayTimersJson["action"] = "dayTimersList_update";

  JsonArray dayTimersArray = dayTimersJson.createNestedArray("dayTimers");

  for (const DayTimer &dayTimer : heatingTimers.getDayTimersList()) {
    JsonObject dayTimerObj = dayTimersArray.createNestedObject();

    dayTimerObj["id"] = dayTimer.getId();
    auto days = dayTimer.getDays();
    dayTimerObj["startDay"] = days.startDay;
    dayTimerObj["endDay"] = days.endDay;

    JsonArray timersArray = dayTimerObj.createNestedArray("timers");

    for (const HeatingTimer &heatingTimer : dayTimer.getTimersList()) {
      JsonObject heatingTimerObj = timersArray.createNestedObject();
      heatingTimerObj["id"] = heatingTimer.getId();
      auto timer = heatingTimer.getTimer();
      heatingTimerObj["startTime"]["hours"] = timer.startTime.hours;
      heatingTimerObj["startTime"]["minutes"] = timer.startTime.minutes;
      heatingTimerObj["endTime"]["hours"] = timer.endTime.hours;
      heatingTimerObj["endTime"]["minutes"] = timer.endTime.minutes;
    }
  }

  String response;
  serializeJson(dayTimersJson, response);
  return response;
}

void displayHeatingTimers() {
  Serial.println("Displaying all heating timers:");

  for (const DayTimer &dayTimer : heatingTimers.getDayTimersList()) {
    DayTimer::DayPair days = dayTimer.getDays();
    Serial.printf("DayTimer %lu: %u to %u\n", dayTimer.getId(), days.startDay, days.endDay);

    for (const HeatingTimer heatingTimer : dayTimer.getTimersList()) {
      HeatingTimer::TimePair timePair = heatingTimer.getTimer();
      Serial.printf("  Timer %lu: %u:%u to %u:%u\n", heatingTimer.getId(), timePair.startTime.hours, timePair.startTime.minutes, timePair.endTime.hours, timePair.endTime.minutes);
    }
  }
}

void saveWeeklyTimers() {
  DynamicJsonDocument doc(4096);
  doc["winterMode"] = heating.getMode();

  JsonArray timersArray = doc.createNestedArray("dayTimers");

  for (const DayTimer &dayTimer : heatingTimers.getDayTimersList()) {
    JsonObject dayTimerObj = timersArray.createNestedObject();
    dayTimerObj["id"] = dayTimer.getId();

    JsonObject days = dayTimerObj.createNestedObject("days");
    days["startDay"] = dayTimer.getDays().startDay;
    days["endDay"] = dayTimer.getDays().endDay;

    JsonArray heatingTimersArray = dayTimerObj.createNestedArray("timers");
    for (const HeatingTimer &heatingTimer : dayTimer.getTimersList()) {
      JsonObject heatingTimerObj = heatingTimersArray.createNestedObject();
      heatingTimerObj["id"] = heatingTimer.getId();
      heatingTimerObj["startTime"]["hours"] = heatingTimer.getTimer().startTime.hours;
      heatingTimerObj["startTime"]["minutes"] = heatingTimer.getTimer().startTime.minutes;
      heatingTimerObj["endTime"]["hours"] = heatingTimer.getTimer().endTime.hours;
      heatingTimerObj["endTime"]["minutes"] = heatingTimer.getTimer().endTime.minutes;
    }
  }

  File file = SPIFFS.open("/timers.json", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  if (serializeJson(doc, file) == 0) {
    Serial.println("Failed to write to file");
  }

  file.close();
  Serial.println("Timers saved to SPIFFS");
}

void loadWeeklyTimers() {
  File file = SPIFFS.open("/timers.json");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  DynamicJsonDocument doc(4096);

  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println("Failed to read file, using default configuration");
    heating.begin(0, 0);
    return;
  }
  file.close();

  bool uSelect = 0;

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    uint16_t currentTimeValue = timeinfo.tm_hour * 60 + timeinfo.tm_min;
    JsonArray timersArray = doc["dayTimers"].as<JsonArray>();
    for (JsonObject jsonDayTimer : timersArray) {
      uint8_t startDay = jsonDayTimer["days"]["startDay"];
      uint8_t endDay = jsonDayTimer["days"]["endDay"];
      DayTimer::DayPair dayPair = { startDay, endDay };

      DayTimer dayTimer(dayPair);

      JsonArray timersArray = jsonDayTimer["timers"].as<JsonArray>();
      for (JsonObject jsonTimer : timersArray) {
        uint8_t startTimeHours = jsonTimer["startTime"]["hours"];
        uint8_t startTimeMinutes = jsonTimer["startTime"]["minutes"];
        HeatingTimer::Time startTime = { startTimeHours, startTimeMinutes };

        uint8_t endTimeHours = jsonTimer["endTime"]["hours"];
        uint8_t endTimeMinutes = jsonTimer["endTime"]["minutes"];
        HeatingTimer::Time endTime = { endTimeHours, endTimeMinutes };

        HeatingTimer::TimePair timePair = { startTime, endTime };

        HeatingTimer heatingTimer(timePair);

        dayTimer.addTimer(heatingTimer);

        if ((dayPair.startDay <= (timeinfo.tm_wday + 6) % 7) && ((timeinfo.tm_wday + 6) % 7 <= dayPair.endDay)) {
          if ((heatingTimer.getValue()[0] <= currentTimeValue) && (currentTimeValue <= heatingTimer.getValue()[1])) {
            uSelect = 1;
          }
        }
      }

      heatingTimers.addDayTimer(dayTimer);
    }
  }

  bool wMode = doc["winterMode"];

  Serial.print(wMode);
  Serial.print(" ");
  Serial.println(uSelect);

  heating.begin(wMode, uSelect);
}

void loop() {
  ws.cleanupClients();
  systemStateWebUpdate();
  systemMonitoring();
}

void systemMonitoring() {
  static unsigned long timerMillis = millis();
  static unsigned int msDelay = 0;
  if (millis() - timerMillis >= msDelay) {
    timerMillis = millis();
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      if (msDelay != 5000 || timeinfo.tm_sec % 5 != 0) {
        msDelay = 5 - timeinfo.tm_sec % 5;
        msDelay *= 1000;
        if (msDelay < 500) {
          msDelay = 5000;
        }
      }
      Serial.printf("%02d/%02d/%02d, %02d:%02d:%02d, day - %d, sys - %d, uSelect - %d\n", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, (timeinfo.tm_wday + 6) % 7, heating.status(), heating.getUserSelect());

      DynamicJsonDocument jsonData(1024);
      jsonData["action"] = "time_update";
      char dateBuffer[11];
      snprintf(dateBuffer, sizeof(dateBuffer), "%02d/%02d/%04d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
      jsonData["date"] = dateBuffer;

      char timeBuffer[6];
      snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
      jsonData["time"] = timeBuffer;
      String message;
      serializeJson(jsonData, message);
      ws.textAll(message);

      if (heating.getMode()) {
        if (heating.togglingError()) return;

        if (heatingTimers.getDayTimerCount() == 0) {
          if (heating.status()) heating.toggleHeating();
          return;
        }

        static bool dayHaveTimer = 0;

        for (DayTimer dayTimer : heatingTimers.getDayTimersList()) {
          DayTimer::DayPair days = dayTimer.getDays();
          if ((days.startDay <= (timeinfo.tm_wday + 6) % 7) && ((timeinfo.tm_wday + 6) % 7 <= days.endDay)) {
            dayHaveTimer = 1;
            for (HeatingTimer heatingTimer : dayTimer.getTimersList()) {
              HeatingTimer::TimePair timePair = heatingTimer.getTimer();

              uint16_t currentTimeValue = timeinfo.tm_hour * 60 + timeinfo.tm_min;
              Serial.print(heatingTimer.getValue()[0]);
              Serial.print(" (");
              Serial.print(currentTimeValue);
              Serial.print(") ");
              Serial.print(heatingTimer.getValue()[1]);
              Serial.print(", ");
              Serial.print(heating.status());
              Serial.print(" ");
              Serial.println(heating.getUserSelect());
              if ((heatingTimer.getValue()[0] == currentTimeValue) || (currentTimeValue == heatingTimer.getValue()[1])) {
                if (currentTimeValue == heatingTimer.getValue()[0] && timeinfo.tm_sec <= 10) {
                  if (!heating.status()) heating.userToggleHeating();
                } else if (currentTimeValue == heatingTimer.getValue()[1] && timeinfo.tm_sec <= 10) {
                  if (heating.status()) heating.userToggleHeating();
                }
              }
              if (heating.status() != heating.getUserSelect()) {
                heating.toggleHeating();
              }
            }
          }
        }

        if (!dayHaveTimer) {
          if (heating.togglingError()) return;
          if (heating.status()) heating.toggleHeating();
        }
      } else {
        if (heating.togglingError()) return;
        if (heating.status()) heating.toggleHeating();
      }
    } else {
      Serial.println("Failed to obtain time");
      if (heating.togglingError()) return;
      if (heating.status()) heating.toggleHeating();
    }
  }
}

void systemStateWebUpdate() {
  heating.heatingMonitor();
  if (heating.statusChanded()) {
    DynamicJsonDocument sensorData(1024);
    sensorData["action"] = "status_update";
    if (heating.getMode()) {
      sensorData["heatingStatus"] = !heating.togglingError() ? heating.status() ? "heatingOn" : "heatingOff" : "heatingError";
    } else {
      sensorData["heatingStatus"] = "heatingOff";
    }
    String message;
    serializeJson(sensorData, message);
    ws.textAll(message);
  }
}
