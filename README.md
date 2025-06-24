# Sistema de Riego Inteligente 🌱💧

Un sistema de riego automatizado para plantas que monitorea la humedad del suelo y controla el riego de manera inteligente, con soporte para energía solar y control remoto.

## 🌟 **Características Principales**

### **Sistema Backend (Python/FastAPI)**
- 🌡️ **Monitoreo de humedad del suelo** en tiempo real
- 💧 **Control automático de riego** basado en umbrales configurables
- 📱 **Interfaz web** para monitoreo y control remoto
- 📊 **Dashboard** con gráficos y estadísticas
- 🔔 **Sistema de alertas** por email/notificaciones
- 📝 **Historial completo** de riego y lecturas
- ⚙️ **Configuración flexible** de horarios y umbrales
- 🔋 **Modo de ahorro de energía** con sensores de bajo consumo

### **Sistema Hardware (Arduino/ESP32/Raspberry Pi)**
- ☀️ **Energía solar** con panel y batería
- 🔌 **Control independiente** sin necesidad de computadora
- 📺 **Display LCD** para información local
- 🔘 **Control manual** con botón físico
- 📶 **WiFi integrado** para control remoto
- ⏰ **Reloj en tiempo real** para programación
- 💾 **Almacenamiento local** de configuración

## 🛠️ **Opciones de Implementación**

### **Opción 1: Sistema Completo (Backend + Hardware)**
- **Backend Python** para análisis avanzado y API
- **Arduino/ESP32/Raspberry Pi** para control del hardware
- **Interfaz web** completa
- **Base de datos** para historial
- **Ideal para**: Proyectos profesionales, múltiples zonas

### **Opción 2: Sistema Independiente (Solo Hardware)**
- **Arduino/ESP32/Raspberry Pi** con control autónomo
- **Energía solar** para funcionamiento 24/7
- **Display local** para monitoreo
- **Ideal para**: Proyectos domésticos, zonas remotas

## 🧠 **Controladores Soportados**

### **Arduino Nano/Uno** ⚡
- **Microcontrolador**: ATmega328P
- **Voltaje**: 5V
- **Memoria**: 32KB Flash, 2KB RAM
- **Entradas Analógicas**: 8 (Nano) / 6 (Uno)
- **Ventajas**: Económico, estable, amplia compatibilidad
- **Ideal para**: Proyectos básicos, aprendizaje
- **Costo**: $5-15 USD

### **ESP32** 📶
- **Microcontrolador**: ESP32
- **Voltaje**: 3.3V
- **Memoria**: 4MB Flash, 520KB RAM
- **WiFi/Bluetooth**: Integrado
- **Entradas Analógicas**: 18
- **Ventajas**: WiFi integrado, más potente, web server
- **Ideal para**: Proyectos avanzados, IoT
- **Costo**: $8-15 USD

### **Raspberry Pi 4** 🖥️
- **Procesador**: ARM Cortex-A72 1.5GHz
- **Voltaje**: 5V
- **Memoria**: 2-8GB RAM
- **Sistema Operativo**: Linux completo
- **Conectividad**: WiFi, Ethernet, Bluetooth
- **Ventajas**: Máxima potencia, sistema completo, base de datos
- **Ideal para**: Proyectos profesionales, análisis avanzado
- **Costo**: $45-65 USD

## 🔋 **Sistema de Energía Solar**

### **Componentes Solares**
- **Panel Solar 20W 12V** - Panel monocristalino
- **Controlador de Carga 10A** - Protección y gestión
- **Batería LiPo 12V 10Ah** - Almacenamiento de energía
- **Cableado Solar** - Conectores impermeables

### **Ventajas de la Energía Solar**
- ✅ **Autonomía total** - Funciona sin electricidad
- ✅ **Ecológico** - Energía renovable
- ✅ **Económico** - Sin facturas de electricidad
- ✅ **Ubicación flexible** - Instalación en cualquier lugar
- ✅ **Bajo mantenimiento** - Sistema confiable

