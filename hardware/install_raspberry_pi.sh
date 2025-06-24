#!/bin/bash

# Script de Instalación - Sistema de Riego Inteligente Raspberry Pi
# Autor: Sistema de Riego Inteligente
# Versión: 1.0

set -e  # Salir si hay algún error

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Función para imprimir mensajes
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Función para verificar si es Raspberry Pi
check_raspberry_pi() {
    if ! grep -q "Raspberry Pi" /proc/cpuinfo; then
        print_error "Este script debe ejecutarse en un Raspberry Pi"
        exit 1
    fi
    print_success "Raspberry Pi detectado"
}

# Función para actualizar el sistema
update_system() {
    print_status "Actualizando sistema..."
    sudo apt-get update
    sudo apt-get upgrade -y
    print_success "Sistema actualizado"
}

# Función para instalar dependencias del sistema
install_system_dependencies() {
    print_status "Instalando dependencias del sistema..."
    
    # Paquetes básicos
    sudo apt-get install -y python3-pip python3-dev python3-venv
    sudo apt-get install -y git curl wget
    
    # Herramientas I2C
    sudo apt-get install -y i2c-tools python3-smbus
    sudo apt-get install -y libgpiod-dev
    
    # Herramientas de desarrollo
    sudo apt-get install -y build-essential cmake pkg-config
    
    # Herramientas de red
    sudo apt-get install -y hostapd dnsmasq
    
    print_success "Dependencias del sistema instaladas"
}

# Función para habilitar I2C
enable_i2c() {
    print_status "Habilitando I2C..."
    
    # Verificar si I2C ya está habilitado
    if grep -q "i2c_arm=on" /boot/config.txt; then
        print_warning "I2C ya está habilitado"
    else
        # Habilitar I2C
        echo "dtparam=i2c_arm=on" | sudo tee -a /boot/config.txt
        print_success "I2C habilitado (requiere reinicio)"
    fi
}

# Función para configurar permisos
setup_permissions() {
    print_status "Configurando permisos..."
    
    # Agregar usuario a grupos necesarios
    sudo usermod -a -G gpio $USER
    sudo usermod -a -G i2c $USER
    sudo usermod -a -G dialout $USER
    
    print_success "Permisos configurados"
}

# Función para crear directorio del proyecto
create_project_directory() {
    print_status "Creando directorio del proyecto..."
    
    PROJECT_DIR="/home/$USER/sistema-riego"
    
    if [ -d "$PROJECT_DIR" ]; then
        print_warning "El directorio $PROJECT_DIR ya existe"
        read -p "¿Desea sobrescribir? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            rm -rf "$PROJECT_DIR"
        else
            print_error "Instalación cancelada"
            exit 1
        fi
    fi
    
    mkdir -p "$PROJECT_DIR"
    cd "$PROJECT_DIR"
    
    print_success "Directorio del proyecto creado: $PROJECT_DIR"
}

# Función para instalar dependencias de Python
install_python_dependencies() {
    print_status "Instalando dependencias de Python..."
    
    # Crear entorno virtual
    python3 -m venv venv
    source venv/bin/activate
    
    # Actualizar pip
    pip install --upgrade pip
    
    # Instalar dependencias
    pip install Flask==2.3.3
    pip install Flask-SocketIO==5.3.6
    pip install python-socketio==5.8.0
    pip install RPi.GPIO==0.7.1
    pip install Adafruit-ADS1x15==1.0.2
    pip install smbus2==0.4.3
    pip install bme280==0.1.0
    pip install python-dateutil==2.8.2
    pip install requests==2.31.0
    pip install psutil==5.9.5
    
    print_success "Dependencias de Python instaladas"
}

