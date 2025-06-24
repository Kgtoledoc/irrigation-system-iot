# Esquema de Conexiones - Raspberry Pi

## 🔌 **Diagrama de Conexiones**

```
┌─────────────────────────────────────────────────────────────┐
│                    SISTEMA DE ENERGÍA SOLAR                 │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ☀️ Panel Solar 20W 12V                                    │
│     ├─── +12V ─────────────────────────────────────────────┤
│     └─── GND  ─────────────────────────────────────────────┤
│                                                             │
│  🔋 Controlador de Carga 12V                               │
│     ├─── PANEL + ──┐                                       │
│     ├─── PANEL - ──┤                                       │
│     ├─── BAT + ─────┼─── Batería 12V 10Ah                  │
│     └─── BAT - ─────┘                                       │
│                                                             │
│  ⚡ Salida 12V ─────────────────────────────────────────────┤
│                                                             │
└─────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────┐
│                    RASPBERRY PI 4                           │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────────┐                                        │
│  │   Raspberry Pi  │                                        │
│  │                 │                                        │
│  │  5V ────────────┼─── 5V (alimentación)                  │
│  │  GND ───────────┼─── GND                                 │
│  │                 │                                        │
│  │  GPIO18 ────────┼─── Relé Bomba                         │
│  │  GPIO17 ────────┼─── Botón Manual                        │
│  │  GPIO27 ────────┼─── LED Estado                          │
│  │  GPIO23 ────────┼─── Voltaje Panel Solar                 │
│  │  GPIO24 ────────┼─── Voltaje Batería                     │
│  │                 │                                        │
│  │  SDA (GPIO2) ───┼─── I2C SDA (ADS1115, BME280)          │
│  │  SCL (GPIO3) ───┼─── I2C SCL (ADS1115, BME280)          │
│  │                 │                                        │
│  └─────────────────┘                                        │
│                                                             │
└─────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────┐
│                    MÓDULOS I2C                              │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  📊 ADS1115 (ADC de 16 bits)                               │
│     ├─── VDD ── 3.3V                                        │
│     ├─── GND ── GND                                         │
│     ├─── SDA ── GPIO2                                       │
│     ├─── SCL ── GPIO3                                       │
│     ├─── A0 ── Sensor Humedad                               │
│     ├─── A1 ── Voltaje Batería                              │
│     ├─── A2 ── Voltaje Panel Solar                          │
│     └─── A3 ── Sensor Luz                                   │
│                                                             │
│  🌡️ BME280 (Temperatura, Humedad, Presión)                 │
│     ├─── VCC ── 3.3V                                        │
│     ├─── GND ── GND                                         │
│     ├─── SDA ── GPIO2                                       │
│     └─── SCL ── GPIO3                                       │
│                                                             │
└─────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────┐
│                    SENSORES Y ACTUADORES                    │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  🌱 Sensor de Humedad del Suelo                            │
│     ├─── VCC ── 3.3V                                        │
│     ├─── GND ── GND                                         │
│     └─── AOUT ── ADS1115 A0                                 │
│                                                             │
│  💧 Módulo Relé 5V                                          │
│     ├─── VCC ── 5V                                          │
│     ├─── GND ── GND                                         │
│     ├─── IN ── GPIO18                                       │
│     ├─── COM ── 12V+                                        │
│     └─── NO ── Bomba +                                      │
│                                                             │
│  🔌 Bomba de Agua 12V                                       │
│     ├─── + ── Relé NO                                       │
│     └─── - ── GND                                           │
│                                                             │
│  🔘 Botón Manual                                            │
│     ├─── Pin 1 ── GPIO17                                    │
│     └─── Pin 2 ── GND (con resistencia pull-up)             │
│                                                             │
│  💡 LED de Estado                                           │
│     ├─── + ── GPIO27 (con resistencia 220Ω)                 │
│     └─── - ── GND                                           │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

## 🔧 **Conexiones Detalladas**

### **1. Sistema de Energía Solar**

#### **Panel Solar → Controlador de Carga**
```
Panel Solar + ──→ Controlador PANEL +
Panel Solar - ──→ Controlador PANEL -
```

#### **Batería → Controlador de Carga**
```
Batería + ──→ Controlador BAT +
Batería - ──→ Controlador BAT -
```

#### **Controlador → Raspberry Pi**
```
Controlador 12V+ ──→ Convertidor 12V→5V ──→ Raspberry Pi 5V
Controlador GND  ──→ Raspberry Pi GND
```

### **2. Módulos I2C**

#### **ADS1115 (ADC)**
```
ADS1115 VDD ──→ Raspberry Pi 3.3V
ADS1115 GND ──→ Raspberry Pi GND
ADS1115 SDA ──→ Raspberry Pi GPIO2 (SDA)
ADS1115 SCL ──→ Raspberry Pi GPIO3 (SCL)
ADS1115 ADDR ──→ GND (dirección 0x48)
```

#### **BME280 (Sensor Ambiental)**
```
BME280 VCC ──→ Raspberry Pi 3.3V
BME280 GND ──→ Raspberry Pi GND
BME280 SDA ──→ Raspberry Pi GPIO2 (SDA)
BME280 SCL ──→ Raspberry Pi GPIO3 (SCL)
```

### **3. Sensores**

#### **Sensor de Humedad del Suelo**
```
Sensor VCC  ──→ Raspberry Pi 3.3V
Sensor GND  ──→ Raspberry Pi GND
Sensor AOUT ──→ ADS1115 A0
```

#### **Divisor de Voltaje para Batería**
```
Batería + ──→ Resistencia 10kΩ ──→ ADS1115 A1
                    │
              Resistencia 10kΩ ──→ Raspberry Pi GND
