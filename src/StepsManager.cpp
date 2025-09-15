#include "StepsManager.h"
#include "EEPROMManager.h"

namespace StepsManager {
    int passosAtuais = 0;
    int objetivoDiario = 5000;
    long passosGlobais = 0;
    float distanciaGlobal = 0; // km
    float distanciaAtual = 0; // km

    void init() {
        passosGlobais = EEPROMManager::passosGlobais;
        objetivoDiario = EEPROMManager::objetivoDiario;
        distanciaGlobal = EEPROMManager::distanciaGlobal;
        passosAtuais = 0;
    }
}
