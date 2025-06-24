# Lista de Componentes - Sistema de Riego Solar con Arduino

## 🔋 **Sistema de Energía Solar**

### **Panel Solar**
- **Panel Solar 20W 12V** - Panel monocristalino de 20W
  - Voltaje: 12V nominal
  - Corriente: ~1.67A en condiciones ideales
  - Dimensiones: ~50x30cm
  - Precio estimado: $25-40

### **Controlador de Carga Solar**
- **Controlador de carga PWM 10A** o **MPPT 10A**
  - Entrada: 12V/24V
  - Salida: 12V
  - Protección: Sobrecarga, descarga profunda, cortocircuito
  - Precio estimado: $15-30

### **Batería**
- **Batería LiPo 12V 10Ah** o **Batería AGM 12V 7Ah**
  - Voltaje: 12V
  - Capacidad: 7-10Ah
  - Ciclos de vida: 500-1000
  - Precio estimado: $40-80

## 🧠 **Controlador Principal**

### **Arduino**
- **Arduino Nano** (recomendado) o **Arduino Uno**
  - Microcontrolador: ATmega328P
  - Voltaje operativo: 5V
  - Entradas analógicas: 8 (Nano) / 6 (Uno)
  - Precio estimado: $5-15

### **Alternativa: ESP32**
- **ESP32 DevKit** (con WiFi integrado)
  - Microcontrolador: ESP32
  - WiFi y Bluetooth integrados
  - Más entradas analógicas
  - Precio estimado: $8-15

## 🌱 **Sensores**

### **Sensor de Humedad del Suelo**
- **Sensor capacitivo de humedad** (recomendado)
  - Voltaje: 3.3V-5V
  - Salida: Analógica 0-3.3V
  - Rango: 0-100% humedad
  - Precio estimado: $3-8

### **Sensor de Luz** (opcional)
- **Fotoresistor LDR** o **Sensor de luz BH1750**
  - Rango: 0-65535 lux
  - Comunicación: I2C
  - Precio estimado: $2-5

### **Sensor de Temperatura** (opcional)
- **DS18B20** o **DHT22**
  - Rango: -40°C a +80°C
  - Precisión: ±0.5°C
  - Precio estimado: $3-8

## 💧 **Sistema de Riego**

### **Bomba de Agua**
- **Bomba sumergible 12V 3-5W**
  - Voltaje: 12V DC
  - Potencia: 3-5W
  - Caudal: 240-360 L/h
  - Precio estimado: $8-15

### **Módulo Relé**
- **Módulo relé 1 canal 5V**
  - Voltaje de control: 5V
  - Voltaje de conmutación: 250V AC / 30V DC
  - Corriente: 10A
  - Precio estimado: $2-5

### **Tubos y Conectores**
- **Tubo de silicona 6mm** (1 metro)
- **Conectores de tubo** (4-6 piezas)
- **Filtro de agua** (opcional)
- Precio estimado: $5-10

## 📺 **Interfaz y Display**

### **Display LCD**
- **LCD 16x2 con I2C**
  - Caracteres: 16x2
  - Comunicación: I2C
  - Voltaje: 5V
  - Precio estimado: $3-8

### **Botón Manual**
- **Botón pulsador 12mm**
  - Voltaje: 5V
  - Precio estimado: $1-2

### **LED de Estado**
- **LED RGB** o **LED simple**
  - Voltaje: 3.3V-5V
  - Precio estimado: $1-3

## ⏰ **Reloj en Tiempo Real** (opcional)

### **Módulo RTC**
- **DS3231** (alta precisión)
  - Precisión: ±2ppm
  - Batería de respaldo incluida
  - Comunicación: I2C
  - Precio estimado: $3-8

## 🔌 **Conectividad** (opcional)

### **Módulo WiFi**
- **ESP8266 ESP-01** (si usas Arduino)
  - WiFi 802.11 b/g/n
  - Voltaje: 3.3V
  - Precio estimado: $3-8

## 🛠️ **Componentes Electrónicos**

### **Resistencias**
- **Resistencia 10kΩ** (2 piezas) - Para divisores de voltaje
- **Resistencia 220Ω** (1 pieza) - Para LED
- Precio estimado: $1-2

### **Diodos**
- **Diodo 1N4007** (2 piezas) - Protección
- Precio estimado: $1-2

### **Capacitores**
- **Capacitor 100nF** (2 piezas) - Filtrado
- Precio estimado: $1-2

## 📦 **Caja y Protección**

### **Caja de Protección**
- **Caja IP65** o **Caja de plástico**
  - Dimensiones: ~15x10x8cm
  - Protección: IP65 (resistente al agua)
  - Precio estimado: $10-20

### **Cableado**
- **Cable multiconductor** (2 metros)
- **Cable de alimentación** (1 metro)
- **Conectores Dupont** (20 piezas)
- Precio estimado: $5-10

## 💰 **Costo Total Estimado**

### **Versión Básica** (sin WiFi, sin RTC)
- **Total: $80-150 USD**

### **Versión Completa** (con WiFi, RTC, sensores adicionales)
- **Total: $120-200 USD**

## 🎯 **Recomendaciones por Presupuesto**

### **Presupuesto Bajo ($50-80)**
- Arduino Nano
- Sensor de humedad básico
- Bomba pequeña 12V
- Batería 7Ah
- Panel solar 10W
- Sin display, sin WiFi

### **Presupuesto Medio ($80-150)**
- ESP32 (con WiFi integrado)
- Sensor capacitivo de humedad
- Display LCD
- Bomba 12V 5W
- Batería 10Ah
- Panel solar 20W
- Controlador de carga

### **Presupuesto Alto ($150+)**
- ESP32
- Múltiples sensores
- Display LCD
- RTC DS3231
- Bomba de mayor potencia
- Batería de mayor capacidad
- Panel solar 30W+
- Caja de protección IP65
- Tubos y accesorios premium

## 🔧 **Herramientas Necesarias**

- **Soldador** y **estaño**
- **Multímetro**
- **Destornilladores**
- **Taladro** (para perforar la caja)
- **Cinta aislante**
- **Cable de programación USB**

## 📋 **Consideraciones Importantes**

1. **Ubicación del panel solar**: Debe recibir al menos 4-6 horas de sol directo
2. **Protección contra agua**: Todos los componentes deben estar protegidos
3. **Calibración del sensor**: Requiere ajuste según el tipo de suelo
4. **Mantenimiento**: Limpieza periódica del sensor y filtros
5. **Backup**: Considerar fuente de energía alternativa para emergencias 