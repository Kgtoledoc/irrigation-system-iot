# 📋 Plantillas para Notion - Sistema de Riego Inteligente

## 🗄️ Base de Datos: Componentes Hardware

### Propiedades de la Base de Datos

| Propiedad | Tipo | Descripción |
|---|---|---|
| **Nombre** | Title | Nombre del componente |
| **Categoría** | Select | Sensor, Controlador, Actuador, Energía, Interfaz |
| **Estado** | Select | Pendiente, Comprado, Instalado, Funcionando, Defectuoso |
| **Precio** | Number | Precio en euros |
| **Proveedor** | Text | Tienda donde comprarlo |
| **URL** | URL | Enlace de compra |
| **Cantidad** | Number | Cantidad necesaria |
| **Prioridad** | Select | Baja, Media, Alta, Crítica |
| **Notas** | Text | Observaciones adicionales |
| **Fecha Compra** | Date | Cuándo se compró |
| **Garantía** | Date | Fecha de vencimiento de garantía |

### Fórmulas Útiles

#### Costo Total
```
prop("Precio") * prop("Cantidad")
```

#### Estado de Garantía
```
if(prop("Garantía") < now(), "Vencida", "Vigente")
```

#### Prioridad con Color
```
if(prop("Prioridad") == "Crítica", "🔴", 
if(prop("Prioridad") == "Alta", "🟡", 
if(prop("Prioridad") == "Media", "🟢", "⚪")))
```

### Vista: Por Categoría
- **Agrupar por**: Categoría
- **Ordenar por**: Prioridad (descendente)
- **Filtrar**: Estado no es "Funcionando"

### Vista: Por Estado
- **Agrupar por**: Estado
- **Ordenar por**: Fecha Compra (descendente)

---

## 📊 Base de Datos: Registro de Sensores

### Propiedades de la Base de Datos

| Propiedad | Tipo | Descripción |
|---|---|---|
| **Fecha/Hora** | Date & Time | Timestamp de la lectura |
| **Humedad Suelo** | Number | Porcentaje de humedad (0-100) |
| **Temperatura** | Number | Temperatura en °C |
| **Luz Solar** | Number | Nivel de luz (0-1023) |
| **Estado Riego** | Select | Inactivo, Activo, Error |
| **Duración Riego** | Number | Segundos de riego |
| **Batería** | Number | Nivel de batería (0-100) |
| **Zona** | Select | Zona 1, Zona 2, Zona 3 |
| **Notas** | Text | Observaciones |

### Fórmulas Útiles

#### Estado de Humedad
```
if(prop("Humedad Suelo") < 30, "🌵 Seco", 
if(prop("Humedad Suelo") < 60, "🌱 Moderado", 
if(prop("Humedad Suelo") < 80, "✅ Óptimo", "💦 Húmedo")))
```

#### Consumo de Agua (L)
```
prop("Duración Riego") * 0.1
```

#### Eficiencia de Riego
```
if(prop("Humedad Suelo") > 80, "⚠️ Excesivo", 
if(prop("Humedad Suelo") < 30, "⚠️ Insuficiente", "✅ Óptimo"))
```

### Vista: Dashboard
- **Filtro**: Últimas 24 horas
- **Agrupar por**: Zona
- **Mostrar**: Promedios de humedad y temperatura

### Vista: Análisis Mensual
- **Agrupar por**: Mes
- **Calcular**: Suma de duración de riego
- **Calcular**: Promedio de humedad

---

## 🔧 Base de Datos: Mantenimiento

### Propiedades de la Base de Datos

| Propiedad | Tipo | Descripción |
|---|---|---|
| **Tarea** | Title | Descripción de la tarea |
| **Tipo** | Select | Preventivo, Correctivo, Calibración |
| **Frecuencia** | Select | Diario, Semanal, Mensual, Trimestral, Semestral |
| **Estado** | Select | Pendiente, En Progreso, Completada, Cancelada |
| **Prioridad** | Select | Baja, Media, Alta, Crítica |
| **Asignado** | Person | Responsable de la tarea |
| **Fecha Programada** | Date | Cuándo debe realizarse |
| **Fecha Completada** | Date | Cuándo se completó |
| **Duración** | Number | Tiempo estimado en minutos |
| **Herramientas** | Multi-select | Herramientas necesarias |
| **Notas** | Text | Observaciones |

### Fórmulas Útiles

#### Días de Retraso
```
if(prop("Fecha Programada") < now() and prop("Estado") == "Pendiente", 
dateBetween(prop("Fecha Programada"), now(), "days"), 0)
```

#### Estado con Color
```
if(prop("Estado") == "Completada", "✅", 
if(prop("Estado") == "En Progreso", "🔄", 
if(prop("Estado") == "Cancelada", "❌", "⏳")))
```

