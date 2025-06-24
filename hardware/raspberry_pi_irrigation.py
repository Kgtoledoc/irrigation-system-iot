#!/usr/bin/env python3
"""
Sistema de Riego Inteligente con Raspberry Pi
Alimentado con Energía Solar

Características:
- Web server con Flask
- Base de datos SQLite
- API REST completa
- Dashboard en tiempo real
- Múltiples sensores
- Control remoto
- Logging avanzado
- Configuración web
"""

import RPi.GPIO as GPIO
import time
import json
import sqlite3
import threading
import logging
from datetime import datetime, timedelta
from flask import Flask, render_template, request, jsonify
from flask_socketio import SocketIO, emit
import Adafruit_ADS1x15
import smbus2
import bme280

# Configuración de logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler('irrigation_system.log'),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger(__name__)

# Configuración de GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# Pines GPIO
PUMP_RELAY_PIN = 18          # GPIO18 - Relé para bomba
BUTTON_PIN = 17              # GPIO17 - Botón manual
LED_STATUS_PIN = 27          # GPIO27 - LED de estado
SOLAR_CHARGE_PIN = 23        # GPIO23 - Voltaje panel solar
BATTERY_VOLTAGE_PIN = 24     # GPIO24 - Voltaje batería

# Configurar pines
GPIO.setup(PUMP_RELAY_PIN, GPIO.OUT)
GPIO.setup(LED_STATUS_PIN, GPIO.OUT)
GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(SOLAR_CHARGE_PIN, GPIO.IN)
GPIO.setup(BATTERY_VOLTAGE_PIN, GPIO.IN)

# Inicializar ADC (ADS1115)
adc = Adafruit_ADS1x15.ADS1115()
GAIN = 1  # ±4.096V

# Inicializar I2C para sensores
bus = smbus2.SMBus(1)

# Configuración del sistema
DEFAULT_CONFIG = {
    'min_humidity': 30,
    'max_humidity': 80,
    'watering_duration': 30,
    'check_interval': 300,
    'auto_mode': True,
    'battery_low_threshold': 11.0,
    'battery_high_threshold': 14.0
}

