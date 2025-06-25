/*
 * Prueba Básica - Sistema de Riego Arduino
 * 
 * Este sketch es para probar los componentes básicos:
 * - Sensor de humedad del suelo
 * - Relé y bomba de agua
 * - LED de estado
 * - Botón manual
 * 
 * Conexiones:
 * - Sensor humedad: A0
 * - Relé bomba: D7
 * - LED estado: D13
 * - Botón manual: D2
 */

// Pines
#define SOIL_MOISTURE_PIN A0    // Sensor de humedad
#define PUMP_RELAY_PIN 7        // Relé para bomba
#define LED_STATUS_PIN 13       // LED de estado
#define BUTTON_PIN 2            // Botón manual

// Variables
int soilMoisture = 0;
bool isWatering = false;
bool buttonPressed = false;

void setup() {
  // Inicializar comunicación serial
  Serial.begin(9600);
  Serial.println("🌱 PRUEBA BÁSICA - Sistema de Riego Arduino");
  Serial.println("============================================");
  
  // Configurar pines
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Estado inicial
  digitalWrite(PUMP_RELAY_PIN, LOW);  // Bomba apagada
  digitalWrite(LED_STATUS_PIN, LOW);  // LED apagado
  
  Serial.println("✅ Sistema inicializado");
  Serial.println("📋 Comandos disponibles:");
  Serial.println("  - 'READ' - Leer sensor de humedad");
  Serial.println("  - 'PUMP_ON' - Encender bomba");
  Serial.println("  - 'PUMP_OFF' - Apagar bomba");
  Serial.println("  - 'TEST' - Prueba automática");
  Serial.println("  - 'CALIBRATE' - Calibrar sensor");
  Serial.println();
}

void loop() {
  // Leer botón manual
  checkButton();
  
  // Leer comandos seriales
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    processCommand(command);
  }
  
  // Actualizar LED de estado
  digitalWrite(LED_STATUS_PIN, isWatering);
  
  delay(100);  // Pequeña pausa
}

void checkButton() {
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
      Serial.println("🔘 Botón manual presionado");
      if (!isWatering) {
        startWatering();
      } else {
        stopWatering();
      }
    }
  }
  
  lastButtonState = reading;
}

void processCommand(String command) {
  command.toUpperCase();
  
  if (command == "READ") {
    readSoilMoisture();
  }
  else if (command == "PUMP_ON") {
    startWatering();
  }
  else if (command == "PUMP_OFF") {
    stopWatering();
  }
  else if (command == "TEST") {
    runTest();
  }
  else if (command == "CALIBRATE") {
    calibrateSensor();
  }
  else if (command == "STATUS") {
    showStatus();
  }
  else {
    Serial.println("❌ Comando no reconocido: " + command);
    Serial.println("📋 Comandos válidos: READ, PUMP_ON, PUMP_OFF, TEST, CALIBRATE, STATUS");
  }
}

void readSoilMoisture() {
  int rawValue = analogRead(SOIL_MOISTURE_PIN);
  soilMoisture = map(rawValue, 1023, 0, 0, 100);  // Invertir valores
  
  Serial.print("🌱 Humedad del suelo: ");
  Serial.print(soilMoisture);
  Serial.print("% (Raw: ");
  Serial.print(rawValue);
  Serial.println(")");
  
  // Interpretación básica
  if (soilMoisture < 30) {
    Serial.println("💧 Suelo muy seco - necesita riego");
  } else if (soilMoisture < 60) {
    Serial.println("🌱 Suelo moderadamente seco");
  } else if (soilMoisture < 80) {
    Serial.println("✅ Suelo húmedo - bien");
  } else {
    Serial.println("💦 Suelo muy húmedo - no necesita riego");
  }
}

void startWatering() {
  if (!isWatering) {
    isWatering = true;
    digitalWrite(PUMP_RELAY_PIN, HIGH);
    Serial.println("💧 Riego INICIADO");
  } else {
    Serial.println("⚠️ El riego ya está activo");
  }
}

void stopWatering() {
  if (isWatering) {
    isWatering = false;
    digitalWrite(PUMP_RELAY_PIN, LOW);
    Serial.println("💧 Riego DETENIDO");
  } else {
    Serial.println("⚠️ El riego no está activo");
  }
}

void runTest() {
  Serial.println("🧪 INICIANDO PRUEBA AUTOMÁTICA");
  Serial.println("================================");
  
  // 1. Leer sensor
  Serial.println("1️⃣ Leyendo sensor de humedad...");
  readSoilMoisture();
  delay(1000);
  
  // 2. Probar LED
  Serial.println("2️⃣ Probando LED de estado...");
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_STATUS_PIN, HIGH);
    delay(200);
    digitalWrite(LED_STATUS_PIN, LOW);
    delay(200);
  }
  Serial.println("✅ LED probado");
  
  // 3. Probar bomba (corto tiempo)
  Serial.println("3️⃣ Probando bomba (3 segundos)...");
  startWatering();
  delay(3000);
  stopWatering();
  
  // 4. Estado final
  Serial.println("4️⃣ Estado final:");
  showStatus();
  
  Serial.println("✅ PRUEBA COMPLETADA");
}

void calibrateSensor() {
  Serial.println("🔧 CALIBRACIÓN DEL SENSOR");
  Serial.println("==========================");
  Serial.println("Coloca el sensor en diferentes condiciones:");
  Serial.println();
  
  Serial.println("1️⃣ Sensor en el aire (seco):");
  Serial.println("   Coloca el sensor en el aire y presiona cualquier tecla...");
  while (!Serial.available()) {
    delay(100);
  }
  Serial.read();  // Limpiar buffer
  int dryValue = analogRead(SOIL_MOISTURE_PIN);
  Serial.print("   Valor en seco: ");
  Serial.println(dryValue);
  
  Serial.println();
  Serial.println("2️⃣ Sensor en agua (húmedo):");
  Serial.println("   Coloca el sensor en agua y presiona cualquier tecla...");
  while (!Serial.available()) {
    delay(100);
  }
  Serial.read();  // Limpiar buffer
  int wetValue = analogRead(SOIL_MOISTURE_PIN);
  Serial.print("   Valor en húmedo: ");
  Serial.println(wetValue);
  
  Serial.println();
  Serial.println("📊 RESULTADOS DE CALIBRACIÓN:");
  Serial.print("   Seco (0%): ");
  Serial.println(dryValue);
  Serial.print("   Húmedo (100%): ");
  Serial.println(wetValue);
  Serial.println();
  Serial.println("💡 Para usar estos valores, modifica la función map() en el código:");
  Serial.print("   soilMoisture = map(rawValue, ");
  Serial.print(dryValue);
  Serial.print(", ");
  Serial.print(wetValue);
  Serial.println(", 0, 100);");
}

void showStatus() {
  Serial.println("📊 ESTADO DEL SISTEMA");
  Serial.println("=====================");
  Serial.print("💧 Riego: ");
  Serial.println(isWatering ? "ACTIVO" : "INACTIVO");
  Serial.print("🌱 Humedad: ");
  Serial.print(soilMoisture);
  Serial.println("%");
  Serial.print("🔘 Botón: ");
  Serial.println(digitalRead(BUTTON_PIN) == HIGH ? "NO PRESIONADO" : "PRESIONADO");
  Serial.print("💡 LED: ");
  Serial.println(isWatering ? "ENCENDIDO" : "APAGADO");
  Serial.println();
} 