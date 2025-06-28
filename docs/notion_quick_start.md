# 🌱 Sistema de Riego Inteligente - Guía Rápida

> 💡 **Nota**: Esta es la versión rápida para Notion. Para documentación completa, consulta el archivo principal.

---

## 🚀 Inicio Rápido

### ¿Qué necesitas para empezar?

**Componentes Mínimos:**
- Arduino Nano ($5-10)
- Sensor de humedad ($3-8)
- Relé 5V ($2-5)
- Bomba 12V ($8-15)
- LED + resistencia ($1-2)
- Botón ($1-2)

**Total: ~$20-40**

### Pasos Básicos:

1. **Conectar componentes** según el diagrama
2. **Subir código** `arduino_test_basic.ino`
3. **Probar con comandos** en monitor serial
4. **Calibrar sensor** con comando `CALIBRATE`

---

## 📋 Checklist de Instalación

### Hardware
- [ ] Arduino conectado por USB
- [ ] Sensor de humedad en A0
- [ ] Relé en D7
- [ ] LED en D13 (con resistencia 220Ω)
- [ ] Botón en D2
- [ ] Fuente 12V para bomba

### Software
- [ ] Arduino IDE instalado
- [ ] Código subido al Arduino
- [ ] Monitor serial abierto (9600 baudios)
- [ ] Librerías instaladas (si usas display)

### Pruebas
- [ ] Comando `STATUS` funciona
- [ ] Comando `READ` lee sensor
- [ ] Comando `PUMP_ON` activa bomba
- [ ] Botón manual funciona
- [ ] LED indica estado

---

## 🔧 Comandos Básicos

| Comando | Función | Ejemplo |
|---|---|---|
| `STATUS` | Ver estado del sistema | `STATUS` |
| `READ` | Leer sensor de humedad | `READ` |
| `PUMP_ON` | Encender bomba | `PUMP_ON` |
| `PUMP_OFF` | Apagar bomba | `PUMP_OFF` |
| `TEST` | Prueba automática | `TEST` |
| `CALIBRATE` | Calibrar sensor | `CALIBRATE` |

---

## ⚠️ Problemas Comunes

### Sensor no lee
> 🔍 **Causa**: Conexiones sueltas o sensor mal calibrado
> 
> **Solución**: 
> 1. Verificar VCC, GND, A0
> 2. Ejecutar `CALIBRATE`
> 3. Insertar sensor correctamente

### Bomba no funciona
> 🔍 **Causa**: Relé o fuente de alimentación
> 
> **Solución**:
> 1. Verificar fuente 12V
> 2. Verificar conexiones del relé
> 3. Probar relé manualmente

### Sistema no responde
> 🔍 **Causa**: Arduino bloqueado o código con error
> 
> **Solución**:
> 1. Reiniciar Arduino (botón reset)
> 2. Verificar código
> 3. Probar con código básico

---

## 📊 Configuración Avanzada

### Parámetros de Riego
```cpp
#define MIN_HUMIDITY_THRESHOLD 30    // Humedad mínima (%)
#define MAX_HUMIDITY_THRESHOLD 80    // Humedad máxima (%)
#define WATERING_DURATION 30         // Duración riego (segundos)
#define CHECK_INTERVAL 300000        // Intervalo verificación (5 min)
```

### Tipos de Plantas
| Tipo de Planta | Humedad Mínima | Humedad Máxima |
|---|---|---|
| **Cactus/Suculentas** | 20% | 60% |
| **Plantas de Interior** | 30% | 80% |
| **Hortalizas** | 40% | 85% |
| **Flores** | 35% | 80% |

---

## 🌞 Energía Solar (Opcional)

### Componentes Adicionales
- Panel solar 20W ($25-40)
- Batería 12V 7Ah ($20-35)
- Controlador de carga ($8-15)

### Conexiones
```
Panel Solar → Controlador → Batería 12V
                        ↓
                    Arduino (5V)
                    Bomba (12V)
```

### Ventajas
- ✅ Funcionamiento 24/7
- ✅ Sin costos de electricidad
- ✅ Instalación en exteriores
- ✅ Completamente autónomo

---

## 📱 Control Remoto (ESP32)

### Ventajas del ESP32
- ✅ WiFi integrado
- ✅ Interfaz web
- ✅ Control desde móvil
- ✅ Datos en tiempo real

### Configuración WiFi
```cpp
const char* ssid = "TU_WIFI_SSID";
const char* password = "TU_WIFI_PASSWORD";
```

### Acceso Web
- IP: `192.168.1.100` (o la que asigne tu router)
- Puerto: `80`
- Interfaz: Dashboard completo

---

## 🔄 Mantenimiento

### Mensual
- [ ] Limpiar sensores
- [ ] Verificar conexiones
- [ ] Limpiar panel solar
- [ ] Revisar nivel de agua

### Trimestral
- [ ] Calibrar sensores
- [ ] Verificar batería
- [ ] Actualizar software

---

## 📞 Soporte

### Recursos
- **Documentación completa**: Ver archivo principal
- **Código fuente**: GitHub del proyecto
- **Comunidad**: Foro Arduino

### Contacto
- **Email**: soporte@sistema-riego.com
- **WhatsApp**: +34 600 000 000

---

## 🎯 Próximos Pasos

1. **Probar sistema básico** ✅
2. **Agregar energía solar** ☀️
3. **Implementar ESP32** 📱
4. **Escalar a múltiples zonas** 🔄
5. **Integrar con IoT** 🌐

---

> 💡 **Consejo**: Comienza con el sistema básico y ve agregando funcionalidades gradualmente. Es mejor tener algo funcionando que un sistema complejo que no funciona.

---

*Guía rápida v1.0 - Sistema de Riego Inteligente* 