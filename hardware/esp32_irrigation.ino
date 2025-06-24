/*
 * Sistema de Riego Inteligente con ESP32
 * Alimentado con Energía Solar + WiFi
 * 
 * Características:
 * - WiFi para monitoreo remoto
 * - Web server integrado
 * - API REST para control
 * - Almacenamiento en memoria flash
 * - Múltiples sensores
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

// Configuración WiFi
const char* ssid = "TU_WIFI_SSID";
const char* password = "TU_WIFI_PASSWORD";

// Configuración de pines ESP32
#define SOIL_MOISTURE_PIN 36    // VP
#define PUMP_RELAY_PIN 26       // GPIO26
#define SOLAR_CHARGE_PIN 39     // VN
#define BATTERY_VOLTAGE_PIN 34  // GPIO34
#define LIGHT_SENSOR_PIN 35     // GPIO35
#define BUTTON_PIN 2            // GPIO2
#define LED_STATUS_PIN 13       // GPIO13

// Configuración del sistema
#define MIN_HUMIDITY_THRESHOLD 30
#define MAX_HUMIDITY_THRESHOLD 80
#define WATERING_DURATION 30
#define CHECK_INTERVAL 300000
#define BATTERY_LOW_THRESHOLD 11.0
#define BATTERY_HIGH_THRESHOLD 14.0

// Variables globales
WebServer server(80);
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

bool isWatering = false;
bool systemEnabled = true;
unsigned long lastCheck = 0;
unsigned long wateringStartTime = 0;
int soilMoisture = 0;
float batteryVoltage = 0.0;
float solarVoltage = 0.0;
int lightLevel = 0;
String deviceIP = "";

// Estructura para configuración
struct SystemConfig {
  int minHumidity;
  int maxHumidity;
  int wateringDuration;
  bool autoMode;
  int checkInterval;
  String wifiSSID;
  String wifiPassword;
} config;

void setup() {
  Serial.begin(115200);
  Serial.println("🌱 Sistema de Riego Inteligente ESP32");
  Serial.println("=====================================");
  
  // Configurar pines
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Inicializar relé (apagado)
  digitalWrite(PUMP_RELAY_PIN, LOW);
  
  // Inicializar SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("❌ Error: No se pudo inicializar SPIFFS");
  } else {
    Serial.println("✅ SPIFFS inicializado");
  }
  
  // Inicializar RTC
  if (!rtc.begin()) {
    Serial.println("❌ Error: No se pudo inicializar el RTC");
  } else {
    Serial.println("✅ RTC inicializado");
  }
  
  // Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ESP32 Riego");
  lcd.setCursor(0, 1);
  lcd.print("Conectando...");
  
  // Cargar configuración
  loadConfig();
  
  // Conectar WiFi
  connectWiFi();
  
  // Configurar servidor web
  setupWebServer();
  
  // Mostrar información
  displaySystemInfo();
  
  Serial.println("✅ Sistema inicializado correctamente");
}

void loop() {
  // Manejar conexiones web
  server.handleClient();
  
  // Leer sensores
  readSensors();
  
  // Verificar botón manual
  checkManualButton();
  
  // Control de riego automático
  if (systemEnabled && config.autoMode) {
    automaticIrrigation();
  }
  
  // Control de riego manual
  if (isWatering) {
    checkWateringStatus();
  }
  
  // Verificar estado de la batería
  checkBatteryStatus();
  
  // Actualizar display
  updateDisplay();
  
  // Enviar datos por serial
  sendDataToSerial();
  
  // Verificar conexión WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi desconectado, reconectando...");
    connectWiFi();
  }
  
  delay(1000);
}

void connectWiFi() {
  Serial.print("📶 Conectando a WiFi: ");
  Serial.println(config.wifiSSID);
  
  WiFi.begin(config.wifiSSID.c_str(), config.wifiPassword.c_str());
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    deviceIP = WiFi.localIP().toString();
    Serial.println();
    Serial.print("✅ WiFi conectado. IP: ");
    Serial.println(deviceIP);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi: ");
    lcd.print(deviceIP);
  } else {
    Serial.println();
    Serial.println("❌ Error: No se pudo conectar al WiFi");
  }
}

void setupWebServer() {
  // Página principal
  server.on("/", HTTP_GET, handleRoot);
  
  // API endpoints
  server.on("/api/status", HTTP_GET, handleGetStatus);
  server.on("/api/water", HTTP_POST, handleStartWatering);
  server.on("/api/stop", HTTP_POST, handleStopWatering);
  server.on("/api/config", HTTP_GET, handleGetConfig);
  server.on("/api/config", HTTP_POST, handleSetConfig);
  server.on("/api/sensors", HTTP_GET, handleGetSensors);
  
  // Archivos estáticos
  server.on("/style.css", HTTP_GET, handleCSS);
  server.on("/script.js", HTTP_GET, handleJS);
  
  server.begin();
  Serial.println("🌐 Servidor web iniciado");
}

void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Sistema de Riego Inteligente</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="/style.css">
</head>
<body>
    <div class="container">
        <h1>🌱 Sistema de Riego Inteligente</h1>
        
        <div class="status-card">
            <h2>📊 Estado del Sistema</h2>
            <div id="status"></div>
        </div>
        
        <div class="control-card">
            <h2>🎛️ Control Manual</h2>
            <button id="waterBtn" onclick="startWatering()">💧 Iniciar Riego</button>
            <button id="stopBtn" onclick="stopWatering()">🛑 Detener Riego</button>
        </div>
        
        <div class="config-card">
            <h2>⚙️ Configuración</h2>
            <form id="configForm">
                <label>Humedad Mínima (%):</label>
                <input type="number" id="minHumidity" min="0" max="100">
                
                <label>Humedad Máxima (%):</label>
                <input type="number" id="maxHumidity" min="0" max="100">
                
                <label>Duración de Riego (seg):</label>
                <input type="number" id="wateringDuration" min="5" max="300">
                
                <label>Modo Automático:</label>
                <input type="checkbox" id="autoMode">
                
                <button type="submit">💾 Guardar Configuración</button>
            </form>
        </div>
    </div>
    
    <script src="/script.js"></script>
</body>
</html>
  )";
  
  server.send(200, "text/html", html);
}

void handleGetStatus() {
  DynamicJsonDocument doc(512);
  doc["isWatering"] = isWatering;
  doc["systemEnabled"] = systemEnabled;
  doc["soilMoisture"] = soilMoisture;
  doc["batteryVoltage"] = batteryVoltage;
  doc["solarVoltage"] = solarVoltage;
  doc["lightLevel"] = lightLevel;
  doc["autoMode"] = config.autoMode;
  doc["wifiConnected"] = WiFi.status() == WL_CONNECTED;
  doc["deviceIP"] = deviceIP;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleStartWatering() {
  if (!isWatering) {
    startWatering();
    server.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Riego iniciado\"}");
  } else {
    server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Ya hay un riego en curso\"}");
  }
}

void handleStopWatering() {
  if (isWatering) {
    stopWatering();
    server.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Riego detenido\"}");
  } else {
    server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"No hay riego en curso\"}");
  }
}

void handleGetConfig() {
  DynamicJsonDocument doc(256);
  doc["minHumidity"] = config.minHumidity;
  doc["maxHumidity"] = config.maxHumidity;
  doc["wateringDuration"] = config.wateringDuration;
  doc["autoMode"] = config.autoMode;
  doc["checkInterval"] = config.checkInterval;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleSetConfig() {
  String body = server.arg("plain");
  DynamicJsonDocument doc(256);
  deserializeJson(doc, body);
  
  if (doc.containsKey("minHumidity")) config.minHumidity = doc["minHumidity"];
  if (doc.containsKey("maxHumidity")) config.maxHumidity = doc["maxHumidity"];
  if (doc.containsKey("wateringDuration")) config.wateringDuration = doc["wateringDuration"];
  if (doc.containsKey("autoMode")) config.autoMode = doc["autoMode"];
  if (doc.containsKey("checkInterval")) config.checkInterval = doc["checkInterval"];
  
  saveConfig();
  server.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Configuración guardada\"}");
}

void handleGetSensors() {
  DynamicJsonDocument doc(512);
  doc["soilMoisture"] = soilMoisture;
  doc["batteryVoltage"] = batteryVoltage;
  doc["solarVoltage"] = solarVoltage;
  doc["lightLevel"] = lightLevel;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleCSS() {
  String css = R"(
body {
    font-family: Arial, sans-serif;
    margin: 0;
    padding: 20px;
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    min-height: 100vh;
}

.container {
    max-width: 800px;
    margin: 0 auto;
}

h1 {
    text-align: center;
    color: white;
    margin-bottom: 30px;
}

.status-card, .control-card, .config-card {
    background: white;
    border-radius: 10px;
    padding: 20px;
    margin-bottom: 20px;
    box-shadow: 0 4px 6px rgba(0,0,0,0.1);
}

button {
    background: #4CAF50;
    color: white;
    border: none;
    padding: 10px 20px;
    border-radius: 5px;
    cursor: pointer;
    margin: 5px;
    font-size: 16px;
}

button:hover {
    background: #45a049;
}

button:disabled {
    background: #cccccc;
    cursor: not-allowed;
}

input, label {
    display: block;
    margin: 10px 0;
}

input[type="number"] {
    width: 100%;
    padding: 8px;
    border: 1px solid #ddd;
    border-radius: 4px;
}

#status {
    font-size: 18px;
    line-height: 1.6;
}
  )";
  
  server.send(200, "text/css", css);
}

void handleJS() {
  String js = R"(
function updateStatus() {
    fetch('/api/status')
        .then(response => response.json())
        .then(data => {
            document.getElementById('status').innerHTML = `
                <p><strong>Riego:</strong> ${data.isWatering ? '💧 ACTIVO' : '⏸️ INACTIVO'}</p>
                <p><strong>Humedad:</strong> ${data.soilMoisture}%</p>
                <p><strong>Batería:</strong> ${data.batteryVoltage}V</p>
                <p><strong>Panel Solar:</strong> ${data.solarVoltage}V</p>
                <p><strong>Luz:</strong> ${data.lightLevel}</p>
                <p><strong>Modo Automático:</strong> ${data.autoMode ? 'ON' : 'OFF'}</p>
                <p><strong>WiFi:</strong> ${data.wifiConnected ? 'Conectado' : 'Desconectado'}</p>
                <p><strong>IP:</strong> ${data.deviceIP}</p>
            `;
            
            document.getElementById('waterBtn').disabled = data.isWatering;
            document.getElementById('stopBtn').disabled = !data.isWatering;
        });
}

function startWatering() {
    fetch('/api/water', {method: 'POST'})
        .then(response => response.json())
        .then(data => {
            alert(data.message);
            updateStatus();
        });
}

function stopWatering() {
    fetch('/api/stop', {method: 'POST'})
        .then(response => response.json())
        .then(data => {
            alert(data.message);
            updateStatus();
        });
}

function loadConfig() {
    fetch('/api/config')
        .then(response => response.json())
        .then(data => {
            document.getElementById('minHumidity').value = data.minHumidity;
            document.getElementById('maxHumidity').value = data.maxHumidity;
            document.getElementById('wateringDuration').value = data.wateringDuration;
            document.getElementById('autoMode').checked = data.autoMode;
        });
}

document.getElementById('configForm').addEventListener('submit', function(e) {
    e.preventDefault();
    
    const config = {
        minHumidity: parseInt(document.getElementById('minHumidity').value),
        maxHumidity: parseInt(document.getElementById('maxHumidity').value),
        wateringDuration: parseInt(document.getElementById('wateringDuration').value),
        autoMode: document.getElementById('autoMode').checked,
        checkInterval: 300000
    };
    
    fetch('/api/config', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(config)
    })
    .then(response => response.json())
    .then(data => {
        alert(data.message);
    });
});

// Actualizar cada 2 segundos
setInterval(updateStatus, 2000);
updateStatus();
loadConfig();
  )";
  
  server.send(200, "application/javascript", js);
}

// Funciones del sistema (similares a la versión Arduino)
void readSensors() {
  // Leer sensor de humedad del suelo
  int rawMoisture = analogRead(SOIL_MOISTURE_PIN);
  soilMoisture = map(rawMoisture, 4095, 0, 0, 100);  // ESP32 tiene 12 bits
  
  // Leer voltaje de la batería
  int rawBattery = analogRead(BATTERY_VOLTAGE_PIN);
  batteryVoltage = (rawBattery * 3.3 / 4095.0) * 3.0;
  
  // Leer voltaje del panel solar
  int rawSolar = analogRead(SOLAR_CHARGE_PIN);
  solarVoltage = (rawSolar * 3.3 / 4095.0) * 3.0;
  
  // Leer sensor de luz
  lightLevel = analogRead(LIGHT_SENSOR_PIN);
  
  // Filtrar lecturas erróneas
  soilMoisture = constrain(soilMoisture, 0, 100);
}

void automaticIrrigation() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastCheck >= config.checkInterval) {
    lastCheck = currentTime;
    
    Serial.print("🔍 Verificando humedad: ");
    Serial.print(soilMoisture);
    Serial.println("%");
    
    if (soilMoisture < config.minHumidity && !isWatering) {
      Serial.println("💧 Humedad baja, iniciando riego automático");
      startWatering();
    } else if (soilMoisture > config.maxHumidity && isWatering) {
      Serial.println("🛑 Humedad alta, deteniendo riego");
      stopWatering();
    }
  }
}

void startWatering() {
  if (!isWatering && batteryVoltage > BATTERY_LOW_THRESHOLD) {
    isWatering = true;
    wateringStartTime = millis();
    digitalWrite(PUMP_RELAY_PIN, HIGH);
    digitalWrite(LED_STATUS_PIN, HIGH);
    
    Serial.println("💧 Riego iniciado");
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RIEGO ACTIVO");
    lcd.setCursor(0, 1);
    lcd.print("Humedad: ");
    lcd.print(soilMoisture);
    lcd.print("%");
  }
}

void stopWatering() {
  if (isWatering) {
    isWatering = false;
    digitalWrite(PUMP_RELAY_PIN, LOW);
    digitalWrite(LED_STATUS_PIN, LOW);
    
    Serial.println("💧 Riego detenido");
    
    unsigned long duration = (millis() - wateringStartTime) / 1000;
    Serial.print("⏱️ Duración del riego: ");
    Serial.print(duration);
    Serial.println(" segundos");
  }
}

void checkWateringStatus() {
  unsigned long currentTime = millis();
  unsigned long elapsed = (currentTime - wateringStartTime) / 1000;
  
  if (elapsed >= config.wateringDuration) {
    Serial.println("⏰ Tiempo de riego completado");
    stopWatering();
  }
  
  if (soilMoisture > config.maxHumidity) {
    Serial.println("💧 Humedad alcanzada, deteniendo riego");
    stopWatering();
  }
}

void checkManualButton() {
  static bool lastButtonState = HIGH;
  static unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;
  
  bool reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && lastButtonState == HIGH) {
      if (!isWatering) {
        Serial.println("🔘 Riego manual iniciado");
        startWatering();
      } else {
        Serial.println("🔘 Riego manual detenido");
        stopWatering();
      }
    }
  }
  
  lastButtonState = reading;
}

void checkBatteryStatus() {
  if (batteryVoltage < BATTERY_LOW_THRESHOLD) {
    Serial.println("⚠️ BATERÍA BAJA - Deshabilitando riego");
    systemEnabled = false;
    if (isWatering) {
      stopWatering();
    }
  } else if (batteryVoltage > BATTERY_HIGH_THRESHOLD) {
    systemEnabled = true;
  }
  
  if (solarVoltage > batteryVoltage + 0.5) {
    Serial.println("☀️ Cargando batería");
  }
}

void updateDisplay() {
  static unsigned long lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate > 2000) {
    lastDisplayUpdate = millis();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("H:");
    lcd.print(soilMoisture);
    lcd.print("% B:");
    lcd.print(batteryVoltage, 1);
    lcd.print("V");
    
    lcd.setCursor(0, 1);
    if (isWatering) {
      lcd.print("RIEGO ACTIVO");
    } else {
      lcd.print("Esperando...");
    }
  }
}

void sendDataToSerial() {
  static unsigned long lastDataSend = 0;
  if (millis() - lastDataSend > 5000) {
    lastDataSend = millis();
    
    Serial.print("📊 DATOS: ");
    Serial.print("Humedad=");
    Serial.print(soilMoisture);
    Serial.print("%, Bateria=");
    Serial.print(batteryVoltage, 1);
    Serial.print("V, Solar=");
    Serial.print(solarVoltage, 1);
    Serial.print("V, Luz=");
    Serial.print(lightLevel);
    Serial.print(", Riego=");
    Serial.print(isWatering ? "ON" : "OFF");
    Serial.print(", WiFi=");
    Serial.print(WiFi.status() == WL_CONNECTED ? "ON" : "OFF");
    Serial.println();
  }
}

void displaySystemInfo() {
  Serial.println("📋 Configuración del Sistema:");
  Serial.print("  - Humedad mínima: ");
  Serial.print(config.minHumidity);
  Serial.println("%");
  Serial.print("  - Humedad máxima: ");
  Serial.print(config.maxHumidity);
  Serial.println("%");
  Serial.print("  - Duración de riego: ");
  Serial.print(config.wateringDuration);
  Serial.println(" segundos");
  Serial.print("  - Modo automático: ");
  Serial.println(config.autoMode ? "ON" : "OFF");
  Serial.print("  - Intervalo de verificación: ");
  Serial.print(config.checkInterval / 1000);
  Serial.println(" segundos");
}

void loadConfig() {
  File file = SPIFFS.open("/config.json", "r");
  if (file) {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, file);
    file.close();
    
    config.minHumidity = doc["minHumidity"] | MIN_HUMIDITY_THRESHOLD;
    config.maxHumidity = doc["maxHumidity"] | MAX_HUMIDITY_THRESHOLD;
    config.wateringDuration = doc["wateringDuration"] | WATERING_DURATION;
    config.autoMode = doc["autoMode"] | true;
    config.checkInterval = doc["checkInterval"] | CHECK_INTERVAL;
    config.wifiSSID = doc["wifiSSID"] | "TU_WIFI_SSID";
    config.wifiPassword = doc["wifiPassword"] | "TU_WIFI_PASSWORD";
  } else {
    // Configuración por defecto
    config.minHumidity = MIN_HUMIDITY_THRESHOLD;
    config.maxHumidity = MAX_HUMIDITY_THRESHOLD;
    config.wateringDuration = WATERING_DURATION;
    config.autoMode = true;
    config.checkInterval = CHECK_INTERVAL;
    config.wifiSSID = "TU_WIFI_SSID";
    config.wifiPassword = "TU_WIFI_PASSWORD";
    saveConfig();
  }
}

void saveConfig() {
  DynamicJsonDocument doc(512);
  doc["minHumidity"] = config.minHumidity;
  doc["maxHumidity"] = config.maxHumidity;
  doc["wateringDuration"] = config.wateringDuration;
  doc["autoMode"] = config.autoMode;
  doc["checkInterval"] = config.checkInterval;
  doc["wifiSSID"] = config.wifiSSID;
  doc["wifiPassword"] = config.wifiPassword;
  
  File file = SPIFFS.open("/config.json", "w");
  if (file) {
    serializeJson(doc, file);
    file.close();
    Serial.println("✅ Configuración guardada");
  }
} 