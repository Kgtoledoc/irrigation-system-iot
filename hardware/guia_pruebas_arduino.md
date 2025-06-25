# 🧪 Guía de Pruebas - Sistema de Riego Arduino

## 🎯 **Objetivo**
Esta guía te llevará paso a paso para probar tu sistema de riego Arduino, comenzando con componentes básicos y avanzando hasta el sistema completo.

## 📋 **Lista de Componentes Mínimos para Pruebas**

### **Componentes Básicos (Prueba Inicial)**
- ✅ **Arduino Nano/Uno** - Controlador principal
- ✅ **Cable USB** - Para programar Arduino
- ✅ **Sensor de humedad del suelo** - Capacitivo o resistivo
- ✅ **Módulo relé 5V** - Para controlar la bomba
- ✅ **Bomba de agua 12V** - Pequeña para pruebas
- ✅ **Fuente de alimentación 12V** - Para la bomba
- ✅ **LED** - Para indicador de estado
- ✅ **Botón pulsador** - Control manual
- ✅ **Resistencias 220Ω** - Para LED y botón
- ✅ **Protoboard** - Para conexiones
- ✅ **Cables Dupont** - Para conectar componentes

### **Componentes Adicionales (Pruebas Avanzadas)**
- 🔋 **Batería 12V** - Para funcionamiento autónomo
- ☀️ **Panel solar 20W** - Para energía solar
- 🔋 **Controlador de carga** - Para gestionar la batería
- 📺 **Display LCD 16x2** - Para información local
- ⏰ **Módulo RTC DS3231** - Para reloj en tiempo real

## 🚀 **Paso 1: Configuración Inicial**

### **1.1 Instalar Arduino IDE**
```bash
# Descargar desde: https://www.arduino.cc/en/software
# Instalar y abrir Arduino IDE
```

### **1.2 Configurar Placa**
```
Herramientas → Placa → Arduino Nano (Old Bootloader)
Herramientas → Puerto → COM3 (o el que corresponda)
```

### **1.3 Instalar Librerías (Opcional)**
```
Herramientas → Administrar Bibliotecas
Buscar e instalar:
- LiquidCrystal_I2C (para display)
- RTClib (para RTC)
```

## 🔌 **Paso 2: Conexiones Básicas**

### **2.1 Esquema de Conexiones Mínimas**
```
Arduino Nano:
├── 5V ──→ Sensor Humedad VCC
├── GND ──→ Sensor Humedad GND
├── A0 ──→ Sensor Humedad AOUT
├── D7 ──→ Relé IN
├── D13 ──→ LED + (con resistencia 220Ω)
├── D2 ──→ Botón (con resistencia pull-up)
└── GND ──→ LED -, Botón, Relé GND

Relé:
├── VCC ──→ Arduino 5V
├── GND ──→ Arduino GND
├── IN ──→ Arduino D7
├── COM ──→ 12V+
└── NO ──→ Bomba +

Bomba:
├── + ──→ Relé NO
└── - ──→ GND
```

### **2.2 Verificación de Conexiones**
1. ✅ **Verificar polaridad** de todos los componentes
2. ✅ **Verificar que no haya cortocircuitos**
3. ✅ **Verificar que el relé esté en posición OFF**
4. ✅ **Conectar fuente de alimentación 12V**

## 💻 **Paso 3: Prueba Básica**

### **3.1 Subir Código de Prueba**
1. Abrir `hardware/arduino_test_basic.ino` en Arduino IDE
2. Verificar que la placa y puerto estén correctos
3. Hacer clic en "Subir" (flecha hacia la derecha)
4. Esperar mensaje "Subida completada"

### **3.2 Abrir Monitor Serial**
```
Herramientas → Monitor Serie
Velocidad: 9600 baudios
```

### **3.3 Comandos de Prueba**
Escribir estos comandos en el monitor serial:

```
STATUS          # Ver estado del sistema
READ            # Leer sensor de humedad
CALIBRATE       # Calibrar sensor
TEST            # Prueba automática completa
PUMP_ON         # Encender bomba
PUMP_OFF        # Apagar bomba
```

## 🧪 **Paso 4: Pruebas Individuales**

### **4.1 Prueba del Sensor de Humedad**
```bash
# Comando: READ
# Resultado esperado:
🌱 Humedad del suelo: 45% (Raw: 563)
🌱 Suelo moderadamente seco
```

**Si no funciona:**
- Verificar conexiones VCC, GND, A0
- Probar con `CALIBRATE` para ajustar valores
- Verificar que el sensor esté bien insertado en el suelo

### **4.2 Prueba del Relé y Bomba**
```bash
# Comando: PUMP_ON
# Resultado esperado:
💧 Riego INICIADO

# Comando: PUMP_OFF
# Resultado esperado:
💧 Riego DETENIDO
```

**Si no funciona:**
- Verificar conexiones del relé
- Verificar fuente de alimentación 12V
- Verificar conexiones de la bomba
- Verificar que el relé esté bien conectado

### **4.3 Prueba del Botón Manual**
```bash
# Presionar botón físico
# Resultado esperado:
🔘 Botón manual presionado
💧 Riego INICIADO (o DETENIDO)
```

