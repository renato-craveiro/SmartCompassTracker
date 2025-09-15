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
#include "MPUManager.h"
#include <Wire.h>


unsigned long lastEEPROM = 0;



void clearEEPROM() {
  EEPROM.begin(512); // Tamanho da EEPROM em bytes (ajusta ao que usas)
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0); // ou 0xFF
  }
  EEPROM.commit();
}

void setup() {
    const char* ssid = "xxxx"; 
    const char* password = "xxxx";
    
    //DEBUG
    clearEEPROM();
    //DEBUG
    
    Serial.begin(115200);

    Wire.begin(4, 5); // SDA=GPIO4, SCL=GPIO5


    OLEDManager::beginDisplay();
    MPUManager::init();
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
    MPUManager::update();
    OLEDManager::update(StepsManager::passosAtuais, StepsManager::passosGlobais, StepsManager::objetivoDiario, StepsManager::distanciaAtual, StepsManager::distanciaGlobal);
    delay(1000);
}
