#include "OLEDManager.h"
#include <U8g2lib.h>
#include "utils.h"

namespace OLEDManager {

// Cria objeto U8g2 para OLED 128x64 I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

void beginDisplay() {
    u8g2.begin();
}

void update(int passosAtuais, long passosGlobais, int objetivoDiario, float distanciaAtual, float distanciaGlobal) {
    // Barra de progresso para o objetivo diário
    int progress = map(passosAtuais, 0, objetivoDiario, 0, 128); // 0-128 pixels horizontalmente
    if(progress > 128) progress = 128;

    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB08_tr); // Fonte legível

        // Título centralizado
        u8g2.setFontMode(1);
        u8g2.setDrawColor(1);
        u8g2.drawStr(0, 10, "Monitor de Passos");

        // Informações principais
        u8g2.setFont(u8g2_font_5x7_tr);
        u8g2.setCursor(0, 24); u8g2.print("Atuais: "); u8g2.print(passosAtuais); u8g2.print(" - Dist: "); u8g2.print(distanciaAtual,3); u8g2.print(" km");
        u8g2.setCursor(0, 34); u8g2.print("Globais: "); u8g2.print(passosGlobais); u8g2.print(" - Dist: "); u8g2.print(distanciaGlobal,3); u8g2.print(" km");
        u8g2.setCursor(0, 44); u8g2.print("Objetivo: "); u8g2.print(objetivoDiario);
        u8g2.setCursor(0, 54); u8g2.print("Hora: "); u8g2.print(Utils::getTime());

        // Barra de progresso
        u8g2.drawFrame(0, 56, 128, 6);       // contorno da barra
        u8g2.drawBox(0, 56, progress, 6);    // preenchimento
    } while(u8g2.nextPage());
}

void printMessage(const char* message) {
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 30, message);
    } while(u8g2.nextPage());
}

} // namespace
