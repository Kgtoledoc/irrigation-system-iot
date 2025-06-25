# 🔌 Conexiones Básicas - Prueba Arduino

## 📋 **Diagrama Visual Simple**

```
┌─────────────────────────────────────────────────────────────┐
│                    ARDUINO NANO/UNO                         │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────────┐                                        │
│  │   Arduino Nano  │                                        │
│  │                 │                                        │
│  │  5V ────────────┼─── Sensor Humedad VCC                  │
│  │  GND ───────────┼─── Sensor Humedad GND                  │
│  │  A0 ────────────┼─── Sensor Humedad AOUT                 │
│  │                 │                                        │
│  │  D7 ────────────┼─── Relé IN                             │
│  │  D13 ───────────┼─── LED + (con resistencia 220Ω)        │
│  │  D2 ────────────┼─── Botón (con resistencia pull-up)     │
│  │  GND ───────────┼─── LED -, Botón, Relé GND              │
│  │                 │                                        │
│  └─────────────────┘                                        │
│                                                             │
└─────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────┐
│                    SENSOR DE HUMEDAD                        │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  🌱 Sensor Capacitivo/Resistivo                            │
│     ├─── VCC ── Arduino 5V                                 │
│     ├─── GND ── Arduino GND                                │
│     └─── AOUT ── Arduino A0                                │
│                                                             │
└─────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────┐
│                    MÓDULO RELÉ                              │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  🔌 Relé 5V 1 Canal                                        │
│     ├─── VCC ── Arduino 5V                                 │
│     ├─── GND ── Arduino GND                                │
│     ├─── IN ── Arduino D7                                  │
│     ├─── COM ── 12V+ (fuente de alimentación)              │
│     └─── NO ── Bomba +                                     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────┐
│                    BOMBA DE AGUA                            │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  💧 Bomba 12V                                               │
│     ├─── + ── Relé NO                                      │
│     └─── - ── GND (fuente de alimentación)                 │
│                                                             │
└─────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────┐
│                    INTERFAZ MANUAL                          │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  🔘 Botón Pulsador                                         │
│     ├─── Pin 1 ── Arduino D2                               │
│     └─── Pin 2 ── Arduino GND                              │
│                                                             │
│  💡 LED de Estado                                           │
│     ├─── + ── Resistencia 220Ω ── Arduino D13              │
│     └─── - ── Arduino GND                                  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

## 🔧 **Conexiones Detalladas**

### **1. Sensor de Humedad del Suelo**
```
Sensor VCC  ──→ Arduino 5V
Sensor GND  ──→ Arduino GND
Sensor AOUT ──→ Arduino A0
```

### **2. Módulo Relé**
```
Relé VCC ──→ Arduino 5V
Relé GND ──→ Arduino GND
Relé IN   ──→ Arduino D7
Relé COM  ──→ Fuente 12V+
Relé NO   ──→ Bomba +
```

### **3. Bomba de Agua**
```
Bomba + ──→ Relé NO
Bomba - ──→ Fuente 12V GND
```

### **4. Botón Manual**
```
Botón Pin 1 ──→ Arduino D2
Botón Pin 2 ──→ Arduino GND
```

### **5. LED de Estado**
```
LED + ──→ Resistencia 220Ω ──→ Arduino D13
LED - ──→ Arduino GND
```

## ⚠️ **Puntos Importantes**

### **Polaridad**
- ✅ **Verificar polaridad** de la bomba (+ y -)
- ✅ **Verificar polaridad** del LED (ánodo y cátodo)
- ✅ **Verificar polaridad** de la fuente de alimentación

### **Resistencias**
- 🔧 **LED**: Resistencia 220Ω obligatoria
- 🔧 **Botón**: Resistencia pull-up interna del Arduino (INPUT_PULLUP)

### **Fuente de Alimentación**
- ⚡ **Arduino**: Alimentado por USB (5V)
- ⚡ **Bomba**: Alimentada por fuente externa 12V
- ⚡ **Relé**: Alimentado por Arduino 5V, pero conmuta 12V

## 🔍 **Verificación de Conexiones**

### **Antes de Encender**
1. ✅ Verificar que no haya cortocircuitos
2. ✅ Verificar que el relé esté en posición OFF
3. ✅ Verificar que la bomba esté desconectada del agua
4. ✅ Verificar que todas las conexiones estén firmes

### **Pruebas Iniciales**
1. **Arduino**: Verificar que encienda correctamente
2. **LED**: Debe encender cuando D13 esté en HIGH
3. **Botón**: Debe cambiar estado cuando se presione
4. **Relé**: Debe hacer clic cuando D7 esté en HIGH
5. **Bomba**: Debe funcionar cuando el relé se active

## 🛠️ **Herramientas Necesarias**

- **Multímetro** - Para verificar voltajes y continuidad
- **Protoboard** - Para conexiones temporales
- **Cables Dupont** - Para conectar componentes
- **Soldador** - Para conexiones permanentes (opcional)
- **Cinta aislante** - Para proteger conexiones

## 📊 **Valores de Referencia**

### **Voltajes Esperados**
- **Arduino 5V**: 4.8V - 5.2V
- **Arduino GND**: 0V
- **Fuente 12V**: 11.5V - 12.5V
- **Pin D13 (LED)**: 0V (apagado) / 5V (encendido)
- **Pin D7 (Relé)**: 0V (apagado) / 5V (encendido)

### **Resistencias**
- **LED**: 220Ω (protege el LED)
- **Pull-up**: Interna del Arduino (no necesita resistencia externa)

## 🎯 **Próximos Pasos**

Una vez que las conexiones básicas funcionen:

1. **Subir código de prueba** (`arduino_test_basic.ino`)
2. **Probar cada componente** individualmente
3. **Calibrar el sensor** de humedad
4. **Probar el sistema completo**
5. **Agregar energía solar** (panel y batería)
6. **Agregar display LCD** para información local
7. **Instalar en exterior** con caja de protección 