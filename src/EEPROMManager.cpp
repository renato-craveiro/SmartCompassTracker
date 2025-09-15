#include <EEPROM.h>
#include <ArduinoJson.h>

namespace EEPROMManager {
    const int EEPROM_SIZE = 512;  // maior espaço para JSON
    const int JSON_SIZE = 256;    // tamanho máximo do JSON

    int objetivoDiario = 5000;
    float passosGlobais = 0;
    float distanciaGlobal = 0;
    //String usuario = "Raoky";

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
            //usuario = String((const char*)doc["usuario"] | "Raoky");
        }
    }

    void save() {
        StaticJsonDocument<JSON_SIZE> doc;
        doc["objetivoDiario"] = objetivoDiario;
        doc["passosGlobais"] = passosGlobais;
        doc["distanciaGlobal"] = distanciaGlobal;
        //doc["usuario"] = usuario;

        char jsonBuffer[JSON_SIZE];
        serializeJson(doc, jsonBuffer, JSON_SIZE);

        for(int i = 0; i < JSON_SIZE; i++) {
            EEPROM.write(i, jsonBuffer[i]);
        }
        EEPROM.commit();
    }
}
