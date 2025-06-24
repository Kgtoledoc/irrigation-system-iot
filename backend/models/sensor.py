"""
Modelos para sensores
"""

from sqlalchemy import Column, Integer, Float, DateTime, Boolean, String, Text, ForeignKey
from sqlalchemy.orm import relationship
from sqlalchemy.sql import func
from .database import Base
import enum

class SensorType(enum.Enum):
    """Tipos de sensores"""
    SOIL_MOISTURE = "soil_moisture"
    TEMPERATURE = "temperature"
    HUMIDITY = "humidity"
    LIGHT = "light"
    PH = "ph"

class SensorStatus(enum.Enum):
    """Estados del sensor"""
    ACTIVE = "active"
    INACTIVE = "inactive"
    ERROR = "error"
    MAINTENANCE = "maintenance"

class Sensor(Base):
    """Sensor"""
    __tablename__ = "sensors"
    
    id = Column(Integer, primary_key=True, index=True)
    name = Column(String, nullable=False)
    sensor_type = Column(String, default=SensorType.SOIL_MOISTURE.value)
    location = Column(String, nullable=True)
    description = Column(Text, nullable=True)
    is_active = Column(Boolean, default=True)
    status = Column(String, default=SensorStatus.ACTIVE.value)
    calibration_offset = Column(Float, default=0.0)
    calibration_scale = Column(Float, default=1.0)
    min_threshold = Column(Float, default=0.0)
    max_threshold = Column(Float, default=100.0)
    created_at = Column(DateTime(timezone=True), server_default=func.now())
    updated_at = Column(DateTime(timezone=True), onupdate=func.now())
    
    # Relaciones
    readings = relationship("SensorReading", back_populates="sensor", cascade="all, delete-orphan")
    irrigation_records = relationship("IrrigationRecord", back_populates="sensor")
    irrigation_schedules = relationship("IrrigationSchedule", back_populates="sensor")

class SensorReading(Base):
    """Lectura de sensor"""
    __tablename__ = "sensor_readings"
    
    id = Column(Integer, primary_key=True, index=True)
    timestamp = Column(DateTime(timezone=True), server_default=func.now())
    value = Column(Float, nullable=False)
    unit = Column(String, default="%")
    is_valid = Column(Boolean, default=True)
    notes = Column(Text, nullable=True)
    
    # Relaciones
    sensor_id = Column(Integer, ForeignKey("sensors.id"))
    sensor = relationship("Sensor", back_populates="readings") 