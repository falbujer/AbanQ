# Arquitectura y Diagrama de AbanQ

## Descripción General de la Arquitectura

AbanQ está construido sobre una arquitectura de capas que proporciona flexibilidad, escalabilidad y mantenibilidad.

```
┌─────────────────────────────────────────────────────────┐
│                   Capa de Presentación                   │
│              (Interfaz Gráfica Qt - Desktop)             │
├─────────────────────────────────────────────────────────┤
│                   Capa de Aplicación                     │
│     (Lógica de negocios, Módulos, Scripts Qt)           │
├─────────────────────────────────────────────────────────┤
│                    Capa de Datos                         │
│        (Base de Datos: PostgreSQL, MySQL, SQLite)       │
└─────────────────────────────────────────────────────────┘
```

## Componentes Principales

### 1. Motor Principal (Engine)

```
AbanQ Engine
├── Core (C++)
│   ├── Gestión de aplicación
│   ├── Gestión de usuarios y permisos
│   ├── Caché de datos
│   └── Comunicación con BD
├── Módulos
│   ├── Contabilidad
│   ├── Facturación
│   ├── Inventario
│   ├── Compras
│   ├── Ventas
│   └── Tesorería
└── Librerías
    ├── Qt Framework
    ├── Qt SQL
    ├── Qt Script
    └── Utilidades
```

### 2. Interfaz Gráfica

```
Interfaz de Usuario (Qt)
├── Ventanas principales
├── Formularios (Forms)
├── Tablas de datos (DataTable)
├── Reportes
└── Diálogos de configuración
```

### 3. Persistencia de Datos

```
Sistemas de Base de Datos Soportados
├── PostgreSQL (Recomendado)
├── MySQL / MariaDB
└── SQLite (Desarrollo)

Características:
├── Transacciones ACID
├── Integridad referencial
├── Índices para rendimiento
└── Backups automáticos
```

## Flujo de Datos

```
Usuario
   │
   ▼
┌────────────────────────┐
│  Interfaz Gráfica (Qt) │
└────────────────────────┘
   │
   ▼
┌────────────────────────┐
│  Lógica de Negocio     │
│  (Módulos/Scripts)     │
└────────────────────────┘
   │
   ▼
┌────────────────────────┐
│  Motor de Aplicación   │
│  (Core Engine C++)     │
└────────────────────────┘
   │
   ▼
┌────────────────────────┐
│  Capa de Acceso a BD   │
└────────────────────────┘
   │
   ▼
┌────────────────────────┐
│  Base de Datos         │
│  (PostgreSQL/MySQL)    │
└────────────────────────┘
```

## Estructura de Módulos

```
AbanQ Modules
├── Módulos de Contabilidad
│   ├── Diarios
│   ├── Mayor contable
│   ├── Balances
│   └── Cierres
├── Módulos de Operaciones
│   ├── Facturación
│   ├── Inventario
│   ├── Compras
│   └── Ventas
├── Módulos de Tesorería
│   ├── Caja
│   ├── Bancos
│   └── Flujo de caja
└── Módulos de Reportes
    ├── Financieros
    ├── Operacionales
    └── Personalizados
```

## Tecnologías Utilizadas

### Frontend
- **Qt Framework**: Framework de interfaz gráfica multiplataforma
- **C++**: Lenguaje principal
- **Qt Designer**: Para diseño de interfaces

### Backend
- **C++**: Lógica de negocio
- **Qt Script**: Lenguaje de scripting integrado
- **XML/JSON**: Formatos de datos

### Base de Datos
- **PostgreSQL**: Recomendado para producción
- **MySQL/MariaDB**: Alternativa compatible
- **SQLite**: Para desarrollo/pruebas

### Librerías Adicionales
- **OpenSSL**: Para encriptación
- **Zlib**: Para compresión
- **ODBC**: Para conectores de BD

## Estructura de Directorios

