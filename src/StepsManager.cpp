#include "StepsManager.h"
#include "EEPROMManager.h"

namespace StepsManager {
    int passosAtuais = 0;
    int objetivoDiario = 5000;
    long passosGlobais = 0;

    void init() {
        passosGlobais = EEPROMManager::passosGlobais;
        objetivoDiario = EEPROMManager::objetivoDiario;
        passosAtuais = 0;
    }
}
