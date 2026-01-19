# Preguntas Frecuentes (FAQ)

## Instalación y Requisitos

### ¿Cuáles son los requisitos mínimos para AbanQ?

**Mínimos:**
- Procesador: x86 compatible
- RAM: 512 MB
- Disco duro: 500 MB libres
- Sistema operativo: Windows XP SP3+, Linux cualquier distro reciente, macOS 10.5+

**Recomendados:**
- RAM: 1-2 GB
- Disco duro: 1-5 GB (según volumen de datos)
- Procesador: Dual-core o superior
- Conexión de red para múltiples usuarios

### ¿Es difícil instalar AbanQ?

No. El instalador guiado en Windows es muy sencillo. En Linux y macOS también se proporciona un instalador ejecutable. El proceso típico toma 10-15 minutos.

### ¿Necesito una licencia para usar AbanQ?

No. AbanQ es completamente libre bajo la licencia GPL-2.0. No tienes que pagar nada por instalar o usar el software.

### ¿Puedo usar AbanQ en múltiples computadoras?

Sí. Una vez instalada la base de datos central (PostgreSQL/MySQL), múltiples usuarios pueden conectarse desde diferentes máquinas.

---

## Bases de Datos

### ¿Qué base de datos debo usar?

Recomendaciones por escenario:

| Escenario | BD Recomendada | Razón |
|---|---|---|
| **Pruebas / Desarrollo** | SQLite | Instalación simple, sin servidor |
| **1-5 usuarios** | MySQL o PostgreSQL | Fiable, fácil de instalar |
| **5+ usuarios** | PostgreSQL | Mejor para concurrencia |
| **Alto rendimiento** | PostgreSQL | Optimizado para grandes volúmenes |

### ¿Puedo cambiar de base de datos después?

Sí, pero requiere una migración de datos. Se recomienda planificar desde el inicio.

### ¿Debo instalar PostgreSQL/MySQL yo mismo?

No es obligatorio. Puedes usar:
1. **Tu propio servidor**: Instalación local o en red
2. **Servidor de terceros**: Cloud como AWS RDS, Azure Database
3. **SQLite**: Sin servidor externo (solo pruebas)

### ¿Puedo hacer backup automático?

Sí. Ambas PostgreSQL y MySQL tienen utilidades de backup. AbanQ también puede exportar datos regularmente.

---

## Funcionalidades

### ¿AbanQ tiene módulo de recursos humanos?

Actualmente no está incluido en el core. Sin embargo, se pueden desarrollar extensiones o utilizarse integraciones con sistemas externos.

### ¿Puedo personalizar los reportes?

Sí. AbanQ permite crear reportes personalizados. Requiere conocimiento de SQL o scripts Qt.

### ¿Soporta código de barras?

Sí. El módulo de inventario soporta lectura de códigos de barras con lectores estándar.

### ¿Puedo importar datos de Excel?

Sí. Hay herramientas para importar datos desde CSV y Excel a las tablas de AbanQ.

### ¿Soporta múltiples monedas?

Sí, aunque con algunas limitaciones. Se requiere configuración manual de tipos de cambio.

### ¿Puedo crear múltiples empresas/sucursales?

Sí. AbanQ está diseñado para gestionar múltiples empresas o sucursales en la misma base de datos.

---

## Uso y Operación

### ¿Dónde almacena AbanQ los datos?

En la base de datos configurada (PostgreSQL, MySQL o SQLite). Los datos nunca se guardan en ficheros locales por defecto.

### ¿Puedo trabajar offline?

No de manera nativa. Se requiere conexión a la base de datos. Sin embargo, es posible desarrollar soluciones con caché local.

### ¿Cuánto cuesta un usuario adicional?

Nada. No hay costos por número de usuarios. Puedes tener 2 o 200 usuarios.

### ¿Hay límite de registros en la base de datos?

No. AbanQ puede gestionar millones de registros, limitado solo por la base de datos.

### ¿Cómo controlo permisos de usuarios?

AbanQ tiene un sistema granular de permisos basado en:
- Módulos (acceso total o ninguno)
- Tablas (lectura, escritura, borrado)
- Campos (según reglas personalizadas)

### ¿Puedo crear usuarios sin acceso a ciertos módulos?

Sí. El administrador puede asignar permisos específicos a cada usuario o grupo.

---

## Migración y Datos

### ¿Cómo migro desde otro ERP?

Pasos típicos:
1. Exportar datos del ERP anterior (generalmente a CSV/Excel)
2. Preparar los datos según el formato de AbanQ
3. Usar herramientas de importación
4. Validar integridad de datos
5. Hacer pruebas antes de datos reales

Se recomienda contactar a especialistas para migraciones complejas.

### ¿Puedo mantener el ERP antiguo y el nuevo en paralelo?

Sí, durante un período de transición. Algunas empresas mantienen ambos sistemas hasta verificar que AbanQ funciona correctamente.