# Función para crear archivos del sistema
create_system_files() {
    print_status "Creando archivos del sistema..."
    
    # Crear directorio de templates
    mkdir -p templates
    
    # Crear archivo de configuración por defecto
    cat > config.json << EOF
{
  "min_humidity": 30,
  "max_humidity": 80,
  "watering_duration": 30,
  "check_interval": 300,
  "auto_mode": true,
  "battery_low_threshold": 11.0,
  "battery_high_threshold": 14.0
}
EOF
    
    # Crear script de inicio
    cat > start_irrigation.sh << EOF
#!/bin/bash
cd /home/$USER/sistema-riego
source venv/bin/activate
python3 raspberry_pi_irrigation.py
EOF
    
    chmod +x start_irrigation.sh
    
    # Crear servicio systemd
    sudo tee /etc/systemd/system/irrigation-system.service > /dev/null << EOF
[Unit]
Description=Sistema de Riego Inteligente
After=network.target

[Service]
Type=simple
User=$USER
WorkingDirectory=/home/$USER/sistema-riego
ExecStart=/home/$USER/sistema-riego/start_irrigation.sh
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
EOF
    
    # Habilitar servicio
    sudo systemctl daemon-reload
    sudo systemctl enable irrigation-system.service
    
    print_success "Archivos del sistema creados"
}

# Función para configurar red WiFi (opcional)
setup_wifi() {
    print_status "Configurando WiFi..."
    
    read -p "¿Desea configurar WiFi ahora? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        read -p "SSID de WiFi: " wifi_ssid
        read -s -p "Contraseña de WiFi: " wifi_password
        echo
        
        # Configurar WiFi
        sudo tee -a /etc/wpa_supplicant/wpa_supplicant.conf > /dev/null << EOF

network={
    ssid="$wifi_ssid"
    psk="$wifi_password"
}
EOF
        
        print_success "WiFi configurado"
    else
        print_warning "WiFi no configurado"
    fi
}

# Función para verificar hardware
verify_hardware() {
    print_status "Verificando hardware..."
    
    # Verificar I2C
    if command -v i2cdetect &> /dev/null; then
        print_status "Dispositivos I2C detectados:"
        sudo i2cdetect -y 1
    else
        print_warning "i2cdetect no disponible"
    fi
    
    # Verificar GPIO
    if python3 -c "import RPi.GPIO as GPIO; print('GPIO disponible')" 2>/dev/null; then
        print_success "GPIO funcionando correctamente"
    else
        print_error "Error con GPIO"
    fi
}

# Función para mostrar información final
show_final_info() {
    print_success "Instalación completada!"
    echo
    echo "📋 Información importante:"
    echo "=========================="
    echo "• Directorio del proyecto: /home/$USER/sistema-riego"
    echo "• Servicio systemd: irrigation-system.service"
    echo "• Puerto web: 5000"
    echo "• Logs: /home/$USER/sistema-riego/irrigation_system.log"
    echo
    echo "🚀 Comandos útiles:"
    echo "=================="
    echo "• Iniciar sistema: sudo systemctl start irrigation-system"
    echo "• Detener sistema: sudo systemctl stop irrigation-system"
    echo "• Ver estado: sudo systemctl status irrigation-system"
    echo "• Ver logs: sudo journalctl -u irrigation-system -f"
    echo "• Acceder web: http://[IP_RASPBERRY_PI]:5000"
    echo
    echo "⚠️  IMPORTANTE:"
    echo "=============="
    echo "• Reinicia el Raspberry Pi para aplicar todos los cambios"
    echo "• Verifica las conexiones de hardware antes de iniciar"
    echo "• El sistema se iniciará automáticamente después del reinicio"
    echo
    read -p "¿Desea reiniciar ahora? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        sudo reboot
    else
        print_warning "Recuerda reiniciar manualmente más tarde"
    fi
}

# Función principal
main() {
    echo "🌱 Sistema de Riego Inteligente - Instalador Raspberry Pi"
    echo "========================================================"
    echo
    
    # Verificar que se ejecute como usuario normal
    if [ "$EUID" -eq 0 ]; then
        print_error "No ejecutes este script como root"
        exit 1
    fi
    
    # Verificar Raspberry Pi
    check_raspberry_pi
    
    # Actualizar sistema
    update_system
    
    # Instalar dependencias
    install_system_dependencies
    
    # Habilitar I2C
    enable_i2c
    
    # Configurar permisos
    setup_permissions
    
    # Crear directorio del proyecto
    create_project_directory
    
    # Instalar dependencias de Python
    install_python_dependencies
    
    # Crear archivos del sistema
    create_system_files
    
    # Configurar WiFi (opcional)
    setup_wifi
    
    # Verificar hardware
    verify_hardware
    
    # Mostrar información final
    show_final_info
}

# Ejecutar función principal
main "$@" 