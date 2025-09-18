#include "WiFiManager.h"
#include <Wire.h>
#include "OLEDManager.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "EEPROMManager.h"

ESP8266WebServer server(80);

namespace WiFiManager {

void handleRoot() {
    int n = WiFi.scanNetworks();
    String html = "<form action='/save' method='POST'>"
                  "SSID: <select name='ssid'>";
    for (int i = 0; i < n; ++i) {
        html += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
    }
    html += "</select><br>"
            "Password: <input name='password' type='password'><br>"
            "<input type='submit'></form>";
    server.send(200, "text/html", html);
}

void handleSave() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    EEPROMManager::wifiSSID = ssid;
    EEPROMManager::wifiPassword = password;
    EEPROMManager::save();
    server.send(200, "text/html", "Saved! Rebooting...");
    delay(1000);
    ESP.restart();
}

void startAP() {
    WiFi.softAP("CompassConfig");
    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);
    OLEDManager::printMessage(("IP: " + apIP.toString()).c_str());
    server.on("/", handleRoot);
    server.on("/save", HTTP_POST, handleSave);
    server.begin();
    while (true) {
        server.handleClient();
        delay(10);
    }
}

void connect(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    OLEDManager::printMessage("Connecting WiFi...");
    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
        delay(500);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi conectado: " + WiFi.localIP().toString());
    } else {
        Serial.println("\nFailed to connect. Starting AP mode.");
        startAP();
    }
}

}