### ¿Cuánto tiempo toma una migración?

Depende del volumen y complejidad:
- Pequeña empresa: 1-2 semanas
- Mediana empresa: 1-2 meses
- Grande: 2-6 meses

---

## Desarrollo y Personalización

### ¿Puedo modificar el código de AbanQ?

Sí, es código abierto bajo GPL-2.0. Puedes modificarlo libremente, pero debes mantener la licencia GPL.

### ¿Cómo desarrollo un módulo personalizado?

Opciones:
1. **Scripts Qt**: Para automatizaciones simples
2. **Módulos C++**: Para funcionalidades complejas
3. **Integraciones externas**: Via API/web services

### ¿Necesito saber programar?

Depende del nivel de personalización:
- Configuración básica: No
- Scripts simples: Básicos de JavaScript/Qt
- Desarrollo de módulos: Sí, requiere C++

### ¿Hay documentación para desarrolladores?

El código está disponible en GitHub. Hay comentarios en el código y se acepta colaboración de la comunidad.

---

## Soporte y Comunidad

### ¿Hay soporte profesional disponible?

La comunidad ofrece soporte mediante:
- Issues en GitHub
- Foros comunitarios
- Repositorio de soluciones

Para soporte comercial, contacta a empresas especializadas en AbanQ.

### ¿Cómo reporto un bug?

Ve a [GitHub Issues](https://github.com/falbujer/AbanQ/issues) y crea un nuevo issue con:
- Descripción clara del problema
- Pasos para reproducir
- Versión de AbanQ
- Sistema operativo
- Logs de error (si aplica)

### ¿Cómo sugiero una característica?

Abre un issue en GitHub con la etiqueta "feature request" explicando:
- Qué quieres que haga
- Por qué lo necesitas
- Cómo crees que debería funcionar

### ¿Puedo contribuir al proyecto?

Definitivamente. Las contribuciones son bienvenidas:
- Reportar bugs
- Sugerir mejoras
- Enviar código
- Mejorar documentación
- Traducir a otros idiomas

---

## Licencia y Legal

### ¿Por qué GPL-2.0?

GPL asegura que:
- El software siempre será libre
- Las mejoras benefician a toda la comunidad
- No hay monopolios sobre el código

### ¿Puedo usar AbanQ en mi negocio?

Sí, completamente. La GPL no restringe uso comercial.

### ¿Tengo que compartir mis modificaciones?

Solo si distribuyes el software modificado. Si lo usas internamente, no necesitas compartir cambios.

### ¿Hay garantía o responsabilidad?

No. El software viene "tal cual" sin garantía. Úsalo bajo tu propio riesgo.

---

## Rendimiento y Escalabilidad

### ¿Cuántos usuarios simultáneos soporta AbanQ?

Depende de:
- Hardware del servidor
- Configuración de base de datos
- Complejidad de reportes

Típicamente: 20-50+ usuarios simultáneos con configuración adecuada.

### ¿Qué tan rápido es AbanQ?

La velocidad es:
- **Buena** para PYMES
- **Adecuada** para empresas medianas
- **Puede necesitar optimización** para empresas muy grandes

Se puede optimizar mediante:
- Índices en base de datos
- Hardware más potente
- Optimización de queries

### ¿Cómo mejoro el rendimiento?

Recomendaciones:
1. Usar PostgreSQL en lugar de MySQL
2. Aumentar RAM del servidor
3. Optimizar índices de base de datos
4. Limpiar datos obsoletos
5. Usar SSD en lugar de HDD

---

## Seguridad

### ¿Es seguro usar AbanQ para datos financieros?

Sí. Proporciona:
- Autenticación de usuarios
- Auditoría de cambios
- Control de permisos granular
- Encriptación de datos sensibles (configurable)

Se recomienda complementar con:
- Backups regulares
- Firewall
- VPN para acceso remoto
- Certificados SSL para web

### ¿Cómo protejo la contraseña de administrador?

Buenas prácticas:
- Usa contraseñas fuertes (>12 caracteres)
- Cámbiala regularmente
- No la compartas
- Usa doble autenticación si está disponible

### ¿Debo hacer backups?

Sí, es esencial:
- Diarios para datos críticos
- Semanales para datos estándar
- Mensuales para archivos

---

## Contacto y Recursos

### ¿Dónde encuentro más información?

- 🌐 [Sitio oficial](https://www.abanq.es/)
- 💻 [GitHub](https://github.com/falbujer/AbanQ)
- 📦 [Módulos](https://github.com/falbujer/AbanQ-Modules)

### ¿Hay una comunidad activa?

Sí, en GitHub y foros online. Puedes:
- Hacer preguntas
- Compartir experiencias
- Colaborar en desarrollos

---

*Última actualización: 19 de enero de 2026*

¿No encuentras respuesta a tu pregunta? [Abre un issue](https://github.com/falbujer/AbanQ/issues) en GitHub.
