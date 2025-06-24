"""
Servicio de riego
"""

import asyncio
import logging
from datetime import datetime, timedelta
from typing import Optional, List
from sqlalchemy.orm import Session
from sqlalchemy import and_, desc

from ..models.database import SessionLocal
from ..models.irrigation import IrrigationRecord, IrrigationSchedule, IrrigationType, IrrigationStatus
from ..models.sensor import Sensor, SensorReading
from .sensor_service import SensorService
from .notification_service import NotificationService

logger = logging.getLogger(__name__)

class IrrigationService:
    """Servicio para control de riego"""
    
    def __init__(self):
        self.sensor_service = SensorService()
        self.notification_service = NotificationService()
        self.is_watering = False
        self.current_irrigation_task = None
        
    async def start_automatic_irrigation(self, sensor_id: int, duration: int = 30) -> bool:
        """Iniciar riego automático"""
        try:
            if self.is_watering:
                logger.warning("Ya hay un riego en curso")
                return False
                
            # Verificar humedad actual
            current_humidity = await self.sensor_service.get_current_humidity(sensor_id)
            if current_humidity is None:
                logger.error(f"No se pudo obtener la humedad del sensor {sensor_id}")
                return False
                
            # Verificar si necesita riego
            if current_humidity > 80:  # Umbral máximo
                logger.info(f"Humedad alta ({current_humidity}%), no se necesita riego")
                return False
                
            # Iniciar riego
            self.is_watering = True
            self.current_irrigation_task = asyncio.create_task(
                self._run_irrigation(sensor_id, duration, IrrigationType.AUTOMATIC)
            )
            
            logger.info(f"Riego automático iniciado para sensor {sensor_id}")
            return True
            
        except Exception as e:
            logger.error(f"Error al iniciar riego automático: {e}")
            return False
    
    async def start_manual_irrigation(self, sensor_id: int, duration: int = 30) -> bool:
        """Iniciar riego manual"""
        try:
            if self.is_watering:
                logger.warning("Ya hay un riego en curso")
                return False
                
            self.is_watering = True
            self.current_irrigation_task = asyncio.create_task(
                self._run_irrigation(sensor_id, duration, IrrigationType.MANUAL)
            )
            
            logger.info(f"Riego manual iniciado para sensor {sensor_id}")
            return True
            
        except Exception as e:
            logger.error(f"Error al iniciar riego manual: {e}")
            return False
    
    async def stop_irrigation(self) -> bool:
        """Detener riego actual"""
        try:
            if not self.is_watering:
                logger.warning("No hay riego en curso")
                return False
                
            if self.current_irrigation_task:
                self.current_irrigation_task.cancel()
                
            self.is_watering = False
            await self._turn_off_pump()
            
            logger.info("Riego detenido")
            return True
            
        except Exception as e:
            logger.error(f"Error al detener riego: {e}")
            return False
    
    async def _run_irrigation(self, sensor_id: int, duration: int, irrigation_type: IrrigationType):
        """Ejecutar riego"""
        db = SessionLocal()
        try:
            # Obtener humedad antes del riego
            humidity_before = await self.sensor_service.get_current_humidity(sensor_id)
            
            # Activar bomba
            await self._turn_on_pump()
            
            # Esperar duración del riego
            await asyncio.sleep(duration)
            
            # Desactivar bomba
            await self._turn_off_pump()
            
            # Obtener humedad después del riego
            await asyncio.sleep(5)  # Esperar a que se estabilice
            humidity_after = await self.sensor_service.get_current_humidity(sensor_id)
            
            # Calcular volumen aproximado (estimación)
            water_volume = self._estimate_water_volume(duration)
            
            # Guardar registro
            irrigation_record = IrrigationRecord(
                sensor_id=sensor_id,
                duration=duration,
                water_volume=water_volume,
                humidity_before=humidity_before,
                humidity_after=humidity_after,
                irrigation_type=irrigation_type.value,
                status=IrrigationStatus.COMPLETED.value,
                notes=f"Riego {irrigation_type.value} completado"
            )
            
            db.add(irrigation_record)
            db.commit()
            
            # Enviar notificación
            await self.notification_service.send_irrigation_completed_notification(
                sensor_id, duration, humidity_before, humidity_after
            )
            
            logger.info(f"Riego completado: {duration}s, humedad {humidity_before}% → {humidity_after}%")
            
        except asyncio.CancelledError:
            logger.info("Riego cancelado")
            await self._turn_off_pump()
            
            # Guardar registro de cancelación
            irrigation_record = IrrigationRecord(
                sensor_id=sensor_id,
                duration=0,
                water_volume=0,
                humidity_before=humidity_before if 'humidity_before' in locals() else None,
                humidity_after=None,
                irrigation_type=irrigation_type.value,
                status=IrrigationStatus.CANCELLED.value,
                notes="Riego cancelado manualmente"
            )
            db.add(irrigation_record)
            db.commit()
            
        except Exception as e:
            logger.error(f"Error durante el riego: {e}")
            await self._turn_off_pump()
            
            # Guardar registro de error
            irrigation_record = IrrigationRecord(
                sensor_id=sensor_id,
                duration=0,
                water_volume=0,
                humidity_before=humidity_before if 'humidity_before' in locals() else None,
                humidity_after=None,
                irrigation_type=irrigation_type.value,
                status=IrrigationStatus.FAILED.value,
                notes=f"Error: {str(e)}"
            )
            db.add(irrigation_record)
            db.commit()
            
        finally:
            self.is_watering = False
            db.close()
    
    async def _turn_on_pump(self):
        """Activar bomba de agua"""
        # Aquí iría la lógica para activar el relé
        # Por ahora simulamos la activación
        logger.info("💧 Bomba de agua ACTIVADA")
        # TODO: Implementar control real del hardware
        
    async def _turn_off_pump(self):
        """Desactivar bomba de agua"""
        # Aquí iría la lógica para desactivar el relé
        # Por ahora simulamos la desactivación
        logger.info("💧 Bomba de agua DESACTIVADA")
        # TODO: Implementar control real del hardware
    
    def _estimate_water_volume(self, duration: int) -> float:
        """Estimar volumen de agua basado en duración"""
        # Estimación: 2 litros por minuto
        return (duration / 60) * 2.0
    
    async def get_irrigation_history(self, sensor_id: int, limit: int = 50) -> List[dict]:
        """Obtener historial de riego"""
        db = SessionLocal()
        try:
            records = db.query(IrrigationRecord).filter(
                IrrigationRecord.sensor_id == sensor_id
            ).order_by(desc(IrrigationRecord.timestamp)).limit(limit).all()
            
            return [
                {
                    "id": record.id,
                    "timestamp": record.timestamp,
                    "duration": record.duration,
                    "water_volume": record.water_volume,
                    "humidity_before": record.humidity_before,
                    "humidity_after": record.humidity_after,
                    "irrigation_type": record.irrigation_type,
                    "status": record.status,
                    "notes": record.notes
                }
                for record in records
            ]
        finally:
            db.close()
    
    async def get_irrigation_status(self) -> dict:
        """Obtener estado actual del riego"""
        return {
            "is_watering": self.is_watering,
            "current_task": self.current_irrigation_task is not None
        }
    
    async def cleanup(self):
        """Limpiar recursos"""
        if self.is_watering:
            await self.stop_irrigation() 