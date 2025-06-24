"""
Configuración de base de datos y sesiones
"""

from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
import os
from dotenv import load_dotenv

load_dotenv()

# Configuración de la base de datos
DATABASE_URL = os.getenv("DATABASE_URL", "sqlite:///./irrigation.db")

# Crear motor de base de datos
engine = create_engine(
    DATABASE_URL,
    echo=False,  # Cambiar a True para debug
    pool_pre_ping=True,
    pool_recycle=300
)

# Crear sesión local
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

# Base para modelos
Base = declarative_base()

def get_db():
    """Obtener sesión de base de datos"""
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

async def init_db():
    """Inicializar base de datos"""
    from .irrigation import IrrigationRecord, IrrigationSchedule
    from .sensor import SensorReading, Sensor
    from .user import User
    
    # Crear todas las tablas
    Base.metadata.create_all(bind=engine) 