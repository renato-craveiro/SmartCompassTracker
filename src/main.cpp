/*
 * "Smart" Compass Tracker - Main Application File
 * 
 * This file initializes and manages the main components of the Smart Compass Tracker project,
 * including WiFi, EEPROM, OLED display, web server, and step counting logic.
 * 
 * Author: Renato Craveiro
 * Date: Sep. 2025
 */


#include "WiFiManager.h"
#include "EEPROMManager.h"
#include "OLEDManager.h"
#include "WebServer.h"
#include "StepsManager.h"
#include "utils.h"

unsigned long lastEEPROM = 0;

void setup() {
    const char* ssid = "xxx"; 
    const char* password = "xxx";
    Serial.begin(115200);

    OLEDManager::beginDisplay();
    EEPROMManager::load();
    WiFiManager::connect(ssid, password);
    StepsManager::init();
    WebServer::init(); // já inicializa o server dentro do namespace
    Utils::initTime();
}

void loop() {
    if (millis() - lastEEPROM > 60000) {
        EEPROMManager::save();
        lastEEPROM = millis();
    }

    OLEDManager::update(StepsManager::passosAtuais, StepsManager::passosGlobais, StepsManager::objetivoDiario);
    delay(1000);
}
