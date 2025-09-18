#include <EEPROM.h>
#include <ArduinoJson.h>
#include "OLEDManager.h"
namespace EEPROMManager {
    const int EEPROM_SIZE = 512;  
    const int JSON_SIZE = 256;    

    int objetivoDiario = 5000;
    float passosGlobais = 0;
    float distanciaGlobal = 0;
    //String usuario = "Raoky";

    String wifiSSID = "";
    String wifiPassword = "";

    void begin() {
        EEPROM.begin(EEPROM_SIZE);
    }

    void load() {
        begin();
        char jsonBuffer[JSON_SIZE];
        for(int i = 0; i < JSON_SIZE; i++) {
            jsonBuffer[i] = EEPROM.read(i);
        }

        StaticJsonDocument<JSON_SIZE> doc;
        DeserializationError error = deserializeJson(doc, jsonBuffer);
        if(!error) {
            objetivoDiario = doc["objetivoDiario"] | 5000;
            passosGlobais = doc["passosGlobais"] | 0;
            distanciaGlobal = doc["distanciaGlobal"] | 0;
            wifiSSID = doc["wifiSSID"] | "";
            wifiPassword = doc["wifiPassword"] | "";
        }
        
    }

    void save() {
        StaticJsonDocument<JSON_SIZE> doc;
        doc["objetivoDiario"] = objetivoDiario;
        doc["passosGlobais"] = passosGlobais;
        doc["distanciaGlobal"] = distanciaGlobal;
        doc["wifiSSID"] = wifiSSID;
        doc["wifiPassword"] = wifiPassword;

        char jsonBuffer[JSON_SIZE];
        serializeJson(doc, jsonBuffer, JSON_SIZE);

        for(int i = 0; i < JSON_SIZE; i++) {
            EEPROM.write(i, jsonBuffer[i]);
        }
        EEPROM.commit();
    }
}