class IrrigationSystem:
    def __init__(self):
        self.is_watering = False
        self.system_enabled = True
        self.last_check = time.time()
        self.watering_start_time = 0
        self.config = self.load_config()
        self.sensor_data = {
            'soil_moisture': 0,
            'temperature': 0,
            'humidity': 0,
            'pressure': 0,
            'battery_voltage': 0,
            'solar_voltage': 0,
            'light_level': 0
        }
        
        # Inicializar Flask
        self.app = Flask(__name__)
        self.app.config['SECRET_KEY'] = 'irrigation_secret_key'
        self.socketio = SocketIO(self.app, cors_allowed_origins="*")
        
        # Configurar rutas
        self.setup_routes()
        
        # Inicializar base de datos
        self.init_database()
        
        # Inicializar GPIO
        GPIO.output(PUMP_RELAY_PIN, GPIO.LOW)
        GPIO.output(LED_STATUS_PIN, GPIO.LOW)
        
        logger.info("🌱 Sistema de Riego Inteligente Raspberry Pi inicializado")
    
    def setup_routes(self):
        """Configurar rutas de la aplicación web"""
        
        @self.app.route('/')
        def index():
            return render_template('dashboard.html')
        
        @self.app.route('/api/status')
        def get_status():
            return jsonify({
                'is_watering': self.is_watering,
                'system_enabled': self.system_enabled,
                'sensor_data': self.sensor_data,
                'config': self.config,
                'uptime': time.time() - self.watering_start_time if self.is_watering else 0
            })
        
        @self.app.route('/api/start_watering', methods=['POST'])
        def start_watering():
            if not self.is_watering:
                self.start_watering()
                return jsonify({'status': 'success', 'message': 'Riego iniciado'})
            else:
                return jsonify({'status': 'error', 'message': 'Ya hay un riego en curso'}), 400
        
        @self.app.route('/api/stop_watering', methods=['POST'])
        def stop_watering():
            if self.is_watering:
                self.stop_watering()
                return jsonify({'status': 'success', 'message': 'Riego detenido'})
            else:
                return jsonify({'status': 'error', 'message': 'No hay riego en curso'}), 400
        
        @self.app.route('/api/config', methods=['GET', 'POST'])
        def handle_config():
            if request.method == 'GET':
                return jsonify(self.config)
            else:
                data = request.get_json()
                self.config.update(data)
                self.save_config()
                return jsonify({'status': 'success', 'message': 'Configuración guardada'})
        
        @self.app.route('/api/history')
        def get_history():
            limit = request.args.get('limit', 100, type=int)
            history = self.get_irrigation_history(limit)
            return jsonify(history)
        
        @self.app.route('/api/sensors')
        def get_sensors():
            return jsonify(self.sensor_data)
    
    def init_database(self):
        """Inicializar base de datos SQLite"""
        try:
            conn = sqlite3.connect('irrigation.db')
            cursor = conn.cursor()
            
            # Tabla de riego
            cursor.execute('''
                CREATE TABLE IF NOT EXISTS irrigation_records (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
                    duration INTEGER,
                    water_volume REAL,
                    humidity_before REAL,
                    humidity_after REAL,
                    irrigation_type TEXT,
                    status TEXT,
                    notes TEXT
                )
            ''')
            
            # Tabla de lecturas de sensores
            cursor.execute('''
                CREATE TABLE IF NOT EXISTS sensor_readings (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
                    soil_moisture REAL,
                    temperature REAL,
                    humidity REAL,
                    pressure REAL,
                    battery_voltage REAL,
                    solar_voltage REAL,
                    light_level INTEGER
                )
            ''')
            
            conn.commit()
            conn.close()
            logger.info("✅ Base de datos inicializada")
            
        except Exception as e:
            logger.error(f"❌ Error al inicializar base de datos: {e}")
    
    def read_sensors(self):
        """Leer todos los sensores"""
        try:
            # Leer sensor de humedad del suelo (ADS1115)
            soil_raw = adc.read_adc(0, gain=GAIN)
            self.sensor_data['soil_moisture'] = self.convert_soil_moisture(soil_raw)
            
            # Leer sensor BME280 (temperatura, humedad, presión)
            try:
                calibration_params = bme280.load_calibration_params(bus, 0x76)
                data = bme280.sample(bus, 0x76, calibration_params)
                self.sensor_data['temperature'] = data.temperature
                self.sensor_data['humidity'] = data.humidity
                self.sensor_data['pressure'] = data.pressure
            except:
                logger.warning("Sensor BME280 no disponible")
            
            # Leer voltaje de batería
            battery_raw = adc.read_adc(1, gain=GAIN)
            self.sensor_data['battery_voltage'] = self.convert_voltage(battery_raw, 3.0)
            
            # Leer voltaje del panel solar
            solar_raw = adc.read_adc(2, gain=GAIN)
            self.sensor_data['solar_voltage'] = self.convert_voltage(solar_raw, 3.0)
            
            # Leer sensor de luz
            light_raw = adc.read_adc(3, gain=GAIN)
            self.sensor_data['light_level'] = light_raw
            
            # Guardar lectura en base de datos
            self.save_sensor_reading()
            
        except Exception as e:
            logger.error(f"Error al leer sensores: {e}")
    
    def convert_soil_moisture(self, raw_value):
        """Convertir lectura ADC a porcentaje de humedad"""
        # Calibración según el sensor específico
        # Valores típicos: 0 = muy húmedo, 32767 = muy seco
        moisture_percent = ((32767 - raw_value) / 32767) * 100
        return max(0, min(100, moisture_percent))
    
    def convert_voltage(self, raw_value, voltage_divider_ratio):
        """Convertir lectura ADC a voltaje"""
        # ADS1115 con GAIN=1: ±4.096V
        voltage = (raw_value / 32767) * 4.096 * voltage_divider_ratio
        return round(voltage, 2)
    
    def save_sensor_reading(self):
        """Guardar lectura de sensores en base de datos"""
        try:
            conn = sqlite3.connect('irrigation.db')
            cursor = conn.cursor()
            
            cursor.execute('''
                INSERT INTO sensor_readings 
                (soil_moisture, temperature, humidity, pressure, battery_voltage, solar_voltage, light_level)
                VALUES (?, ?, ?, ?, ?, ?, ?)
            ''', (
                self.sensor_data['soil_moisture'],
                self.sensor_data['temperature'],
                self.sensor_data['humidity'],
                self.sensor_data['pressure'],
                self.sensor_data['battery_voltage'],
                self.sensor_data['solar_voltage'],
                self.sensor_data['light_level']
            ))
            
            conn.commit()
            conn.close()
            
        except Exception as e:
            logger.error(f"Error al guardar lectura de sensores: {e}")
    
    def automatic_irrigation(self):
        """Control de riego automático"""
        current_time = time.time()
        
        if current_time - self.last_check >= self.config['check_interval']:
            self.last_check = current_time
            
            logger.info(f"🔍 Verificando humedad: {self.sensor_data['soil_moisture']:.1f}%")
            
            # Verificar si necesita riego
            if (self.sensor_data['soil_moisture'] < self.config['min_humidity'] and 
                not self.is_watering):
                logger.info("💧 Humedad baja, iniciando riego automático")
                self.start_watering('automatic')
            elif (self.sensor_data['soil_moisture'] > self.config['max_humidity'] and 
                  self.is_watering):
                logger.info("🛑 Humedad alta, deteniendo riego")
                self.stop_watering()
    
    def start_watering(self, irrigation_type='manual'):
        """Iniciar riego"""
        if not self.is_watering and self.sensor_data['battery_voltage'] > self.config['battery_low_threshold']:
            self.is_watering = True
            self.watering_start_time = time.time()
            GPIO.output(PUMP_RELAY_PIN, GPIO.HIGH)
            GPIO.output(LED_STATUS_PIN, GPIO.HIGH)
            
            logger.info(f"💧 Riego iniciado ({irrigation_type})")
            
            # Emitir evento WebSocket
            self.socketio.emit('watering_status', {'status': 'started', 'type': irrigation_type})
            
            # Guardar registro
            self.save_irrigation_record(irrigation_type, 'started')
    
    def stop_watering(self):
        """Detener riego"""
        if self.is_watering:
            self.is_watering = False
            duration = int(time.time() - self.watering_start_time)
            GPIO.output(PUMP_RELAY_PIN, GPIO.LOW)
            GPIO.output(LED_STATUS_PIN, GPIO.LOW)
            
            logger.info(f"💧 Riego detenido. Duración: {duration} segundos")
            
            # Emitir evento WebSocket
            self.socketio.emit('watering_status', {'status': 'stopped', 'duration': duration})
            
            # Guardar registro
            self.save_irrigation_record('manual', 'completed', duration)
    
    def save_irrigation_record(self, irrigation_type, status, duration=0):
        """Guardar registro de riego"""
        try:
            conn = sqlite3.connect('irrigation.db')
            cursor = conn.cursor()
            
            cursor.execute('''
                INSERT INTO irrigation_records 
                (duration, water_volume, humidity_before, humidity_after, irrigation_type, status, notes)
                VALUES (?, ?, ?, ?, ?, ?, ?)
            ''', (
                duration,
                duration * 2.0 / 60,  # Estimación: 2L/min
                self.sensor_data['soil_moisture'],
                self.sensor_data['soil_moisture'],  # Se actualizará después
                irrigation_type,
                status,
                f"Riego {irrigation_type} - {status}"
            ))
            
            conn.commit()
            conn.close()
            
        except Exception as e:
            logger.error(f"Error al guardar registro de riego: {e}")
    
    def get_irrigation_history(self, limit=100):
        """Obtener historial de riego"""
        try:
            conn = sqlite3.connect('irrigation.db')
            cursor = conn.cursor()
            
            cursor.execute('''
                SELECT * FROM irrigation_records 
                ORDER BY timestamp DESC 
                LIMIT ?
            ''', (limit,))
            
            records = cursor.fetchall()
            conn.close()
            
            return [
                {
                    'id': record[0],
                    'timestamp': record[1],
                    'duration': record[2],
                    'water_volume': record[3],
                    'humidity_before': record[4],
                    'humidity_after': record[5],
                    'irrigation_type': record[6],
                    'status': record[7],
                    'notes': record[8]
                }
                for record in records
            ]
            
        except Exception as e:
            logger.error(f"Error al obtener historial: {e}")
            return []
    
    def check_battery_status(self):
        """Verificar estado de la batería"""
        if self.sensor_data['battery_voltage'] < self.config['battery_low_threshold']:
            logger.warning("⚠️ BATERÍA BAJA - Deshabilitando riego")
            self.system_enabled = False
            if self.is_watering:
                self.stop_watering()
        elif self.sensor_data['battery_voltage'] > self.config['battery_high_threshold']:
            self.system_enabled = True
        
        # Verificar carga solar
        if self.sensor_data['solar_voltage'] > self.sensor_data['battery_voltage'] + 0.5:
            logger.info("☀️ Cargando batería")
    
    def check_manual_button(self):
        """Verificar botón manual"""
        if GPIO.input(BUTTON_PIN) == GPIO.LOW:
            if not self.is_watering:
                logger.info("🔘 Riego manual iniciado por botón")
                self.start_watering('manual')
            else:
                logger.info("🔘 Riego manual detenido por botón")
                self.stop_watering()
            time.sleep(0.5)  # Debounce
    
    def load_config(self):
        """Cargar configuración desde archivo"""
        try:
            with open('config.json', 'r') as f:
                config = json.load(f)
                logger.info("✅ Configuración cargada")
                return config
        except FileNotFoundError:
            logger.info("📝 Creando configuración por defecto")
            self.save_config(DEFAULT_CONFIG)
            return DEFAULT_CONFIG
        except Exception as e:
            logger.error(f"Error al cargar configuración: {e}")
            return DEFAULT_CONFIG
    
    def save_config(self, config=None):
        """Guardar configuración en archivo"""
        if config:
            self.config = config
        
        try:
            with open('config.json', 'w') as f:
                json.dump(self.config, f, indent=2)
            logger.info("✅ Configuración guardada")
        except Exception as e:
            logger.error(f"Error al guardar configuración: {e}")
    
    def monitoring_loop(self):
        """Bucle principal de monitoreo"""
        while True:
            try:
                # Leer sensores
                self.read_sensors()
                
                # Verificar botón manual
                self.check_manual_button()
                
                # Control de riego automático
                if self.system_enabled and self.config['auto_mode']:
                    self.automatic_irrigation()
                
                # Verificar estado de la batería
                self.check_battery_status()
                
                # Emitir datos por WebSocket
                self.socketio.emit('sensor_data', self.sensor_data)
                
                # Verificar si debe detenerse el riego por tiempo
                if self.is_watering:
                    elapsed = time.time() - self.watering_start_time
                    if elapsed >= self.config['watering_duration']:
                        logger.info("⏰ Tiempo de riego completado")
                        self.stop_watering()
                
                time.sleep(1)
                
            except KeyboardInterrupt:
                logger.info("🛑 Sistema detenido por usuario")
                break
            except Exception as e:
                logger.error(f"Error en bucle de monitoreo: {e}")
                time.sleep(5)
    
    def run(self):
        """Ejecutar el sistema"""
        try:
            # Iniciar hilo de monitoreo
            monitor_thread = threading.Thread(target=self.monitoring_loop, daemon=True)
            monitor_thread.start()
            
            logger.info("🚀 Iniciando servidor web en http://0.0.0.0:5000")
            
            # Ejecutar servidor web
            self.socketio.run(self.app, host='0.0.0.0', port=5000, debug=False)
            
        except KeyboardInterrupt:
            logger.info("🛑 Sistema detenido")
        finally:
            GPIO.cleanup()

if __name__ == "__main__":
    system = IrrigationSystem()
    system.run() 