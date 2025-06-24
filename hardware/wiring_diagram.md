# Esquema de Conexiones - Sistema de Riego Solar

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
│                    ARDUINO NANO/ESP32                       │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────────┐                                        │
│  │   Arduino Nano  │                                        │
│  │                 │                                        │
│  │  VIN ───────────┼─── 12V (desde controlador)             │
│  │  GND ───────────┼─── GND                                 │
│  │                 │                                        │
│  │  A0 ────────────┼─── Sensor Humedad                      │
│  │  A1 ────────────┼─── Voltaje Panel Solar                 │
│  │  A2 ────────────┼─── Voltaje Batería                     │
│  │  A3 ────────────┼─── Sensor Luz (opcional)               │
│  │                 │                                        │
│  │  D2 ────────────┼─── Botón Manual                        │
│  │  D7 ────────────┼─── Relé Bomba                          │
│  │  D13 ───────────┼─── LED Estado                          │
│  │                 │                                        │
│  │  A4/SDA ────────┼─── LCD I2C SDA                         │
│  │  A5/SCL ────────┼─── LCD I2C SCL                         │
│  │                 │                                        │
│  └─────────────────┘                                        │
│                                                             │
└─────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────┐
│                    SENSORES Y ACTUADORES                    │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  🌱 Sensor de Humedad del Suelo                            │
│     ├─── VCC ── 3.3V/5V                                    │
│     ├─── GND ── GND                                         │
│     └─── AOUT ── A0                                         │
│                                                             │
│  💧 Módulo Relé 5V                                          │
│     ├─── VCC ── 5V                                          │
│     ├─── GND ── GND                                         │
│     ├─── IN ── D7                                           │
│     ├─── COM ── 12V+                                        │
│     └─── NO ── Bomba +                                      │
│                                                             │
│  🔌 Bomba de Agua 12V                                       │
│     ├─── + ── Relé NO                                       │
│     └─── - ── GND                                           │
│                                                             │
│  📺 Display LCD 16x2 I2C                                    │
│     ├─── VCC ── 5V                                          │
│     ├─── GND ── GND                                         │
│     ├─── SDA ── A4                                          │
│     └─── SCL ── A5                                          │
│                                                             │
│  🔘 Botón Manual                                            │
│     ├─── Pin 1 ── D2                                        │
│     └─── Pin 2 ── GND (con resistencia pull-up)             │
│                                                             │
│  💡 LED de Estado                                           │
│     ├─── + ── D13 (con resistencia 220Ω)                    │
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

#### **Controlador → Arduino**
```
Controlador 12V+ ──→ Arduino VIN
Controlador GND  ──→ Arduino GND
```

### **2. Sensores**

#### **Sensor de Humedad del Suelo**
```
Sensor VCC  ──→ Arduino 3.3V/5V
Sensor GND  ──→ Arduino GND
Sensor AOUT ──→ Arduino A0
```

#### **Divisor de Voltaje para Batería**
```
Batería + ──→ Resistencia 10kΩ ──→ Arduino A2
                    │
              Resistencia 10kΩ ──→ Arduino GND
```

#### **Divisor de Voltaje para Panel Solar**
```
Panel + ──→ Resistencia 10kΩ ──→ Arduino A1
                │
          Resistencia 10kΩ ──→ Arduino GND
```

### **3. Sistema de Riego**

#### **Módulo Relé**
```
Relé VCC ──→ Arduino 5V
Relé GND ──→ Arduino GND
Relé IN   ──→ Arduino D7
```

#### **Bomba de Agua**
```
Relé COM ──→ 12V+
Relé NO   ──→ Bomba +
Bomba -   ──→ GND
```

### **4. Interfaz**

#### **Display LCD I2C**
```
LCD VCC ──→ Arduino 5V
LCD GND ──→ Arduino GND
LCD SDA ──→ Arduino A4
LCD SCL ──→ Arduino A5
```

#### **Botón Manual**
```
Botón Pin 1 ──→ Arduino D2
Botón Pin 2 ──→ Arduino GND
```

#### **LED de Estado**
```
LED + ──→ Resistencia 220Ω ──→ Arduino D13
LED - ──→ Arduino GND
```

## ⚠️ **Consideraciones de Seguridad**

### **Protección Eléctrica**
1. **Fusibles**: Agregar fusible de 1A en la línea de 12V
2. **Diodos**: Usar diodos 1N4007 para protección contra voltaje inverso
3. **Capacitores**: Agregar capacitores de 100nF para filtrado de ruido

### **Protección Mecánica**
1. **Caja IP65**: Todos los componentes electrónicos en caja sellada
2. **Cableado**: Usar cable resistente a la intemperie
3. **Conectores**: Usar conectores impermeables para conexiones externas

### **Protección del Agua**
1. **Filtro**: Agregar filtro de agua antes de la bomba
2. **Válvula de retención**: Evitar retroceso de agua
3. **Drenaje**: Sistema de drenaje para evitar congelación

## 🔍 **Verificación de Conexiones**

### **Antes de Encender**
1. ✅ Verificar polaridad de la batería
2. ✅ Verificar conexiones del panel solar
3. ✅ Verificar que el relé esté en posición OFF
4. ✅ Verificar que no haya cortocircuitos

### **Pruebas Iniciales**
1. **Panel Solar**: Medir voltaje en condiciones de luz
2. **Batería**: Verificar voltaje (debe ser ~12.6V cargada)
3. **Arduino**: Verificar que encienda correctamente
4. **Sensores**: Verificar lecturas en el monitor serial
5. **Relé**: Probar activación manual
6. **Bomba**: Probar funcionamiento con agua

## 📋 **Lista de Verificación**

- [ ] Panel solar instalado y orientado correctamente
- [ ] Controlador de carga configurado
- [ ] Batería conectada y cargada
- [ ] Arduino programado y funcionando
- [ ] Sensores calibrados
- [ ] Sistema de riego probado
- [ ] Caja sellada y protegida
- [ ] Cableado organizado y seguro

## 🛠️ **Herramientas Necesarias**

- **Multímetro** para verificar voltajes
- **Soldador** para conexiones permanentes
- **Cinta aislante** para protección
- **Destornilladores** para ajustes
- **Taladro** para perforar la caja 