## 📋 **Comparación de Controladores**

| Característica | Arduino | ESP32 | Raspberry Pi |
|----------------|---------|-------|--------------|
| **Potencia** | Básica | Media | Alta |
| **Memoria** | 2KB RAM | 520KB RAM | 2-8GB RAM |
| **WiFi** | Módulo externo | Integrado | Integrado |
| **Sistema Web** | Limitado | Básico | Completo |
| **Base de Datos** | No | Limitado | SQLite/MySQL |
| **Multitarea** | No | Limitado | Completo |
| **Desarrollo** | C++ | C++ | Python |
| **Costo** | $5-15 | $8-15 | $45-65 |
| **Consumo** | Muy bajo | Bajo | Medio |
| **Complejidad** | Baja | Media | Alta |

## 🚀 **Instalación Rápida**

### **1. Arduino/ESP32**
```bash
# Instalar librerías Arduino
# Ver hardware/arduino_libraries.txt

# Subir código a Arduino/ESP32
# Ver hardware/arduino_irrigation.ino
# o hardware/esp32_irrigation.ino

# Conectar componentes según esquema
# Ver hardware/wiring_diagram.md
```

### **2. Raspberry Pi**
```bash
# Descargar script de instalación
wget https://raw.githubusercontent.com/tu-usuario/sistema-riego/main/hardware/install_raspberry_pi.sh

# Ejecutar instalador
chmod +x install_raspberry_pi.sh
./install_raspberry_pi.sh

# El script configurará automáticamente:
# - Dependencias del sistema
# - I2C y GPIO
# - Entorno Python
# - Servicio systemd
# - Configuración WiFi
```

### **3. Sistema Backend**
```bash
# Clonar repositorio
git clone <tu-repositorio>
cd sistema-riego

# Instalar dependencias
pip install -r requirements.txt

# Configurar variables de entorno
cp env.example .env
# Editar .env con tus configuraciones

# Ejecutar sistema
python main.py
```

## 🔧 **Configuración del Hardware**

### **Conexiones Principales**
```
Panel Solar → Controlador de Carga → Batería
                                    ↓
Controlador ← 12V ← Controlador de Carga
    ↓
Sensor Humedad → ADC
Relé → GPIO → Bomba de Agua
Display LCD → I2C/GPIO
```

### **Configuración de Sensores**
- **Humedad mínima**: 30% (ajustable)
- **Humedad máxima**: 80% (ajustable)
- **Duración de riego**: 30 segundos (ajustable)
- **Intervalo de verificación**: 5 minutos

## 📱 **Interfaz de Control**

### **Web (ESP32/Raspberry Pi)**
- **URL**: http://[IP_DEVICE]
- **Funciones**: Control manual, configuración, monitoreo
- **Responsive**: Funciona en móviles y tablets

### **Serial (Arduino)**
- **Comandos**: WATER_NOW, STOP_WATER, SET_MIN:30, etc.
- **Monitoreo**: Datos en tiempo real
- **Configuración**: Cambios por comandos

### **Display LCD**
- **Información**: Humedad, batería, estado del riego
- **Navegación**: Botón manual para control
- **Indicadores**: LED de estado

## 💰 **Costos Estimados por Opción**

### **Opción Arduino ($80-150)**
- Arduino Nano: $5-15
- Sensores básicos: $15-25
- Sistema solar: $40-80
- Componentes varios: $20-30

### **Opción ESP32 ($120-200)**
- ESP32: $8-15
- Sensores avanzados: $25-40
- Sistema solar: $50-100
- Componentes varios: $37-45

### **Opción Raspberry Pi ($150-400)**
- Raspberry Pi 4: $45-65
- Sensores múltiples: $25-40
- Sistema solar: $80-120
- Componentes varios: $50-75

## 🎯 **Recomendaciones por Caso de Uso**