```

#### **Divisor de Voltaje para Panel Solar**
```
Panel + ──→ Resistencia 10kΩ ──→ ADS1115 A2
            │
      Resistencia 10kΩ ──→ Raspberry Pi GND
```

#### **Sensor de Luz**
```
Sensor Luz + ──→ Resistencia 10kΩ ──→ ADS1115 A3
Sensor Luz - ──→ Raspberry Pi GND
```

### **4. Sistema de Riego**

#### **Módulo Relé**
```
Relé VCC ──→ Raspberry Pi 5V
Relé GND ──→ Raspberry Pi GND
Relé IN   ──→ Raspberry Pi GPIO18
```

#### **Bomba de Agua**
```
Relé COM ──→ 12V+
Relé NO   ──→ Bomba +
Bomba -   ──→ GND
```

### **5. Interfaz**

#### **Botón Manual**
```
Botón Pin 1 ──→ Raspberry Pi GPIO17
Botón Pin 2 ──→ Raspberry Pi GND
```

#### **LED de Estado**
```
LED + ──→ Resistencia 220Ω ──→ Raspberry Pi GPIO27
LED - ──→ Raspberry Pi GND
```

## 📋 **Lista de Componentes Raspberry Pi**

### **Hardware Principal**
- **Raspberry Pi 4 Model B** (2GB RAM mínimo)
- **Tarjeta microSD 16GB+** (Clase 10)
- **Fuente de alimentación 5V/3A** (para desarrollo)

### **Sensores y ADC**
- **ADS1115** - ADC de 16 bits, 4 canales
- **BME280** - Sensor de temperatura, humedad y presión
- **Sensor de humedad capacitivo** - Para suelo
- **Fotoresistor LDR** - Sensor de luz

### **Sistema de Energía**
- **Panel Solar 20W 12V**
- **Controlador de carga PWM 10A**
- **Batería LiPo 12V 10Ah**
- **Convertidor 12V→5V** (para Raspberry Pi)

### **Sistema de Riego**
- **Módulo relé 1 canal 5V**
- **Bomba de agua 12V 5W**
- **Tubos de silicona 6mm**
- **Conectores de tubo**

### **Componentes Electrónicos**
- **Resistencias 10kΩ** (4 piezas) - Divisores de voltaje
- **Resistencia 220Ω** (1 pieza) - LED
- **Protoboard** - Para conexiones
- **Cables Dupont** - Conectores
- **Caja de protección IP65**

## ⚙️ **Configuración del Sistema**

### **1. Habilitar I2C**
```bash
sudo raspi-config
# Interface Options → I2C → Enable
sudo reboot
```

### **2. Verificar I2C**
```bash
sudo i2cdetect -y 1
# Deberías ver:
# 48 (ADS1115)
# 76 (BME280)
```

### **3. Instalar Dependencias**
```bash
sudo apt-get update
sudo apt-get install python3-pip python3-dev
sudo apt-get install i2c-tools python3-smbus
sudo apt-get install libgpiod-dev
```

### **4. Instalar Paquetes Python**
```bash
pip3 install -r requirements_raspberry_pi.txt
```

### **5. Configurar Permisos**
```bash
sudo usermod -a -G gpio $USER
sudo usermod -a -G i2c $USER
sudo reboot
```

## 🔍 **Verificación de Conexiones**

### **Antes de Encender**
1. ✅ Verificar polaridad de la batería
2. ✅ Verificar conexiones del panel solar
3. ✅ Verificar que el relé esté en posición OFF
4. ✅ Verificar que no haya cortocircuitos
5. ✅ Verificar direcciones I2C

### **Pruebas Iniciales**
1. **Panel Solar**: Medir voltaje en condiciones de luz
2. **Batería**: Verificar voltaje (debe ser ~12.6V cargada)
3. **Raspberry Pi**: Verificar que encienda correctamente
4. **I2C**: Verificar dispositivos conectados
5. **Sensores**: Verificar lecturas en el dashboard
6. **Relé**: Probar activación manual
7. **Bomba**: Probar funcionamiento con agua

## 🛠️ **Herramientas Necesarias**

- **Multímetro** para verificar voltajes
- **Soldador** para conexiones permanentes
- **Cinta aislante** para protección
- **Destornilladores** para ajustes
- **Taladro** para perforar la caja
- **Cable de red** para configuración inicial

## 📊 **Ventajas del Raspberry Pi**

### **vs Arduino**
- ✅ **Más potente** - Procesador ARM de 1.5GHz
- ✅ **Más memoria** - 2-8GB RAM vs 2KB
- ✅ **Sistema operativo** - Linux completo
- ✅ **Conectividad** - WiFi, Ethernet, Bluetooth
- ✅ **Almacenamiento** - Tarjeta SD de varios GB
- ✅ **Multitarea** - Múltiples procesos simultáneos

### **vs ESP32**
- ✅ **Más potente** - Mejor rendimiento
- ✅ **Sistema completo** - Base de datos, web server
- ✅ **Más puertos** - Más GPIO disponibles
- ✅ **Mejor ADC** - ADS1115 de 16 bits
- ✅ **Desarrollo fácil** - Python estándar
- ✅ **Expansibilidad** - Múltiples módulos

## 💰 **Costo Estimado**

### **Sistema Básico ($150-250)**
- Raspberry Pi 4 (2GB): $45
- Sensores y ADC: $25
- Sistema solar: $80
- Componentes varios: $50

### **Sistema Avanzado ($250-400)**
- Raspberry Pi 4 (4GB): $65
- Sensores múltiples: $40
- Sistema solar mejorado: $120
- Caja y accesorios: $75

## 🎯 **Casos de Uso Ideales**

### **Jardín Inteligente**
- Múltiples zonas de riego
- Análisis de datos avanzado
- Integración con otros sistemas
- Dashboard web completo

### **Invernadero Profesional**
- Control de clima
- Registro de datos
- Alertas y notificaciones
- Integración con IoT

### **Investigación Agrícola**
- Análisis de patrones
- Optimización de riego
- Predicción de necesidades
- Reportes automáticos 