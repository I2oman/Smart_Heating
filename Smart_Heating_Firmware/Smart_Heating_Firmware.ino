#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>
#include "maintenance.h"

// #define MAINTENANCE

#define LGRS 36
#define LED 16
#define BTN 4
#define SERVO 17

const char *ssid = "WiFi-SSID";
const char *password = "Password";

IPAddress local_IP(192, 168, 0, 225);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

Servo myservo;

bool toggle_error = 0;

bool toggleHeating = 0;

bool ledState = 0;

void setup() {
  Serial.begin(115200);

  pinMode(BTN, INPUT);

  pinMode(LED, OUTPUT);
  ledState = analogRead(LGRS) > 1500;
  digitalWrite(LED, ledState);

  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }

#ifdef MAINTENANCE
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

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  myservo.attach(SERVO);
  myservo.write(0);

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

    DynamicJsonDocument stateDoc(1024);
    stateDoc["request_type"] = "init_update";
    if (!toggle_error) {
      stateDoc["heatingStatus"] = analogRead(LGRS) > 1500 ? "heatingOn" : "heatingOff";
    } else {
      stateDoc["heatingStatus"] = "toggle_error";
    }
    String state;
    serializeJson(stateDoc, state);
    client->text(state);

  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
  } else if (type == WS_EVT_DATA) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, data, len);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    for (JsonPair kv : doc.as<JsonObject>()) {
      const char *key = kv.key().c_str();
      JsonVariant value = kv.value();

      Serial.printf("Key: %s, Value: ", key);
      if (value.is<const char *>()) {
        Serial.printf("%s\n", value.as<const char *>());
      } else if (value.is<int>()) {
        Serial.printf("%d\n", value.as<int>());
      } else if (value.is<bool>()) {
        Serial.printf("%s\n", value.as<bool>() ? "true" : "false");
      } else if (value.is<float>()) {
        Serial.printf("%f\n", value.as<float>());
      } else {
        Serial.println("Unknown type");
      }

      // Process actions based on key-value pairs
      if (strcmp(key, "toggleHeating") == 0) {
        int action = value.as<int>();
        if (action == 1) {
          toggleHeating = 1;
        }
      } else if (strcmp(key, "servoWrite") == 0) {
        int action = value.as<int>();
        myservo.write(action);
      }
    }

    // Send a response back to the client
    DynamicJsonDocument responseDoc(1024);
    responseDoc["status"] = "success";
    responseDoc["message"] = "Command received";
    String response;
    serializeJson(responseDoc, response);
    client->text(response);
  }
}

void loop() {
  ws.cleanupClients();
  buttonMonitor();
  if (toggleHeating) {
    toggleHeatingVoid();
  }
  systemMonitoring();
}

void buttonMonitor() {
  static bool btnPr;
  static unsigned long btnTimer;
  if (digitalRead(BTN) != btnPr && millis() - btnTimer >= 250) {
    btnPr = digitalRead(BTN);
    btnTimer = millis();

    if (btnPr) {
      toggleHeating = 1;
    }
  }
}

void systemMonitoring() {
  static unsigned long systemMonitorTimer;
  if (millis() - systemMonitorTimer >= 1000) {
    systemMonitorTimer = millis();

    ledState = analogRead(LGRS) > 1500;
    digitalWrite(LED, ledState);

    DynamicJsonDocument sensorData(1024);
    sensorData["request_type"] = "status_update";
    sensorData["lgRsVal"] = analogRead(LGRS);
    if (!toggle_error) {
      sensorData["heatingStatus"] = analogRead(LGRS) > 1500 ? "heatingOn" : "heatingOff";
    } else {
      sensorData["heatingStatus"] = "toggle_error";
    }
    sensorData["btnVal"] = digitalRead(BTN);
    sensorData["ledVal"] = ledState;

    String message;
    serializeJson(sensorData, message);
    ws.textAll(message);
  }
}

void toggleHeatingVoid() {
  static bool prevState;
  static unsigned long toggleTimer;
  if (millis() - toggleTimer > 1000) {
    toggleTimer = millis();
    prevState = analogRead(LGRS) > 1500;
    myservo.write(20);
  }
  if (millis() - toggleTimer >= 500 && millis() - toggleTimer < 1000) {
    ledState = analogRead(LGRS) > 1500;
    digitalWrite(LED, ledState);

    DynamicJsonDocument sensorData(1024);
    sensorData["request_type"] = "toggle_response";
    sensorData["heatingStatus"] = prevState != (analogRead(LGRS) > 1500) ? analogRead(LGRS) > 1500 ? "heatingOn" : "heatingOff" : "toggle_error";
    toggle_error = sensorData["heatingStatus"] == "toggle_error";

    String message;
    serializeJson(sensorData, message);
    ws.textAll(message);

    myservo.write(0);
    toggleHeating = 0;
  }
}
