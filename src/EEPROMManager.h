#pragma once
#include <EEPROM.h>

namespace EEPROMManager {
    void load();
    void save();

    extern int objetivoDiario;
    extern long passosGlobais;
}
