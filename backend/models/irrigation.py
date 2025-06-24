"""
Modelos para el sistema de riego
"""

from sqlalchemy import Column, Integer, Float, DateTime, Boolean, String, Text, ForeignKey
from sqlalchemy.orm import relationship
from sqlalchemy.sql import func
from .database import Base
import enum

class IrrigationType(enum.Enum):
    """Tipos de riego"""
    AUTOMATIC = "automatic"
    MANUAL = "manual"
    SCHEDULED = "scheduled"

class IrrigationStatus(enum.Enum):
    """Estados del riego"""
    PENDING = "pending"
    RUNNING = "running"
    COMPLETED = "completed"
    FAILED = "failed"
    CANCELLED = "cancelled"

class IrrigationRecord(Base):
    """Registro de riego"""
    __tablename__ = "irrigation_records"
    
    id = Column(Integer, primary_key=True, index=True)
    timestamp = Column(DateTime(timezone=True), server_default=func.now())
    duration = Column(Integer)  # duración en segundos
    water_volume = Column(Float)  # volumen en litros
    humidity_before = Column(Float)  # humedad antes del riego
    humidity_after = Column(Float)  # humedad después del riego
    irrigation_type = Column(String, default=IrrigationType.AUTOMATIC.value)
    status = Column(String, default=IrrigationStatus.COMPLETED.value)
    notes = Column(Text, nullable=True)
    
    # Relaciones
    sensor_id = Column(Integer, ForeignKey("sensors.id"))
    sensor = relationship("Sensor", back_populates="irrigation_records")

class IrrigationSchedule(Base):
    """Programación de riego"""
    __tablename__ = "irrigation_schedules"
    
    id = Column(Integer, primary_key=True, index=True)
    name = Column(String, nullable=False)
    description = Column(Text, nullable=True)
    is_active = Column(Boolean, default=True)
    start_time = Column(String)  # formato HH:MM
    end_time = Column(String)  # formato HH:MM
    days_of_week = Column(String)  # días separados por coma (1,2,3,4,5,6,7)
    duration = Column(Integer, default=30)  # duración en segundos
    min_humidity_threshold = Column(Float, default=30.0)
    max_humidity_threshold = Column(Float, default=80.0)
    created_at = Column(DateTime(timezone=True), server_default=func.now())
    updated_at = Column(DateTime(timezone=True), onupdate=func.now())
    
    # Relaciones
    sensor_id = Column(Integer, ForeignKey("sensors.id"))
    sensor = relationship("Sensor", back_populates="irrigation_schedules") 