#pragma once
#include <EEPROM.h>
#include <Arduino.h> // <-- Add this line

namespace EEPROMManager {
    void load();
    void save();

    extern int objetivoDiario;
    extern long passosGlobais;
    extern float distanciaGlobal;
    extern String wifiSSID;
    extern String wifiPassword;
}
