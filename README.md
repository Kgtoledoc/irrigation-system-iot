# Sistema de Riego Inteligente 🌱💧

Un sistema de riego automatizado para plantas que monitorea la humedad del suelo y controla el riego de manera inteligente.

## Características

- 🌡️ **Monitoreo de humedad del suelo** en tiempo real
- 💧 **Control automático de riego** basado en umbrales configurables
- 📱 **Interfaz web** para monitoreo y control remoto
- 📊 **Dashboard** con gráficos y estadísticas
- 🔔 **Sistema de alertas** por email/notificaciones
- 📝 **Historial completo** de riego y lecturas
- ⚙️ **Configuración flexible** de horarios y umbrales
- 🔋 **Modo de ahorro de energía** con sensores de bajo consumo

## Hardware Requerido

- **ESP32** o **Raspberry Pi** como controlador principal
- **Sensor de humedad del suelo** (capacitivo recomendado)
- **Bomba de agua** de 12V con relé
- **Módulo relé** para controlar la bomba
- **Fuente de alimentación** 12V
- **Tubos y conectores** para el sistema de riego
- **Resistencia de 10kΩ** (para sensor resistivo)

## Instalación

### 1. Clonar el repositorio
```bash
git clone <tu-repositorio>
cd sistema-riego
```

### 2. Instalar dependencias
```bash
pip install -r requirements.txt
```

### 3. Configurar variables de entorno
```bash
cp .env.example .env
# Editar .env con tus configuraciones
```

### 4. Ejecutar el sistema
```bash
python main.py
```

## Estructura del Proyecto

```
sistema-riego/
├── backend/              # Lógica del servidor
│   ├── api/             # Endpoints de la API
│   ├── models/          # Modelos de datos
│   ├── services/        # Servicios de negocio
│   └── utils/           # Utilidades
├── frontend/            # Interfaz web
│   ├── components/      # Componentes React
│   ├── pages/          # Páginas
│   └── styles/         # Estilos CSS
├── hardware/           # Código para ESP32/Arduino
├── docs/              # Documentación
└── config/            # Archivos de configuración
```

## Configuración del Hardware

### Conexiones del Sensor de Humedad
- VCC → 3.3V
- GND → GND
- AOUT → GPIO 36 (ADC1_CH0)

### Conexiones del Relé
- VCC → 5V
- GND → GND
- IN → GPIO 26

### Conexiones de la Bomba
- Positivo → Relé NO (Normalmente Abierto)
- Negativo → GND

## Uso

1. **Configuración inicial**: Ajusta los umbrales de humedad en la interfaz web
2. **Monitoreo**: Revisa el dashboard para ver el estado actual
3. **Control manual**: Activa/desactiva el riego manualmente si es necesario
4. **Alertas**: Recibe notificaciones cuando la humedad esté baja

## Contribuir

1. Fork el proyecto
2. Crea una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## Licencia

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para detalles.

## Soporte

Si tienes problemas o preguntas, abre un issue en el repositorio. 