#### Próxima Fecha
```
if(prop("Frecuencia") == "Diario", dateAdd(prop("Fecha Completada"), 1, "days"),
if(prop("Frecuencia") == "Semanal", dateAdd(prop("Fecha Completada"), 7, "days"),
if(prop("Frecuencia") == "Mensual", dateAdd(prop("Fecha Completada"), 30, "days"),
if(prop("Frecuencia") == "Trimestral", dateAdd(prop("Fecha Completada"), 90, "days"),
dateAdd(prop("Fecha Completada"), 180, "days")))))
```

### Vista: Calendario
- **Mostrar**: Fecha Programada
- **Agrupar por**: Asignado
- **Filtrar**: Estado no es "Completada"

### Vista: Tareas Pendientes
- **Filtrar**: Estado es "Pendiente"
- **Ordenar por**: Fecha Programada (ascendente)
- **Agrupar por**: Prioridad

---

## 🐛 Base de Datos: Problemas y Soluciones

### Propiedades de la Base de Datos

| Propiedad | Tipo | Descripción |
|---|---|---|
| **Problema** | Title | Descripción del problema |
| **Categoría** | Select | Hardware, Software, Energía, Conectividad |
| **Severidad** | Select | Baja, Media, Alta, Crítica |
| **Estado** | Select | Reportado, En Investigación, Solucionado, Cerrado |
| **Reportado por** | Person | Quién reportó el problema |
| **Asignado a** | Person | Quién está resolviendo |
| **Fecha Reporte** | Date | Cuándo se reportó |
| **Fecha Resolución** | Date | Cuándo se solucionó |
| **Causa Raíz** | Text | Causa identificada |
| **Solución** | Text | Solución aplicada |
| **Tiempo Resolución** | Number | Horas para resolver |
| **Prevención** | Text | Cómo evitar en el futuro |

### Fórmulas Útiles

#### Tiempo de Resolución (Horas)
```
if(prop("Fecha Resolución"), 
dateBetween(prop("Fecha Reporte"), prop("Fecha Resolución"), "hours"), 0)
```

#### Severidad con Color
```
if(prop("Severidad") == "Crítica", "🔴", 
if(prop("Severidad") == "Alta", "🟠", 
if(prop("Severidad") == "Media", "🟡", "🟢")))
```

#### Estado de Resolución
```
if(prop("Estado") == "Solucionado", "✅", 
if(prop("Estado") == "En Investigación", "🔍", 
if(prop("Estado") == "Cerrado", "🔒", "📝")))
```

### Vista: Problemas Abiertos
- **Filtrar**: Estado no es "Cerrado"
- **Ordenar por**: Severidad (descendente)
- **Agrupar por**: Categoría

### Vista: Análisis de Tiempo
- **Filtrar**: Estado es "Solucionado"
- **Agrupar por**: Categoría
- **Calcular**: Promedio de tiempo de resolución

---

## 📚 Base de Datos: Recursos y Enlaces

### Propiedades de la Base de Datos

| Propiedad | Tipo | Descripción |
|---|---|---|
| **Título** | Title | Nombre del recurso |
| **Tipo** | Select | Documentación, Video, Código, Herramienta, Comunidad |
| **Categoría** | Select | Arduino, ESP32, Raspberry Pi, Hardware, Software |
| **URL** | URL | Enlace al recurso |
| **Descripción** | Text | Descripción del recurso |
| **Idioma** | Select | Español, Inglés, Otro |
| **Nivel** | Select | Básico, Intermedio, Avanzado |
| **Estado** | Select | Activo, Obsoleto, En Revisión |
| **Fecha Agregado** | Date | Cuándo se agregó |
| **Última Visita** | Date | Última vez que se consultó |
| **Notas** | Text | Observaciones personales |

### Fórmulas Útiles

#### Antigüedad del Recurso
```
dateBetween(prop("Fecha Agregado"), now(), "days")
```

#### Estado de Actualización
```
if(prop("Última Visita") < dateAdd(now(), -30, "days"), "⚠️ Desactualizado", "✅ Actualizado")
```

#### Nivel con Icono
```
if(prop("Nivel") == "Básico", "🟢", 
if(prop("Nivel") == "Intermedio", "🟡", "🔴"))
```

### Vista: Por Categoría
- **Agrupar por**: Categoría
- **Ordenar por**: Fecha Agregado (descendente)

### Vista: Recursos Favoritos
- **Filtro**: Estado es "Activo" y Nivel es "Básico"
- **Ordenar por**: Última Visita (descendente)

---

## 🎯 Base de Datos: Proyectos y Mejoras

### Propiedades de la Base de Datos

