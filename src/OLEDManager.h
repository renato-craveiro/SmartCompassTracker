#pragma once
#include <Adafruit_SSD1306.h>

namespace OLEDManager {
    void beginDisplay();
    void update(int passosAtuais, long passosGlobais, int objetivoDiario, float distanciaAtual, float distanciaGlobal);
    void printMessage(const char* message);
}
