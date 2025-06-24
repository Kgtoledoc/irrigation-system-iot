/*
 * Sistema de Riego Inteligente con Arduino
 * Alimentado con Energía Solar
 * 
 * Componentes:
 * - Arduino Nano/Uno/Mega
 * - Sensor de humedad del suelo (capacitivo)
 * - Módulo relé 5V
 * - Bomba de agua 12V
 * - Módulo de carga solar
 * - Batería LiPo 12V
 * - Panel solar 20W
 * - Módulo RTC DS3231 (opcional)
 * - Display LCD 16x2 (opcional)
 * - Módulo WiFi ESP8266 (para conectividad)
 */

#include <Wire.h>
#include <RTClib.h>  // Para reloj en tiempo real
#include <LiquidCrystal_I2C.h>  // Para display LCD
#include <EEPROM.h>

// Configuración de pines
#define SOIL_MOISTURE_PIN A0    // Sensor de humedad del suelo
#define PUMP_RELAY_PIN 7        // Relé para la bomba
#define SOLAR_CHARGE_PIN A1     // Voltaje del panel solar
#define BATTERY_VOLTAGE_PIN A2  // Voltaje de la batería
#define LIGHT_SENSOR_PIN A3     // Sensor de luz (opcional)
#define BUTTON_PIN 2            // Botón manual
#define LED_STATUS_PIN 13       // LED de estado

// Configuración del sistema
#define MIN_HUMIDITY_THRESHOLD 30    // Umbral mínimo de humedad (%)
#define MAX_HUMIDITY_THRESHOLD 80    // Umbral máximo de humedad (%)
#define WATERING_DURATION 30         // Duración del riego (segundos)
#define CHECK_INTERVAL 300000        // Intervalo de verificación (5 minutos)
#define BATTERY_LOW_THRESHOLD 11.0   // Voltaje mínimo de batería (V)
#define BATTERY_HIGH_THRESHOLD 14.0  // Voltaje máximo de batería (V)

// Variables globales
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C del LCD

bool isWatering = false;
bool systemEnabled = true;
unsigned long lastCheck = 0;
unsigned long wateringStartTime = 0;
int soilMoisture = 0;
float batteryVoltage = 0.0;
float solarVoltage = 0.0;
int lightLevel = 0;

// Estructura para configuración
struct SystemConfig {
  int minHumidity;
  int maxHumidity;
  int wateringDuration;
  bool autoMode;
  int checkInterval;
} config;

void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600);
  Serial.println("🌱 Sistema de Riego Inteligente Solar");
  Serial.println("=====================================");
  
  // Configurar pines
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Inicializar relé (apagado)
  digitalWrite(PUMP_RELAY_PIN, LOW);
  
  // Inicializar RTC
  if (!rtc.begin()) {
    Serial.println("❌ Error: No se pudo inicializar el RTC");
  } else {
    Serial.println("✅ RTC inicializado correctamente");
    // Establecer fecha/hora si es necesario
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  // Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Riego");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  
  // Cargar configuración desde EEPROM
  loadConfig();
  
  // Mostrar información inicial
  displaySystemInfo();
  
  Serial.println("✅ Sistema inicializado correctamente");
}

void loop() {
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
  
  // Esperar antes de la siguiente verificación
  delay(1000);
}

void readSensors() {
  // Leer sensor de humedad del suelo
  int rawMoisture = analogRead(SOIL_MOISTURE_PIN);
  soilMoisture = map(rawMoisture, 1023, 0, 0, 100);  // Invertir valores
  
  // Leer voltaje de la batería
  int rawBattery = analogRead(BATTERY_VOLTAGE_PIN);
  batteryVoltage = (rawBattery * 5.0 / 1023.0) * 3.0;  // Divisor de voltaje 1:3
  
  // Leer voltaje del panel solar
  int rawSolar = analogRead(SOLAR_CHARGE_PIN);
  solarVoltage = (rawSolar * 5.0 / 1023.0) * 3.0;
  
  // Leer sensor de luz
  lightLevel = analogRead(LIGHT_SENSOR_PIN);
  
  // Filtrar lecturas erróneas
  soilMoisture = constrain(soilMoisture, 0, 100);
}

void automaticIrrigation() {
  unsigned long currentTime = millis();
  
  // Verificar si es momento de revisar
  if (currentTime - lastCheck >= config.checkInterval) {
    lastCheck = currentTime;
    
    Serial.print("🔍 Verificando humedad: ");
    Serial.print(soilMoisture);
    Serial.println("%");
    
    // Verificar si necesita riego
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
    
    // Mostrar en LCD
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
    
    // Mostrar duración del riego
    unsigned long duration = (millis() - wateringStartTime) / 1000;
    Serial.print("⏱️ Duración del riego: ");
    Serial.print(duration);
    Serial.println(" segundos");
  }
}

void checkWateringStatus() {
  unsigned long currentTime = millis();
  unsigned long elapsed = (currentTime - wateringStartTime) / 1000;
  
  // Verificar si debe detenerse por tiempo
  if (elapsed >= config.wateringDuration) {
    Serial.println("⏰ Tiempo de riego completado");
    stopWatering();
  }
  
  // Verificar si debe detenerse por humedad alta
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
      // Botón presionado
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
  
  // Mostrar estado de carga
  if (solarVoltage > batteryVoltage + 0.5) {
    Serial.println("☀️ Cargando batería");
  }
}

void updateDisplay() {
  static unsigned long lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate > 2000) {  // Actualizar cada 2 segundos
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
  if (millis() - lastDataSend > 5000) {  // Enviar cada 5 segundos
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
  // Cargar configuración desde EEPROM
  EEPROM.get(0, config);
  
  // Si es la primera vez, usar valores por defecto
  if (config.minHumidity == 0) {
    config.minHumidity = MIN_HUMIDITY_THRESHOLD;
    config.maxHumidity = MAX_HUMIDITY_THRESHOLD;
    config.wateringDuration = WATERING_DURATION;
    config.autoMode = true;
    config.checkInterval = CHECK_INTERVAL;
    saveConfig();
  }
}

void saveConfig() {
  EEPROM.put(0, config);
}

// Función para cambiar configuración por serial
void processSerialCommand() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.startsWith("SET_MIN:")) {
      config.minHumidity = command.substring(8).toInt();
      saveConfig();
      Serial.println("✅ Humedad mínima actualizada");
    }
    else if (command.startsWith("SET_MAX:")) {
      config.maxHumidity = command.substring(8).toInt();
      saveConfig();
      Serial.println("✅ Humedad máxima actualizada");
    }
    else if (command.startsWith("SET_DURATION:")) {
      config.wateringDuration = command.substring(13).toInt();
      saveConfig();
      Serial.println("✅ Duración de riego actualizada");
    }
    else if (command == "AUTO_ON") {
      config.autoMode = true;
      saveConfig();
      Serial.println("✅ Modo automático activado");
    }
    else if (command == "AUTO_OFF") {
      config.autoMode = false;
      saveConfig();
      Serial.println("✅ Modo automático desactivado");
    }
    else if (command == "WATER_NOW") {
      if (!isWatering) {
        startWatering();
      }
    }
    else if (command == "STOP_WATER") {
      if (isWatering) {
        stopWatering();
      }
    }
    else if (command == "STATUS") {
      displaySystemInfo();
    }
  }
} 