```
AbanQ/
├── engine/                      # Motor principal
│   ├── src/                    # Código fuente
│   │   ├── flbase/            # Base del framework
│   │   ├── advance/           # Componentes avanzados
│   │   ├── barcode/           # Códigos de barras
│   │   ├── dbf/               # Manejo de archivos DBF
│   │   ├── kugar/             # Motor de reportes
│   │   ├── qsa/               # Qt Script engine
│   │   ├── scripts/           # Scripts base
│   │   └── plugins/           # Plugins
│   ├── build.sh               # Script de compilación
│   ├── clean.sh               # Limpieza de builds
│   └── Doxyfile               # Documentación
├── genesis/                     # Módulos adicionales
├── docs/                       # Documentación
└── README.md                   # Este archivo
```

## Ciclo de Desarrollo

```
1. Instalación
   ↓
2. Configuración Inicial
   ├── Base de datos
   ├── Empresa
   └── Usuarios
   ↓
3. Activación de Módulos
   ├── Seleccionar módulos necesarios
   └── Configurar permisos
   ↓
4. Migración de Datos (opcional)
   ├── Importar datos históricos
   └── Validar integridad
   ↓
5. Operación Normal
   ├── Entrada de datos diarios
   ├── Generación de reportes
   └── Mantenimiento
   ↓
6. Crecimiento
   ├── Activar nuevos módulos
   ├── Aumentar usuarios
   └── Optimizar rendimiento
```

## Características de Seguridad

```
Niveles de Seguridad
├── Autenticación
│   ├── Usuario/Contraseña
│   └── Permisos por rol
├── Autorización
│   ├── Control de módulos
│   ├── Control de tablas
│   └── Control de campos
├── Auditoría
│   ├── Log de accesos
│   ├── Log de cambios
│   └── Log de eliminaciones
└── Protección de Datos
    ├── Cifrado de datos sensibles
    ├── Backups automáticos
    └── Control de acceso concurrente
```

## Escalabilidad

### Horizontal
```
Múltiples clientes
      │
      ├─── Servidor Web (futuro)
      │
      └─── Servidor de BD Central
            ├─── PostgreSQL Primary
            └─── PostgreSQL Replica (backup)
```

### Vertical
```
Optimizaciones de rendimiento
├── Índices de BD
├── Caché en memoria
├── Compresión de datos
├── Particionamiento de tablas
└── Hardware más potente
```

## Integración con Sistemas Externos

### Métodos de integración
```
AbanQ
├── API REST (futuro)
├── Web Services (XML-RPC)
├── Import/Export (CSV, Excel)
├── Base de datos compartida
└── Conectores personalizados
```

### Ejemplos
- Sincronización con e-commerce
- Integración con sistemas de pago
- Conectores con marketplaces
- APIs propias de terceros

## Performance y Benchmarks

### Métricas típicas
```
Operación                    | Tiempo      | Usuarios
─────────────────────────────┼─────────────┼──────────
Apertura de formulario       | 200-500ms   | -
Búsqueda en tabla (1M recs)  | 100-300ms   | -
Generación de reporte        | 1-5 seg     | -
Usuarios simultáneos         | -           | 20-50+
Transacciones/segundo        | -           | 100+
```

### Factores que afectan performance
- Tamaño de base de datos
- Hardware del servidor
- Configuración de índices
- Complejidad de reportes
- Número de usuarios simultáneos

## Roadmap Técnico

### Corto plazo (2024-2025)
- Mejoras de rendimiento
- Actualización de dependencias
- Bug fixes y estabilidad

### Mediano plazo (2025-2026)
- Interfaz web (opcional)
- Aplicación móvil
- Nuevos conectores

### Largo plazo (2026+)
- Microservicios (opcional)
- Machine Learning
- Análisis avanzado

---

## Recursos de Desarrollo

### Para desarrolladores que quieran compilar

```bash
# Requisitos
- Qt 4.8+
- C++ compiler (g++, clang, MSVC)
- PostgreSQL/MySQL dev libraries
- cmake o qmake

# Compilación
./engine/build.sh

# O manualmente
cd engine/src
qmake -r src.pro
make -j4
```

### Documentación interna
- Código comentado en C++
- Scripts de ejemplo
- Comentarios Doxygen

### Contribuciones
- Fork del repositorio
- Branch para tu feature
- Pull request con cambios

---

*Para más detalles técnicos, consulta el código fuente en [GitHub](https://github.com/falbujer/AbanQ)*

*Última actualización: 19 de enero de 2026*
