#include "WiFiManager.h"
#include <Wire.h>
#include "OLEDManager.h"

namespace WiFiManager {

void connect(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    OLEDManager::printMessage("Conectando WiFi...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi conectado: " + WiFi.localIP().toString());
}

}
