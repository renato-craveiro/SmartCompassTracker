#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <time.h>
#include <EEPROM.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "MEO-2BE7F0";
const char* password = "40ef12f659";
AsyncWebServer server(80);

int passosAtuais = 123;
int objetivoDiario = 5000;
long passosGlobais = 0;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

String getTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "Sem hora";
  }
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
  return String(buffer);
}

void loadEEPROM() {
  EEPROM.begin(64);
  EEPROM.get(0, passosGlobais);
  EEPROM.get(10, objetivoDiario);
  if(passosGlobais < 0) passosGlobais = 0;
  if(objetivoDiario <= 0) objetivoDiario = 5000;
}

void saveEEPROM() {
  EEPROM.put(0, passosGlobais);
  EEPROM.put(10, objetivoDiario);
  EEPROM.commit();
}

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED nao encontrado"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  WiFi.begin(ssid, password);
  display.setCursor(0,0);
  display.println("Conectando WiFi...");
  display.display();
  while(WiFi.status() != WL_CONNECTED) delay(500);

  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  loadEEPROM();

  // Página principal
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <meta charset="utf-8">
        <title>Monitor de Passos</title>
        <style>
          body { font-family: Arial; text-align: center; background: #f4f4f4; margin: 0; padding: 0; }
          .card { background: white; max-width: 400px; margin: 40px auto; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1);}
          h1 { color: #333; }
          p { font-size: 18px; }
          .ok { color: green; font-weight: bold; }
          .warn { color: red; font-weight: bold; }
          input { font-size: 16px; padding: 5px; width: 100px; }
          button { font-size: 16px; padding: 5px 10px; }
        </style>
      </head>
      <body>
        <div class="card">
          <h1>🏃 Monitor de Passos</h1>
          <p><b>Hora atual:</b> <span id="time">%TIME%</span></p>
          <p><b>Passos atuais:</b> <span id="steps">%STEPS%</span></p>
          <p><b>Passos globais:</b> <span id="global">%GLOBAL%</span></p>
          <p><b>Objetivo diário:</b> <span id="goal">%GOAL%</span></p>
          <p class="%STATUS_CLASS%" id="status">%STATUS%</p>

          <h3>Alterar Objetivo</h3>
          <form id="goalForm">
            <input type="number" id="newGoal" placeholder="Novo objetivo">
            <button type="submit">Salvar</button>
          </form>
        </div>

        <script>
          // Atualiza os valores sem recarregar a página
          setInterval(() => {
            fetch('/status')
            .then(res => res.json())
            .then(data => {
              document.getElementById('time').innerText = data.time;
              document.getElementById('steps').innerText = data.steps;
              document.getElementById('global').innerText = data.global;
              document.getElementById('goal').innerText = data.goal;
              const status = data.steps >= data.goal ? "🎉 Objetivo atingido!" : "➡️ Continue caminhando!";
              document.getElementById('status').innerText = status;
              document.getElementById('status').className = data.steps >= data.goal ? "ok" : "warn";
            });
          }, 5000);

          // Envia novo objetivo
          document.getElementById('goalForm').addEventListener('submit', function(e){
            e.preventDefault();
            const goal = document.getElementById('newGoal').value;
            fetch('/setGoal?value=' + goal)
            .then(() => {
              document.getElementById('newGoal').value = '';
            });
          });
        </script>
      </body>
      </html>
    )rawliteral";

    html.replace("%TIME%", getTime());
    html.replace("%STEPS%", String(passosAtuais));
    html.replace("%GLOBAL%", String(passosGlobais));
    html.replace("%GOAL%", String(objetivoDiario));
    if(passosAtuais >= objetivoDiario) {
      html.replace("%STATUS%", "🎉 Objetivo atingido!");
      html.replace("%STATUS_CLASS%", "ok");
    } else {
      html.replace("%STATUS%", "➡️ Continue caminhando!");
      html.replace("%STATUS_CLASS%", "warn");
    }

    request->send(200, "text/html", html);
  });

  // Endpoint AJAX para status
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"time\":\"" + getTime() + "\",";
    json += "\"steps\":" + String(passosAtuais) + ",";
    json += "\"global\":" + String(passosGlobais) + ",";
    json += "\"goal\":" + String(objetivoDiario);
    json += "}";
    request->send(200, "application/json", json);
  });

  // Endpoint para alterar objetivo
  server.on("/setGoal", HTTP_GET, [](AsyncWebServerRequest *request){
    if(request->hasParam("value")) {
      objetivoDiario = request->getParam("value")->value().toInt();
      saveEEPROM();
    }
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

unsigned long lastEEPROM = 0;

void loop() {
  // Salva EEPROM a cada minuto
  if(millis() - lastEEPROM > 60000) {
    saveEEPROM();
    lastEEPROM = millis();
  }

  // Atualiza OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Monitor de Passos");
  display.setCursor(0,16);
  display.print("Atuais: "); display.println(passosAtuais);
  display.setCursor(0,28);
  display.print("Globais: "); display.println(passosGlobais);
  display.setCursor(0,40);
  display.print("Objetivo: "); display.println(objetivoDiario);
  display.setCursor(0,52);
  display.print("Hora: "); display.println(getTime());
  display.display();

  delay(1000);
}
