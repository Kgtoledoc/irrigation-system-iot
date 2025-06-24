#!/usr/bin/env python3
"""
Sistema de Riego Inteligente - Backend Principal
"""

import uvicorn
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from contextlib import asynccontextmanager
import logging
from dotenv import load_dotenv
import os

from backend.api.routes import irrigation, sensors, dashboard, auth
from backend.services.irrigation_service import IrrigationService
from backend.services.sensor_service import SensorService
from backend.utils.database import init_db
from backend.utils.logger import setup_logging

# Cargar variables de entorno
load_dotenv()

# Configurar logging
setup_logging()
logger = logging.getLogger(__name__)

@asynccontextmanager
async def lifespan(app: FastAPI):
    """Gestión del ciclo de vida de la aplicación"""
    # Inicializar servicios al arrancar
    logger.info("🚀 Iniciando Sistema de Riego Inteligente...")
    
    # Inicializar base de datos
    await init_db()
    
    # Inicializar servicios
    irrigation_service = IrrigationService()
    sensor_service = SensorService()
    
    # Iniciar monitoreo de sensores
    await sensor_service.start_monitoring()
    
    logger.info("✅ Sistema iniciado correctamente")
    
    yield
    
    # Limpiar recursos al cerrar
    logger.info("🛑 Cerrando Sistema de Riego Inteligente...")
    await sensor_service.stop_monitoring()
    await irrigation_service.cleanup()

# Crear aplicación FastAPI
app = FastAPI(
    title="Sistema de Riego Inteligente",
    description="API para control y monitoreo de sistema de riego automatizado",
    version="1.0.0",
    lifespan=lifespan
)

# Configurar CORS
app.add_middleware(
    CORSMiddleware,
    allow_origins=os.getenv("CORS_ORIGINS", ["http://localhost:3000"]),
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Incluir rutas
app.include_router(auth.router, prefix="/api/auth", tags=["Autenticación"])
app.include_router(irrigation.router, prefix="/api/irrigation", tags=["Riego"])
app.include_router(sensors.router, prefix="/api/sensors", tags=["Sensores"])
app.include_router(dashboard.router, prefix="/api/dashboard", tags=["Dashboard"])

@app.get("/")
async def root():
    """Endpoint raíz"""
    return {
        "message": "🌱 Sistema de Riego Inteligente",
        "version": "1.0.0",
        "status": "operativo",
        "docs": "/docs"
    }

@app.get("/health")
async def health_check():
    """Verificación de salud del sistema"""
    return {
        "status": "healthy",
        "timestamp": "2024-01-01T00:00:00Z"
    }

if __name__ == "__main__":
    # Configuración del servidor
    host = os.getenv("API_HOST", "0.0.0.0")
    port = int(os.getenv("API_PORT", 8000))
    debug = os.getenv("DEBUG", "False").lower() == "true"
    
    logger.info(f"🌐 Iniciando servidor en {host}:{port}")
    
    uvicorn.run(
        "main:app",
        host=host,
        port=port,
        reload=debug,
        log_level="info"
    ) 