#pragma once
#include <Adafruit_SSD1306.h>

namespace OLEDManager {
    void beginDisplay();
    void update(int passosAtuais, long passosGlobais, int objetivoDiario);
    void printMessage(const char* message);
}