| Propiedad | Tipo | Descripción |
|---|---|---|
| **Proyecto** | Title | Nombre del proyecto |
| **Tipo** | Select | Nueva Funcionalidad, Mejora, Bug Fix, Optimización |
| **Estado** | Select | Idea, Planificado, En Desarrollo, Testing, Completado |
| **Prioridad** | Select | Baja, Media, Alta, Crítica |
| **Complejidad** | Select | Simple, Moderada, Compleja, Muy Compleja |
| **Asignado** | Person | Responsable del proyecto |
| **Fecha Inicio** | Date | Cuándo comenzó |
| **Fecha Objetivo** | Date | Cuándo debe completarse |
| **Fecha Completado** | Date | Cuándo se completó |
| **Tiempo Estimado** | Number | Horas estimadas |
| **Tiempo Real** | Number | Horas reales |
| **Descripción** | Text | Descripción detallada |
| **Requisitos** | Text | Requisitos del proyecto |
| **Notas** | Text | Observaciones |

### Fórmulas Útiles

#### Progreso del Proyecto
```
if(prop("Estado") == "Completado", 100,
if(prop("Estado") == "Testing", 80,
if(prop("Estado") == "En Desarrollo", 50,
if(prop("Estado") == "Planificado", 20, 0))))
```

#### Retraso (Días)
```
if(prop("Fecha Objetivo") < now() and prop("Estado") != "Completado", 
dateBetween(prop("Fecha Objetivo"), now(), "days"), 0)
```

#### Eficiencia
```
if(prop("Tiempo Real"), 
round(prop("Tiempo Estimado") / prop("Tiempo Real") * 100), 0)
```

### Vista: Roadmap
- **Agrupar por**: Estado
- **Ordenar por**: Fecha Objetivo (ascendente)
- **Filtrar**: Estado no es "Completado"

### Vista: Análisis de Tiempo
- **Filtrar**: Estado es "Completado"
- **Agrupar por**: Tipo
- **Calcular**: Promedio de eficiencia

---

## 📋 Plantilla: Checklist de Instalación

### Propiedades del Template

| Propiedad | Tipo | Descripción |
|---|---|---|
| **Paso** | Title | Descripción del paso |
| **Categoría** | Select | Hardware, Software, Configuración, Prueba |
| **Completado** | Checkbox | Si está completado |
| **Dependencias** | Relation | Pasos que deben completarse antes |
| **Tiempo Estimado** | Number | Minutos estimados |
| **Dificultad** | Select | Fácil, Moderado, Difícil |
| **Notas** | Text | Observaciones |

### Fórmulas Útiles

#### Progreso General
```
round(prop("Completado") * 100)
```

#### Tiempo Restante
```
if(not prop("Completado"), prop("Tiempo Estimado"), 0)
```

### Vista: Progreso
- **Agrupar por**: Categoría
- **Ordenar por**: Dependencias
- **Mostrar**: Progreso por categoría

---

## 🔗 Relaciones entre Bases de Datos

### Relaciones Recomendadas

1. **Componentes ↔ Mantenimiento**
   - Un componente puede tener múltiples tareas de mantenimiento

2. **Problemas ↔ Componentes**
   - Un problema puede afectar a múltiples componentes

3. **Recursos ↔ Categorías**
   - Los recursos se relacionan con categorías específicas

4. **Proyectos ↔ Problemas**
   - Los proyectos pueden resolver problemas específicos

### Fórmulas de Relación

#### Componentes con Problemas
```
length(prop("Problemas Relacionados"))
```

#### Mantenimiento Pendiente por Componente
```
length(filter(prop("Tareas Mantenimiento"), 
{Estado: "Pendiente"}))
```

---

## 📊 Dashboard Principal

### Widgets Recomendados

1. **Estado del Sistema**
   - Última lectura de sensores
   - Estado actual de riego
   - Nivel de batería

2. **Tareas Pendientes**
   - Mantenimiento próximo
   - Problemas abiertos
   - Proyectos en desarrollo

3. **Métricas Clave**
   - Eficiencia de riego
   - Consumo de agua
   - Tiempo de funcionamiento

4. **Enlaces Rápidos**
   - Documentación
   - Código fuente
   - Comunidad

### Fórmulas del Dashboard

#### Estado General del Sistema
```
if(prop("Última Lectura") > dateAdd(now(), -1, "hours"), "✅ Online", "⚠️ Offline")
```

#### Próximo Mantenimiento
```
formatDate(min(filter(prop("Mantenimiento"), {Estado: "Pendiente"}).prop("Fecha Programada")))
```

---

*Estas plantillas están optimizadas para Notion y pueden ser importadas directamente. Ajusta las propiedades y fórmulas según tus necesidades específicas.* 