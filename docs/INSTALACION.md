# Guía de Instalación de AbanQ

## Descripción General

AbanQ es un sistema ERP modular que se puede instalar en Windows, Linux y macOS. Esta guía te ayudará a instalarlo correctamente.

## Requisitos Previos

- **Procesador**: x86 compatible
- **Memoria RAM**: Mínimo 512 MB (1 GB recomendado)
- **Espacio en disco**: 500 MB libres
- **Base de datos**: PostgreSQL, MySQL o SQLite
- **Conexión a Internet**: Para descargas iniciales

## Instalación en Windows

### Paso 1: Descargar el instalador
1. Ve a [GitHub Releases](https://github.com/falbujer/AbanQ/releases)
2. Descarga `AbanQ-2.5.0.38719-Windows-x86-Install.exe`
3. Guarda el archivo en una carpeta fácil de ubicar

### Paso 2: Ejecutar el instalador
1. Haz doble clic en el archivo `.exe`
2. Acepta los términos de licencia GPL-2.0
3. Selecciona la carpeta de instalación (por defecto: `C:\Program Files\AbanQ`)
4. Elige los módulos que deseas instalar
5. Sigue los pasos hasta completar la instalación

### Paso 3: Configurar la base de datos
1. Inicia AbanQ desde el menú Inicio
2. En la primera ejecución, se te pedirá configurar la base de datos
3. Selecciona el motor de base de datos (PostgreSQL, MySQL o SQLite)
4. Ingresa los datos de conexión

### Paso 4: Crear empresa
1. Configura tu primera empresa
2. Ingresa datos básicos (nombre, CIF, domicilio)
3. Elige los módulos activos para tu empresa

## Instalación en Linux

### Descarga e instalación
```bash
# Descargar el instalador
wget https://github.com/falbujer/AbanQ/releases/download/2.5.0.38719/AbanQ-2.5.0.38719-Linux-x86_64-Install

# Hacer ejecutable
chmod +x AbanQ-2.5.0.38719-Linux-x86_64-Install

# Ejecutar instalador
./AbanQ-2.5.0.38719-Linux-x86_64-Install
```

### Instalación desde terminal (alternativa)
```bash
# Instalar dependencias (ejemplo para Ubuntu/Debian)
sudo apt-get install libqt4-core libqt4-gui

# Ejecutar instalador con permisos
sudo ./AbanQ-2.5.0.38719-Linux-x86_64-Install
```

## Instalación en macOS

### Paso 1: Descargar
Descarga `AbanQ-2.5.0.38719-MacOS-X-x86-Install.dmg` desde [GitHub Releases](https://github.com/falbujer/AbanQ/releases)

### Paso 2: Instalar
1. Haz doble clic en el archivo `.dmg`
2. Arrastra la aplicación AbanQ a la carpeta Aplicaciones
3. Abre Aplicaciones y haz doble clic en AbanQ

### Paso 3: Permitir ejecución
Si ves un mensaje de seguridad:
1. Ve a Preferencias del Sistema > Seguridad y privacidad
2. Haz clic en "Abrir de todas formas"

## Configuración Inicial

### Base de datos

**Opción 1: SQLite (recomendado para iniciarse)**
- Requiere menos configuración
- Ideal para pequeñas empresas
- Limitado para múltiples usuarios

**Opción 2: PostgreSQL (recomendado para producción)**
```bash
# Instalar PostgreSQL (Ubuntu/Debian)
sudo apt-get install postgresql postgresql-contrib

# Crear usuario para AbanQ
sudo -u postgres createuser abanq -P
```

**Opción 3: MySQL**
```bash
# Instalar MySQL (Ubuntu/Debian)
sudo apt-get install mysql-server

# Crear base de datos
mysql -u root -p
CREATE DATABASE abanq;
GRANT ALL ON abanq.* TO 'abanq'@'localhost' IDENTIFIED BY 'password';
```

## Compilar desde Fuente

### Requisitos de desarrollo
```bash
# Ubuntu/Debian
sudo apt-get install build-essential git qt4-dev-tools libqt4-dev

# Fedora/RHEL
sudo dnf install gcc-c++ git qt4-devel

# macOS (con Homebrew)
brew install qt4
```

### Clonar y compilar
```bash
# Clonar repositorio
git clone https://github.com/falbujer/AbanQ.git
cd AbanQ

# Compilar
./engine/build.sh

# Instalar (opcional)
sudo make install
```

## Solución de Problemas

### Problema: "No se puede conectar a la base de datos"
**Solución:**
- Verifica que el servidor de base de datos esté ejecutándose
- Revisa los datos de conexión en la configuración
- Asegúrate de que el usuario de base de datos tiene permisos

### Problema: "Interfaz gráfica distorsionada"
**Solución:**
- Actualiza los drivers de video
- Instala las librerías Qt necesarias: `libqt4-gui libqt4-core`

### Problema: "Falta de espacio en disco"
**Solución:**
- Libera al menos 500 MB de espacio
- Considera usar una carpeta de instalación alternativa

### Problema: "Puerto 5432 en uso (PostgreSQL)"
**Solución:**
- Cambia el puerto de PostgreSQL en postgresql.conf
- O termina el proceso que está usando el puerto

## Próximos Pasos

1. **Configurar la empresa**: Ingresa datos de tu negocio
2. **Crear usuarios**: Añade otros usuarios del sistema
3. **Activar módulos**: Habilita los módulos que necesitas
4. **Importar datos**: Si migras desde otro sistema
5. **Capacitarse**: Consulta la documentación de cada módulo

## Recursos Útiles

- [Documentación oficial](https://github.com/falbujer/AbanQ)
- [Módulos disponibles](https://github.com/falbujer/AbanQ-Modules)
- [Reportar problemas](https://github.com/falbujer/AbanQ/issues)

---

*Última actualización: 19 de enero de 2026*