**Si no funciona:**
- Verificar conexión del botón a D2 y GND
- Verificar resistencia pull-up
- Verificar que el botón esté bien soldado/conectado

### **4.4 Prueba del LED**
```bash
# Comando: TEST
# Resultado esperado:
2️⃣ Probando LED de estado...
✅ LED probado
```

**Si no funciona:**
- Verificar conexión LED a D13 y GND
- Verificar resistencia 220Ω
- Verificar polaridad del LED

## 🔧 **Paso 5: Calibración del Sensor**

### **5.1 Ejecutar Calibración**
```bash
# Comando: CALIBRATE
# Seguir las instrucciones en pantalla
```

### **5.2 Proceso de Calibración**
1. **Sensor en aire (seco)**: Colocar sensor en el aire
2. **Sensor en agua (húmedo)**: Colocar sensor en agua
3. **Anotar valores**: Seco y húmedo
4. **Modificar código**: Usar valores obtenidos

### **5.3 Ejemplo de Calibración**
```
📊 RESULTADOS DE CALIBRACIÓN:
   Seco (0%): 950
   Húmedo (100%): 200

💡 Para usar estos valores, modifica la función map():
   soilMoisture = map(rawValue, 950, 200, 0, 100);
```

## 🚀 **Paso 6: Prueba Automática Completa**

### **6.1 Ejecutar Test Completo**
```bash
# Comando: TEST
# El sistema realizará:
1️⃣ Leyendo sensor de humedad...
2️⃣ Probando LED de estado...
3️⃣ Probando bomba (3 segundos)...
4️⃣ Estado final:
```

### **6.2 Verificar Resultados**
- ✅ Sensor lee correctamente
- ✅ LED parpadea 5 veces
- ✅ Bomba funciona 3 segundos
- ✅ Estado se muestra correctamente

## 🔄 **Paso 7: Prueba del Sistema Completo**

### **7.1 Subir Código Completo**
Una vez que las pruebas básicas funcionen:
1. Abrir `hardware/arduino_irrigation.ino`
2. Configurar parámetros según tus necesidades
3. Subir código al Arduino

### **7.2 Configuración Inicial**
```cpp
// Ajustar estos valores según tu calibración
#define MIN_HUMIDITY_THRESHOLD 30    // Humedad mínima (%)
#define MAX_HUMIDITY_THRESHOLD 80    // Humedad máxima (%)
#define WATERING_DURATION 30         // Duración riego (segundos)
#define CHECK_INTERVAL 300000        // Intervalo verificación (5 min)
```

### **7.3 Comandos del Sistema Completo**
```
STATUS          # Estado del sistema
WATER_NOW       # Riego manual inmediato
STOP_WATER      # Detener riego
AUTO_ON         # Activar modo automático
AUTO_OFF        # Desactivar modo automático
SET_MIN:25      # Cambiar humedad mínima
SET_MAX:75      # Cambiar humedad máxima
```

## ⚠️ **Solución de Problemas**

### **Problema: Sensor no lee correctamente**
```
❌ Síntomas: Valores constantes o erróneos
🔧 Soluciones:
- Verificar conexiones VCC, GND, A0
- Calibrar sensor con CALIBRATE
- Verificar que el sensor esté bien insertado
- Probar con diferentes tipos de suelo
```

### **Problema: Bomba no funciona**
```
❌ Síntomas: No se activa o no bombea agua
🔧 Soluciones:
- Verificar fuente de alimentación 12V
- Verificar conexiones del relé
- Verificar conexiones de la bomba
- Verificar que el relé esté bien conectado
- Probar relé manualmente
```

### **Problema: Botón no responde**
```
❌ Síntomas: No detecta presiones
🔧 Soluciones:
- Verificar conexión a D2 y GND
- Verificar resistencia pull-up
- Verificar que el botón esté bien conectado
- Probar con multímetro
```

### **Problema: LED no enciende**
```
❌ Síntomas: No se ilumina
🔧 Soluciones:
- Verificar conexión a D13 y GND
- Verificar resistencia 220Ω
- Verificar polaridad del LED
- Probar con multímetro
```

## 📊 **Verificación Final**

### **Checklist de Pruebas**
- [ ] Arduino se programa correctamente
- [ ] Monitor serial funciona
- [ ] Sensor de humedad lee valores
- [ ] Relé se activa/desactiva
- [ ] Bomba funciona con agua
- [ ] LED indica estado
- [ ] Botón manual funciona
- [ ] Prueba automática completa
- [ ] Sistema completo funciona

### **Próximos Pasos**
1. **Agregar energía solar** - Panel y batería
2. **Agregar display LCD** - Para información local
3. **Agregar RTC** - Para programación temporal
4. **Optimizar código** - Ajustar parámetros
5. **Instalar en exterior** - Caja de protección

## 🎉 **¡Felicidades!**

Si has completado todas las pruebas, tu sistema de riego Arduino está funcionando correctamente. Ahora puedes:

- **Instalar en tu jardín** con energía solar
- **Agregar más sensores** para monitoreo avanzado
- **Conectar a WiFi** con ESP32 para control remoto
- **Escalar a múltiples zonas** de riego

¿Necesitas ayuda con algún paso específico o quieres avanzar a la siguiente fase? 