#include "WebServer.h"
#include "EEPROMManager.h"
#include "StepsManager.h"
#include "utils.h"
//extern AsyncWebServer server;

namespace WebServer {
// Definição real da variável dentro do namespace
AsyncWebServer server(80);
void init() {
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

        html.replace("%TIME%", Utils::getTime());
        html.replace("%STEPS%", String(StepsManager::passosAtuais));
        html.replace("%GLOBAL%", String(StepsManager::passosGlobais));
        html.replace("%GOAL%", String(StepsManager::objetivoDiario));
        if(StepsManager::passosAtuais >= StepsManager::objetivoDiario) {
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
        json += "\"time\":\"" + Utils::getTime() + "\",";
        json += "\"steps\":" + String(StepsManager::passosAtuais) + ",";
        json += "\"global\":" + String(StepsManager::passosGlobais) + ",";
        json += "\"goal\":" + String(StepsManager::objetivoDiario);
        json += "}";
        request->send(200, "application/json", json);
    });

    // Endpoint para alterar objetivo
    server.on("/setGoal", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            StepsManager::objetivoDiario = request->getParam("value")->value().toInt();

            // Atualiza EEPROMManager para gravar
            EEPROMManager::objetivoDiario = StepsManager::objetivoDiario;
            EEPROMManager::save();
        //saveEEPROM();
        }
        request->send(200, "text/plain", "OK");
    });

    server.begin();
}

}