### **Jardín Doméstico Simple**
- **Recomendado**: Arduino Nano
- **Razón**: Económico, fácil de configurar
- **Características**: Riego básico, sin conectividad

### **Jardín con Control Remoto**
- **Recomendado**: ESP32
- **Razón**: WiFi integrado, buena relación precio/rendimiento
- **Características**: Control web, múltiples sensores

### **Invernadero Profesional**
- **Recomendado**: Raspberry Pi
- **Razón**: Máxima potencia, análisis avanzado
- **Características**: Base de datos, múltiples zonas, reportes

### **Investigación Agrícola**
- **Recomendado**: Raspberry Pi
- **Razón**: Análisis de datos, integración con otros sistemas
- **Características**: Machine learning, predicciones, IoT

## 🔍 **Monitoreo y Mantenimiento**

### **Verificaciones Diarias**
- ✅ Estado de la batería
- ✅ Lecturas del sensor
- ✅ Funcionamiento del riego
- ✅ Conexión WiFi (ESP32/Raspberry Pi)

### **Mantenimiento Semanal**
- 🔧 Limpieza del sensor de humedad
- 🔧 Verificación de conexiones
- 🔧 Limpieza del filtro de agua
- 🔧 Revisión del panel solar

### **Mantenimiento Mensual**
- 📊 Análisis de datos de riego
- 🔋 Verificación de capacidad de batería
- 🌱 Ajuste de umbrales según estación
- 🧹 Limpieza general del sistema

## 🔒 **Seguridad y Protección**

### **Protección Eléctrica**
- Fusibles en líneas principales
- Diodos de protección
- Capacitores de filtrado
- Aislamiento de componentes

### **Protección Mecánica**
- Caja IP65 resistente al agua
- Cableado resistente a la intemperie
- Conectores impermeables
- Filtros de agua

### **Protección del Sistema**
- Verificación de voltaje de batería
- Protección contra sobrecarga
- Detección de errores
- Modo de emergencia

## 📊 **Análisis de Datos**

### **Métricas Principales**
- **Eficiencia de riego**: Agua utilizada vs. humedad alcanzada
- **Consumo energético**: Batería y panel solar
- **Patrones de riego**: Frecuencia y duración
- **Condiciones ambientales**: Temperatura, luz, humedad

### **Optimización**
- **Ajuste de umbrales** según tipo de planta
- **Programación estacional** según clima
- **Eficiencia energética** con modos de ahorro
- **Predicción** de necesidades de riego

## 🆘 **Soporte y Troubleshooting**

### **Problemas Comunes**

#### **Arduino/ESP32**
- **Batería baja**: Verificar panel solar y conexiones
- **Sensor erróneo**: Calibrar según tipo de suelo
- **WiFi desconectado**: Verificar credenciales y señal
- **Riego no funciona**: Verificar relé y bomba

#### **Raspberry Pi**
- **Error de permisos GPIO**: Ejecutar `sudo usermod -a -G gpio $USER`
- **I2C no detectado**: Habilitar I2C con `sudo raspi-config`
- **Servicio no inicia**: Verificar logs con `sudo journalctl -u irrigation-system`
- **Puerto ocupado**: Cambiar puerto en configuración

### **Recursos Adicionales**
- 📖 [Documentación Completa](docs/)
- 🔧 [Guía de Instalación](docs/installation.md)
- 🛠️ [Troubleshooting](docs/troubleshooting.md)
- 💬 [Issues](https://github.com/tu-usuario/sistema-riego/issues)

## 🤝 **Contribuir**

1. Fork el proyecto
2. Crea una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## 📄 **Licencia**

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para detalles.

---

**🌱 ¡Haz que tus plantas crezcan felices con el Sistema de Riego Inteligente! 💧**

**Elige tu controlador favorito: Arduino para simplicidad, ESP32 para conectividad, o Raspberry Pi para máxima potencia.** 