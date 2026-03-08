#include "web.h"

/* Web server */
WebServer server(80);
Preferences prefs;

/* ===== STORAGE ===== */

void saveSettings() {
    prefs.begin("linebot", false);

    prefs.putFloat("kp", Kp);
    prefs.putFloat("ki", Ki);
    prefs.putFloat("kd", Kd);

    prefs.putInt("sens", sensitivity);
    prefs.putInt("avg", sensorAverage);
    prefs.putInt("offset", sensitivityOffset);

    prefs.end();
}

void loadSettings() {
    prefs.begin("linebot", true);

    Kp = prefs.getFloat("kp", 0.0);
    Ki = prefs.getFloat("ki", 0.0);
    Kd = prefs.getFloat("kd", 0.0);

    sensitivity = prefs.getInt("sens", 0);
    sensorAverage = prefs.getInt("avg", 0);
    sensitivityOffset = prefs.getInt("offset", 30);

    prefs.end();
}

/* ===== HTML PAGE ===== */

String htmlPage() {
    return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Line Robot Control</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 40px;
        }

        input {
            width: 140px;
            padding: 6px;
            margin: 6px;
            border: 1px solid #ccc;
            border-radius: 4px;
        }

        input[readonly] {
            background-color: #f0f0f0;
            cursor: not-allowed;
        }

        button {
            padding: 12px 20px;
            margin: 10px;
            font-size: 18px;
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 4px;
            cursor: pointer;
        }

        button:hover {
            background-color: #45a049;
        }
    </style>
</head>
<body>
    <h2>Line Robot Control</h2>

    <div>
        <label>Kp</label><input id="kp" type="number" step="0.01"><br>
        <label>Ki</label><input id="ki" type="number" step="0.01"><br>
        <label>Kd</label><input id="kd" type="number" step="0.01"><br>
        <label>Sensitivity</label><input id="sens" type="number"><br>
        <label>Sensor Avg</label><input id="avg" type="number" readonly><br>
        <label>Sensitivity Offset</label><input id="offset" type="number"><br>
    </div>

    <button onclick="saveData()">Save Parameters</button><br>
    <button onclick="setSensitivity()">Set Sensitivity</button><br>
    <button onclick="fetch('/start')">START</button>
    <button onclick="fetch('/stop')">STOP</button>

    <script>
        function loadData() {
            fetch("/get")
                .then(r => r.json())
                .then(data => {
                    document.getElementById("kp").value = data.kp;
                    document.getElementById("ki").value = data.ki;
                    document.getElementById("kd").value = data.kd;
                    document.getElementById("sens").value = data.sens;
                    document.getElementById("avg").value = data.avg;
                    document.getElementById("offset").value = data.offset;
                });
        }

        function saveData() {
            let form = new FormData();

            form.append("kp", document.getElementById("kp").value);
            form.append("ki", document.getElementById("ki").value);
            form.append("kd", document.getElementById("kd").value);
            form.append("sens", document.getElementById("sens").value);
            form.append("offset", document.getElementById("offset").value);

            fetch("/set", { method: "POST", body: form });
        }

        function setSensitivity() {
            let form = new FormData();
            form.append("setSensAuto", "1");
            fetch("/set", { method: "POST", body: form });
        }

        window.onload = function() {
            loadData();
            setInterval(loadData, 100);
        };
    </script>
</body>
</html>
)rawliteral";
}

/* ===== HANDLERS ===== */

void handleRoot() {
    server.send(200, "text/html", htmlPage());
}

void handleStart() {
    robotStarted = true;
    server.send(200, "text/plain", "START");
}

void handleStop() {
    robotStarted = false;
    server.send(200, "text/plain", "STOP");
}

void handleSet() {
    if (server.hasArg("kp")) Kp = server.arg("kp").toFloat();
    if (server.hasArg("ki")) Ki = server.arg("ki").toFloat();
    if (server.hasArg("kd")) Kd = server.arg("kd").toFloat();

    if (server.hasArg("sens")) sensitivity = server.arg("sens").toInt();
    if (server.hasArg("offset")) sensitivityOffset = server.arg("offset").toInt();

    if (server.hasArg("setSensAuto")) {
        sensitivity = sensorAverage - sensitivityOffset;
    }

    saveSettings();

    server.send(200, "text/plain", "OK");
}

void handleGet() {
    String json = "{";
    json += "\"kp\":" + String(Kp, 2) + ",";
    json += "\"ki\":" + String(Ki, 2) + ",";
    json += "\"kd\":" + String(Kd, 2) + ",";
    json += "\"sens\":" + String(sensitivity) + ",";
    json += "\"avg\":" + String(sensorAverage) + ",";
    json += "\"offset\":" + String(sensitivityOffset);
    json += "}";

    server.send(200, "application/json", json);
}

/* ===== PUBLIC ===== */

void beginWebServer() {
    loadSettings();

    server.on("/", handleRoot);
    server.on("/start", handleStart);
    server.on("/stop", handleStop);
    server.on("/set", HTTP_POST, handleSet);
    server.on("/get", handleGet);

    server.begin();
}

void handleWebServer() {
    server.handleClient();
}