#pragma once
#include <Arduino.h>
#include <SPIFFS.h>
#include "controlpanel_html.h"
#include "timers_html.h"
#include "about_html.h"
#include "notFound_html.h"
#include "styles_css.h"
#include "script_js.h"

void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
      Serial.println();
      Serial.println("--------------------------------------------------");
      readFile(SPIFFS, file.path());
      Serial.println("--------------------------------------------------");
      Serial.println();
    }
    file = root.openNextFile();
  }
}

void deleteAllFiles(fs::FS &fs, const char *dirname) {
  Serial.printf("Deleting All Files: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      deleteAllFiles(fs, file.path());
    } else {
      deleteFile(SPIFFS, file.path());
    }
    file = root.openNextFile();
  }
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

bool fileExists(fs::FS &fs, const char *path) {
  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    file.close();
    return false;
  }
  file.close();
  return true;
}

void createAllNecessaryFiles() {
  writeFile(SPIFFS, "/controlpanel.html", controlpanel_html);
  writeFile(SPIFFS, "/timers.html", timers_html);
  writeFile(SPIFFS, "/about.html", about_html);
  writeFile(SPIFFS, "/notFound.html", notFound_html);
  writeFile(SPIFFS, "/styles.css", styles_css);
  writeFile(SPIFFS, "/script.js", script_js);
}

void refreshFiles() {
  listDir(SPIFFS, "/", 0);
  deleteAllFiles(SPIFFS, "/");
  listDir(SPIFFS, "/", 0);
  createAllNecessaryFiles();
  listDir(SPIFFS, "/", 0);
}
