# 🌱 Sistema de Riego Inteligente - Documentación Completa

## 📋 Tabla de Contenidos
- [Visión General](#visión-general)
- [Arquitectura del Sistema](#arquitectura-del-sistema)
- [Componentes Hardware](#componentes-hardware)
- [Software y Código](#software-y-código)
- [Instalación y Configuración](#instalación-y-configuración)
- [Guías de Prueba](#guías-de-prueba)
- [Mantenimiento](#mantenimiento)
- [Solución de Problemas](#solución-de-problemas)
- [Recursos Adicionales](#recursos-adicionales)

---

## 🎯 Visión General

### ¿Qué es el Sistema de Riego Inteligente?
Un sistema automatizado que monitorea la humedad del suelo y riega las plantas de manera inteligente, optimizando el uso del agua y manteniendo las plantas saludables.

### Características Principales
| Característica | Descripción |
|---|---|
| 🌱 **Monitoreo Automático** | Sensor de humedad del suelo en tiempo real |
| 💧 **Riego Inteligente** | Activación automática basada en umbrales de humedad |
| ☀️ **Energía Solar** | Funcionamiento autónomo con panel solar |
| 📱 **Control Remoto** | Interfaz web para monitoreo y control |
| 🔋 **Batería Integrada** | Funcionamiento 24/7 sin interrupciones |
| 📊 **Registro de Datos** | Historial de riego y lecturas de sensores |

### Beneficios
- ✅ **Ahorro de agua** hasta 70%
- ✅ **Plantas más saludables** con riego optimizado
- ✅ **Funcionamiento autónomo** sin intervención manual
- ✅ **Monitoreo remoto** desde cualquier dispositivo
- ✅ **Escalable** para múltiples zonas de riego

---

## 🏗️ Arquitectura del Sistema

### Diagrama de Arquitectura
```
┌─────────────────────────────────────────────────────────────┐
│                    SISTEMA DE RIEGO INTELIGENTE            │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐     │
│  │   SENSORES  │    │  CONTROLADOR │    │   ACTUADORES │     │
│  │             │    │             │    │             │     │
│  │ 🌱 Humedad  │───▶│  Arduino/   │───▶│ 💧 Bomba    │     │
│  │ 🌡️ Temperatura│   │  ESP32/RPi  │    │ 🔌 Relé     │     │
│  │ ☀️ Luz Solar │    │             │    │ 💡 LED      │     │
│  └─────────────┘    └─────────────┘    └─────────────┘     │
│         │                   │                   │           │
│         └───────────────────┼───────────────────┘           │
│                             │                               │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐     │
│  │   ENERGÍA   │    │  INTERFAZ   │    │  ALMACENAM. │     │
│  │             │    │             │    │             │     │
│  │ ☀️ Panel    │    │ 📱 Web App  │    │ 💾 Base de  │     │
│  │ 🔋 Batería  │    │ 🔘 Botón    │    │    Datos    │     │
│  │ ⚡ Controlador│   │ 📺 Display   │    │ 📊 Logs     │     │
│  └─────────────┘    └─────────────┘    └─────────────┘     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### Componentes del Sistema

#### 1. **Sensores**
- **Sensor de Humedad del Suelo**: Monitorea la humedad del suelo
- **Sensor de Temperatura**: Mide la temperatura ambiente
- **Sensor de Luz**: Detecta la intensidad de luz solar

#### 2. **Controlador**
- **Arduino Nano/ESP32/Raspberry Pi**: Procesa datos y controla actuadores
- **Lógica de Control**: Algoritmos de riego inteligente

#### 3. **Actuadores**
- **Bomba de Agua**: Suministra agua a las plantas
- **Relé**: Controla la activación de la bomba
- **LED de Estado**: Indica el estado del sistema

#### 4. **Energía**
- **Panel Solar**: Genera energía eléctrica
- **Batería**: Almacena energía para uso nocturno
- **Controlador de Carga**: Gestiona la carga de la batería

#### 5. **Interfaz**
- **Aplicación Web**: Control y monitoreo remoto
- **Botón Manual**: Control local de emergencia
- **Display LCD**: Información local del sistema

---

## 🔧 Componentes Hardware

### Lista de Componentes Básicos

| Componente | Cantidad | Descripción | Precio Aprox. |
|---|---|---|---|
| **Arduino Nano** | 1 | Microcontrolador principal | $5-10 |
| **Sensor Humedad Suelo** | 1 | Capacitivo o resistivo | $3-8 |
| **Módulo Relé 5V** | 1 | Control de bomba | $2-5 |
| **Bomba de Agua 12V** | 1 | Bomba sumergible pequeña | $8-15 |
| **Panel Solar 20W** | 1 | Generación de energía | $25-40 |
| **Batería 12V 7Ah** | 1 | Almacenamiento de energía | $20-35 |
| **Controlador de Carga** | 1 | Gestión de batería | $8-15 |
| **Display LCD 16x2** | 1 | Información local | $5-10 |
| **LED + Resistencia** | 1 | Indicador de estado | $1-2 |
| **Botón Pulsador** | 1 | Control manual | $1-2 |
| **Cables y Conectores** | - | Conexiones | $5-10 |
| **Caja de Protección** | 1 | Protección exterior | $10-20 |

**Total Estimado: $90-170**

### Componentes Opcionales

| Componente | Descripción | Beneficio |
|---|---|---|
| **Módulo RTC DS3231** | Reloj en tiempo real | Programación temporal |
| **Sensor DHT22** | Temperatura y humedad | Monitoreo ambiental |
| **Módulo WiFi ESP8266** | Conectividad WiFi | Control remoto |
| **Múltiples Sensores** | Sensores adicionales | Monitoreo de múltiples zonas |
| **Válvulas Solenoides** | Control de múltiples zonas | Sistema escalable |

### Especificaciones Técnicas

#### Arduino Nano
- **Microcontrolador**: ATmega328P
- **Voltaje Operativo**: 5V
- **Pines Digitales**: 14
- **Pines Analógicos**: 8
- **Memoria Flash**: 32KB
- **SRAM**: 2KB

#### Sensor de Humedad del Suelo
- **Tipo**: Capacitivo (recomendado) o Resistivo
- **Voltaje Operativo**: 3.3V-5V
- **Rango de Medición**: 0-100%
- **Precisión**: ±2%
- **Tiempo de Respuesta**: <1 segundo

#### Bomba de Agua
- **Voltaje**: 12V DC
- **Corriente**: 0.5-2A
- **Caudal**: 240-360L/h
- **Presión**: 1-2 bar
- **Material**: Plástico resistente

---

## 💻 Software y Código

### Estructura del Proyecto
```
sistema-riego/
├── backend/                 # Servidor FastAPI
│   ├── app/
│   │   ├── models/         # Modelos de datos
│   │   ├── services/       # Lógica de negocio
│   │   └── main.py         # Aplicación principal
│   ├── requirements.txt    # Dependencias Python
│   └── .env.example        # Configuración de ejemplo
├── hardware/               # Código Arduino/ESP32
│   ├── arduino_irrigation.ino
│   ├── esp32_irrigation.ino
│   └── raspberry_pi_irrigation.py
├── docs/                   # Documentación
│   ├── wiring_diagrams.md
│   ├── installation_guide.md
│   └── troubleshooting.md
└── README.md              # Documentación principal
```

### Tecnologías Utilizadas

#### Backend (FastAPI)
- **Framework**: FastAPI
- **Base de Datos**: SQLite/PostgreSQL
- **Autenticación**: JWT
- **Documentación**: Swagger/OpenAPI
- **WebSockets**: SocketIO para tiempo real

#### Hardware (Arduino/ESP32)
- **Lenguaje**: C++
- **Librerías**: 
  - LiquidCrystal_I2C (display)
  - RTClib (reloj)
  - WiFi (ESP32)
  - WebServer (ESP32)

#### Frontend (Web)
- **HTML/CSS/JavaScript**
- **Bootstrap** para diseño responsive
- **Chart.js** para gráficos
- **Socket.IO** para actualizaciones en tiempo real

### Funcionalidades del Software

#### 1. **Monitoreo Automático**
```cpp
// Lectura de sensores cada 5 minutos
void readSensors() {
  soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  temperature = dht.readTemperature();
  lightLevel = analogRead(LIGHT_SENSOR_PIN);
}
```

#### 2. **Lógica de Riego Inteligente**
```cpp
// Algoritmo de decisión de riego
if (soilMoisture < MIN_HUMIDITY && 
    !isWatering && 
    isDayTime()) {
  startWatering();
}
```

#### 3. **Control Remoto**
```python
# API endpoint para control manual
@app.post("/irrigation/start")
async def start_irrigation():
    # Lógica para iniciar riego
    pass
```

#### 4. **Registro de Datos**
```python
# Modelo de datos para sensores
class SensorReading(BaseModel):
    timestamp: datetime
    soil_moisture: float
    temperature: float
    light_level: float
```

---

## 🚀 Instalación y Configuración

### Paso 1: Preparación del Hardware

#### 1.1 Conexiones Básicas
```
Arduino Nano:
├── 5V ──→ Sensor Humedad VCC
├── GND ──→ Sensor Humedad GND
├── A0 ──→ Sensor Humedad AOUT
├── D7 ──→ Relé IN
├── D13 ──→ LED + (con resistencia 220Ω)
├── D2 ──→ Botón (con resistencia pull-up)
└── GND ──→ LED -, Botón, Relé GND
```

#### 1.2 Conexiones de Energía Solar
```
Panel Solar ──→ Controlador de Carga ──→ Batería 12V
                                    └──→ Arduino (via regulador 5V)
                                    └──→ Bomba 12V
```

### Paso 2: Configuración del Software

#### 2.1 Instalación de Arduino IDE
1. Descargar Arduino IDE desde [arduino.cc](https://www.arduino.cc/en/software)
2. Instalar y abrir Arduino IDE
3. Configurar placa: `Herramientas → Placa → Arduino Nano`
4. Configurar puerto: `Herramientas → Puerto → COM3`

#### 2.2 Instalación de Librerías
```
Herramientas → Administrar Bibliotecas
Buscar e instalar:
- LiquidCrystal_I2C
- RTClib
- DHT sensor library
```

#### 2.3 Configuración del Backend
```bash
# Clonar repositorio
git clone https://github.com/tu-usuario/sistema-riego.git
cd sistema-riego/backend

# Crear entorno virtual
python -m venv venv
source venv/bin/activate  # Linux/Mac
# o
venv\Scripts\activate     # Windows

# Instalar dependencias
pip install -r requirements.txt

# Configurar variables de entorno
cp .env.example .env
# Editar .env con tus configuraciones

# Ejecutar servidor
uvicorn app.main:app --reload
```

### Paso 3: Configuración Inicial

#### 3.1 Calibración del Sensor
```cpp
// Ejecutar comando CALIBRATE en monitor serial
// Seguir instrucciones para calibrar sensor
```

#### 3.2 Configuración de Parámetros
```cpp
// Ajustar estos valores según tus necesidades
#define MIN_HUMIDITY_THRESHOLD 30    // Humedad mínima (%)
#define MAX_HUMIDITY_THRESHOLD 80    // Humedad máxima (%)
#define WATERING_DURATION 30         // Duración riego (segundos)
#define CHECK_INTERVAL 300000        // Intervalo verificación (5 min)
```

#### 3.3 Configuración de Red (ESP32)
```cpp
// Configurar credenciales WiFi
const char* ssid = "TU_WIFI_SSID";
const char* password = "TU_WIFI_PASSWORD";
```

---

## 🧪 Guías de Prueba

### Prueba Básica del Sistema

#### 1. **Prueba de Componentes Individuales**
| Componente | Comando | Resultado Esperado |
|---|---|---|
| **Sensor Humedad** | `READ` | Lectura de 0-100% |
| **Relé/Bomba** | `PUMP_ON` | Activación de bomba |
| **LED Estado** | `TEST` | Parpadeo del LED |
| **Botón Manual** | Presionar botón | Cambio de estado |

#### 2. **Prueba Automática Completa**
```bash
# Comando: TEST
# El sistema realizará:
1️⃣ Leyendo sensor de humedad...
2️⃣ Probando LED de estado...
3️⃣ Probando bomba (3 segundos)...
4️⃣ Estado final del sistema
```

#### 3. **Calibración del Sensor**
```bash
# Comando: CALIBRATE
# Proceso:
1. Sensor en aire (seco) - Anotar valor
2. Sensor en agua (húmedo) - Anotar valor
3. Modificar función map() con valores obtenidos
```

### Pruebas Avanzadas

#### 1. **Prueba de Energía Solar**
- Verificar carga de batería
- Probar funcionamiento nocturno
- Monitorear eficiencia del panel

#### 2. **Prueba de Conectividad**
- Verificar conexión WiFi (ESP32)
- Probar acceso web remoto
- Verificar sincronización de datos

#### 3. **Prueba de Resistencia**
- Funcionamiento en diferentes condiciones climáticas
- Durabilidad de componentes
- Estabilidad del sistema

---

## 🔧 Mantenimiento

### Mantenimiento Preventivo

#### Mensual
- [ ] **Limpiar sensores** de humedad
- [ ] **Verificar conexiones** eléctricas
- [ ] **Limpiar panel solar** de polvo
- [ ] **Revisar nivel de agua** en depósito

#### Trimestral
- [ ] **Calibrar sensores** de humedad
- [ ] **Verificar estado de batería**
- [ ] **Limpiar filtros** de agua
- [ ] **Actualizar software** si es necesario

#### Semestral
- [ ] **Revisar caja de protección**
- [ ] **Verificar sellado** contra humedad
- [ ] **Probar sistema de respaldo**
- [ ] **Revisar documentación** y logs

### Mantenimiento Correctivo

#### Problemas Comunes
| Problema | Causa Probable | Solución |
|---|---|---|
| **Bomba no funciona** | Relé defectuoso | Reemplazar módulo relé |
| **Sensor no lee** | Conexiones sueltas | Verificar y apretar conexiones |
| **Batería se agota rápido** | Panel solar sucio | Limpiar panel solar |
| **Sistema no responde** | Arduino bloqueado | Reiniciar Arduino |

#### Herramientas Necesarias
- **Multímetro** - Para verificar voltajes
- **Destornillador** - Para ajustes mecánicos
- **Cepillo suave** - Para limpieza de sensores
- **Cinta aislante** - Para reparaciones temporales

---

## 🛠️ Solución de Problemas

### Problemas de Hardware

#### 1. **Sensor de Humedad no Funciona**
```
❌ Síntomas:
- Lecturas constantes o erróneas
- Valores fuera de rango

🔧 Soluciones:
1. Verificar conexiones VCC, GND, A0
2. Calibrar sensor con comando CALIBRATE
3. Verificar que sensor esté bien insertado
4. Probar con diferentes tipos de suelo
5. Reemplazar sensor si es necesario
```

#### 2. **Bomba de Agua no Activa**
```
❌ Síntomas:
- Relé no hace clic
- Bomba no bombea agua
- Sin respuesta al comando PUMP_ON

🔧 Soluciones:
1. Verificar fuente de alimentación 12V
2. Verificar conexiones del relé
3. Verificar conexiones de la bomba
4. Probar relé manualmente
5. Verificar que bomba no esté obstruida
```

#### 3. **Sistema no Enciende**
```
❌ Síntomas:
- Arduino no se enciende
- Sin indicadores LED
- No hay comunicación serial

🔧 Soluciones:
1. Verificar alimentación USB/5V
2. Verificar cable USB
3. Verificar que no haya cortocircuitos
4. Probar con fuente de alimentación diferente
5. Verificar placa Arduino
```

### Problemas de Software

#### 1. **Código no se Sube**
```
❌ Síntomas:
- Error de compilación
- Error de subida
- Placa no reconocida

🔧 Soluciones:
1. Verificar selección de placa correcta
2. Verificar puerto COM correcto
3. Instalar drivers necesarios
4. Verificar librerías instaladas
5. Reiniciar Arduino IDE
```

#### 2. **Comunicación Serial no Funciona**
```
❌ Síntomas:
- Monitor serial vacío
- Caracteres extraños
- Sin respuesta a comandos

🔧 Soluciones:
1. Verificar velocidad 9600 baudios
2. Verificar puerto COM correcto
3. Reiniciar monitor serial
4. Verificar que código esté ejecutándose
5. Probar con otro cable USB
```

#### 3. **Sistema no Responde**
```
❌ Síntomas:
- Arduino bloqueado
- Sin respuesta a comandos
- LED no cambia de estado

🔧 Soluciones:
1. Reiniciar Arduino (botón reset)
2. Verificar que no haya bucles infinitos
3. Verificar memoria disponible
4. Simplificar código para debug
5. Probar con código básico de prueba
```

### Problemas de Energía Solar

#### 1. **Batería se Agota Rápidamente**
```
❌ Síntomas:
- Sistema se apaga por la noche
- Batería no mantiene carga
- Panel solar no carga batería

🔧 Soluciones:
1. Verificar conexiones del panel solar
2. Limpiar panel solar de polvo
3. Verificar controlador de carga
4. Verificar estado de la batería
5. Considerar batería de mayor capacidad
```

#### 2. **Panel Solar no Genera Energía**
```
❌ Síntomas:
- Batería no se carga
- Voltaje del panel bajo
- Sin indicadores de carga

🔧 Soluciones:
1. Verificar orientación del panel
2. Limpiar panel solar
3. Verificar conexiones
4. Verificar sombras en el panel
5. Probar panel con multímetro
```

---

## 📚 Recursos Adicionales

### Enlaces Útiles
| Recurso | Descripción | Enlace |
|---|---|---|
| **Arduino IDE** | Software de programación | [arduino.cc](https://www.arduino.cc/en/software) |
| **Documentación Arduino** | Referencia de comandos | [arduino.cc/reference](https://www.arduino.cc/reference) |
| **Librerías Arduino** | Librerías adicionales | [arduino.cc/libraries](https://www.arduino.cc/libraries) |
| **Foro Arduino** | Comunidad y soporte | [forum.arduino.cc](https://forum.arduino.cc) |

### Videos Tutoriales
- **Configuración Básica Arduino**: [Link al video]
- **Conexiones de Sensores**: [Link al video]
- **Programación de Riego**: [Link al video]
- **Instalación Solar**: [Link al video]

### Comunidad y Soporte
- **Grupo de Facebook**: Sistema de Riego Inteligente
- **Canal de YouTube**: Tutoriales y actualizaciones
- **GitHub**: Código fuente y issues
- **Discord**: Chat en tiempo real

### Mejoras Futuras
- [ ] **Integración con Alexa/Google Home**
- [ ] **Aplicación móvil nativa**
- [ ] **Machine Learning para optimización**
- [ ] **Sistema multi-zona avanzado**
- [ ] **Integración con estación meteorológica**
- [ ] **Análisis predictivo de riego**

---

## 📞 Contacto y Soporte

### Información de Contacto
- **Email**: soporte@sistema-riego.com
- **WhatsApp**: +34 600 000 000
- **Horario**: Lunes a Viernes 9:00-18:00

### Formulario de Soporte
Para reportar problemas o solicitar ayuda:
1. Describir el problema detalladamente
2. Incluir fotos del hardware si es necesario
3. Adjuntar logs del sistema
4. Especificar versión del software

### FAQ (Preguntas Frecuentes)

#### ¿Cuánto tiempo dura la batería?
La duración depende del uso, pero típicamente 2-3 días sin sol.

#### ¿Puedo usar el sistema en interiores?
Sí, pero necesitarás una fuente de alimentación alternativa.

#### ¿Qué tipo de plantas son compatibles?
El sistema funciona con cualquier tipo de planta, solo ajusta los umbrales de humedad.

#### ¿Puedo controlar múltiples zonas?
Sí, puedes escalar el sistema agregando más sensores y válvulas.

---

*Documentación actualizada: [Fecha]*
*Versión del software: 1.0.0*
*Autor: Tu Nombre* 