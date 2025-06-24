"""
Servicio de sensores
"""

import asyncio
import logging
import random
from datetime import datetime, timedelta
from typing import Optional, List, Dict
from sqlalchemy.orm import Session
from sqlalchemy import desc, func

from ..models.database import SessionLocal
from ..models.sensor import Sensor, SensorReading, SensorStatus
from .notification_service import NotificationService

logger = logging.getLogger(__name__)

class SensorService:
    """Servicio para gestión de sensores"""
    
    def __init__(self):
        self.notification_service = NotificationService()
        self.monitoring_task = None
        self.is_monitoring = False
        
    async def start_monitoring(self):
        """Iniciar monitoreo de sensores"""
        if self.is_monitoring:
            logger.warning("El monitoreo ya está activo")
            return
            
        self.is_monitoring = True
        self.monitoring_task = asyncio.create_task(self._monitor_sensors())
        logger.info("🔍 Monitoreo de sensores iniciado")
    
    async def stop_monitoring(self):
        """Detener monitoreo de sensores"""
        if not self.is_monitoring:
            return
            
        self.is_monitoring = False
        if self.monitoring_task:
            self.monitoring_task.cancel()
            try:
                await self.monitoring_task
            except asyncio.CancelledError:
                pass
        logger.info("🔍 Monitoreo de sensores detenido")
    
    async def _monitor_sensors(self):
        """Bucle de monitoreo de sensores"""
        while self.is_monitoring:
            try:
                # Obtener todos los sensores activos
                sensors = await self.get_active_sensors()
                
                for sensor in sensors:
                    # Leer sensor
                    reading = await self._read_sensor(sensor)
                    if reading is not None:
                        # Guardar lectura
                        await self.save_sensor_reading(sensor.id, reading)
                        
                        # Verificar umbrales
                        await self._check_thresholds(sensor, reading)
                
                # Esperar antes de la siguiente lectura
                await asyncio.sleep(60)  # Leer cada minuto
                
            except asyncio.CancelledError:
                break
            except Exception as e:
                logger.error(f"Error en monitoreo de sensores: {e}")
                await asyncio.sleep(30)  # Esperar antes de reintentar
    
    async def _read_sensor(self, sensor: Sensor) -> Optional[float]:
        """Leer valor del sensor"""
        try:
            # TODO: Implementar lectura real del hardware
            # Por ahora simulamos lecturas
            if sensor.sensor_type == "soil_moisture":
                # Simular lectura de humedad del suelo (30-90%)
                base_value = 60
                variation = random.uniform(-20, 20)
                value = max(0, min(100, base_value + variation))
            elif sensor.sensor_type == "temperature":
                # Simular temperatura (15-35°C)
                value = random.uniform(15, 35)
            elif sensor.sensor_type == "humidity":
                # Simular humedad ambiental (40-80%)
                value = random.uniform(40, 80)
            else:
                value = random.uniform(0, 100)
            
            # Aplicar calibración
            calibrated_value = (value * sensor.calibration_scale) + sensor.calibration_offset
            
            logger.debug(f"Sensor {sensor.name}: {value} → {calibrated_value}")
            return calibrated_value
            
        except Exception as e:
            logger.error(f"Error al leer sensor {sensor.name}: {e}")
            return None
    
    async def _check_thresholds(self, sensor: Sensor, value: float):
        """Verificar umbrales del sensor"""
        try:
            if value < sensor.min_threshold:
                await self.notification_service.send_low_threshold_alert(
                    sensor.id, sensor.name, value, sensor.min_threshold
                )
                logger.warning(f"Sensor {sensor.name} bajo umbral mínimo: {value} < {sensor.min_threshold}")
                
            elif value > sensor.max_threshold:
                await self.notification_service.send_high_threshold_alert(
                    sensor.id, sensor.name, value, sensor.max_threshold
                )
                logger.warning(f"Sensor {sensor.name} sobre umbral máximo: {value} > {sensor.max_threshold}")
                
        except Exception as e:
            logger.error(f"Error al verificar umbrales: {e}")
    
    async def get_current_humidity(self, sensor_id: int) -> Optional[float]:
        """Obtener humedad actual de un sensor"""
        db = SessionLocal()
        try:
            reading = db.query(SensorReading).filter(
                SensorReading.sensor_id == sensor_id,
                SensorReading.is_valid == True
            ).order_by(desc(SensorReading.timestamp)).first()
            
            return reading.value if reading else None
            
        finally:
            db.close()
    
    async def get_active_sensors(self) -> List[Sensor]:
        """Obtener sensores activos"""
        db = SessionLocal()
        try:
            sensors = db.query(Sensor).filter(
                Sensor.is_active == True,
                Sensor.status == SensorStatus.ACTIVE.value
            ).all()
            return sensors
        finally:
            db.close()
    
    async def save_sensor_reading(self, sensor_id: int, value: float, notes: str = None) -> bool:
        """Guardar lectura de sensor"""
        db = SessionLocal()
        try:
            reading = SensorReading(
                sensor_id=sensor_id,
                value=value,
                is_valid=True,
                notes=notes
            )
            
            db.add(reading)
            db.commit()
            
            logger.debug(f"Lectura guardada: sensor {sensor_id}, valor {value}")
            return True
            
        except Exception as e:
            logger.error(f"Error al guardar lectura: {e}")
            db.rollback()
            return False
        finally:
            db.close()
    
    async def get_sensor_readings(self, sensor_id: int, hours: int = 24) -> List[Dict]:
        """Obtener lecturas de sensor de las últimas horas"""
        db = SessionLocal()
        try:
            since = datetime.now() - timedelta(hours=hours)
            
            readings = db.query(SensorReading).filter(
                SensorReading.sensor_id == sensor_id,
                SensorReading.timestamp >= since,
                SensorReading.is_valid == True
            ).order_by(SensorReading.timestamp).all()
            
            return [
                {
                    "timestamp": reading.timestamp,
                    "value": reading.value,
                    "unit": reading.unit
                }
                for reading in readings
            ]
            
        finally:
            db.close()
    
    async def get_sensor_statistics(self, sensor_id: int, hours: int = 24) -> Dict:
        """Obtener estadísticas del sensor"""
        db = SessionLocal()
        try:
            since = datetime.now() - timedelta(hours=hours)
            
            # Obtener estadísticas
            stats = db.query(
                func.avg(SensorReading.value).label('avg_value'),
                func.min(SensorReading.value).label('min_value'),
                func.max(SensorReading.value).label('max_value'),
                func.count(SensorReading.id).label('readings_count')
            ).filter(
                SensorReading.sensor_id == sensor_id,
                SensorReading.timestamp >= since,
                SensorReading.is_valid == True
            ).first()
            
            return {
                "average": float(stats.avg_value) if stats.avg_value else 0,
                "minimum": float(stats.min_value) if stats.min_value else 0,
                "maximum": float(stats.max_value) if stats.max_value else 0,
                "readings_count": stats.readings_count or 0
            }
            
        finally:
            db.close() 