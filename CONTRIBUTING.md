# Guía de Contribución

¡Gracias por tu interés en contribuir a AbanQ! Este proyecto vive gracias a la comunidad de desarrolladores y usuarios que lo mantienen vivo y lo mejoran constantemente.

## Cómo Contribuir

Hay muchas formas de contribuir a AbanQ, no todas requieren programación:

### 1. Reportar Bugs

Si encuentras un bug, es muy valioso para nosotros:

#### Pasos para reportar:
1. Ve a [GitHub Issues](https://github.com/falbujer/AbanQ/issues)
2. Haz clic en "New Issue"
3. Elige "Bug report"
4. Completa el formulario con:

```markdown
## Descripción del problema
[Descripción clara del bug]

## Pasos para reproducir
1. Primer paso
2. Segundo paso
3. Etc.

## Comportamiento esperado
[Qué debería pasar]

## Comportamiento actual
[Qué pasa realmente]

## Sistema
- SO: [Windows/Linux/macOS]
- Versión AbanQ: [ej: 2.5.0]
- Navegador/Cliente: [ej: Qt Client]
- Logs: [adjunta si es posible]
```

#### Consejos para buenos reportes:
- Sé específico y detallado
- Incluye ejemplos de código si es relevant
- Adjunta capturas de pantalla
- Indica la versión que usas
- Menciona qué base de datos utilizas

### 2. Sugerir Mejoras

¿Tienes una idea para mejorar AbanQ?

#### Cómo sugerir:
1. Ve a [GitHub Issues](https://github.com/falbujer/AbanQ/issues)
2. Crea un nuevo issue con etiqueta "feature request"
3. Describe:

```markdown
## Descripción de la mejora
[Qué quieres que haga]

## Beneficios
[Por qué sería útil]

## Implementación sugerida
[Cómo crees que debería hacerse]

## Alternativas consideradas
[Otras formas de resolver esto]
```

#### Antes de sugerir:
- Verifica que no exista ya una sugerencia similar
- Considera el impacto en el sistema
- Piensa en los casos de uso

### 3. Mejorar Documentación

La documentación es crucial y siempre necesita mejoras:

#### Cómo contribuir:
1. Edita los archivos `.md` en `/docs`
2. Agrega ejemplos, aclaraciones o traducciones
3. Crea un Pull Request

#### Áreas que necesitan help:
- Tutoriales paso a paso
- Videos de demostración
- Traducción a otros idiomas
- Ejemplos de uso
- Correcciones de ortografía

### 4. Traducir a Otros Idiomas

AbanQ puede alcanzar a más usuarios en otros idiomas:

#### Idiomas buscados:
- Inglés (mejoras)
- Francés
- Alemán
- Portugués
- Italiano
- Otros

#### Cómo colaborar:
1. Contacta al mantener: falbujer
2. Traduce archivos `.md` con sufijo de idioma
3. Ejemplo: `README_en.md`, `README_fr.md`

### 5. Contribuir Código

Si eres desarrollador, ¡es el momento de brillar!

#### Configurar el ambiente:

**Requisitos:**
```bash
# Ubuntu/Debian
sudo apt-get install build-essential git qt4-dev-tools libqt4-dev

# Fedora/RHEL
sudo dnf install gcc-c++ git qt4-devel

# macOS
brew install qt4
```

**Clonar y compilar:**
```bash
git clone https://github.com/falbujer/AbanQ.git
cd AbanQ
./engine/build.sh
```

#### Flujo de trabajo:

1. **Fork el repositorio**
   ```bash
   # En GitHub, haz clic en "Fork"
   ```

2. **Clona tu fork**
   ```bash
   git clone https://github.com/TU_USUARIO/AbanQ.git
   cd AbanQ
   ```

3. **Crea una rama para tu feature**
   ```bash
   git checkout -b feature/mi-nueva-caracteristica
   ```

4. **Haz cambios y commits**
   ```bash
   git add .
   git commit -m "Descripción clara de los cambios"
   ```

5. **Push a tu fork**
   ```bash
   git push origin feature/mi-nueva-caracteristica
   ```

6. **Crea un Pull Request**
   - Ve a GitHub
   - Haz clic en "New Pull Request"
   - Describe tus cambios claramente

#### Guía de Commits

- Mensajes claros y descriptivos
- En inglés preferentemente
- Ejemplo: "Add validation to invoice module"
- No: "fixed stuff" ❌

#### Guía de Código

Sigue el estilo existente:
- Indentación: 4 espacios
- Nombres: camelCase para variables, PascalCase para clases
- Comentarios en inglés
- Documenta código complejo

Ejemplo:
```cpp
// Calculate invoice total with tax
double calculateTotal(const Invoice& invoice) {
    double subtotal = invoice.getSubtotal();
    double tax = subtotal * invoice.getTaxRate();
    return subtotal + tax;
}
```

#### Testing

Antes de hacer Pull Request:
1. Compila sin errores
2. Prueba tu cambio completamente
3. Verifica que no rompe funcionalidad existente
4. Incluye casos de uso en la descripción

### 6. Revisar Pull Requests

Ayuda revisando contribuciones de otros:

- Prueba el código
- Sugiere mejoras
- Reporta bugs encontrados
- Valida que sigue las guías

## Código de Conducta

Para mantener un ambiente positivo:

- ✅ Sé respetuoso y amable
- ✅ Acepta críticas constructivas
- ✅ Ayuda a otros
- ✅ Da crédito
- ❌ No hagas spam
- ❌ No insultes
- ❌ No discrimines

## Licencia

Al contribuir a AbanQ, aceptas que tu código sea publicado bajo licencia GPL-2.0, permitiendo que sea libre y reutilizable por otros.

## Proceso de Review

1. **Validación automática**: GitHub Actions verifica el código
2. **Revisión manual**: El mantener o colaboradores revisan
3. **Feedback**: Se piden cambios si es necesario
4. **Merge**: Una vez aprobado, se integra el cambio

## Prioridades de Contribución

### Alta prioridad
- Bug fixes críticos
- Mejoras de seguridad
- Optimizaciones de performance

### Media prioridad
- Nuevas características útiles
- Mejora de documentación
- Traducciones
- Mejora de UX

### Baja prioridad
- Cambios estéticos menores
- Comentarios adicionales
- Reorganización de código

## Tips para Contribuciones Exitosas

1. **Comunica primero**: Abre un issue antes de trabajar en cambios grandes
2. **Pequeñas contribuciones**: Mejor 5 PRs pequeños que 1 enorme
3. **Tests**: Incluye pruebas para tu código
4. **Documentación**: Documenta cambios y nuevas características
5. **Paciencia**: El review puede tomar tiempo

## Áreas donde se buscan contribuciones

### Backend (C++)
- Optimizaciones de performance
- Nuevos módulos
- Mejora de estabilidad
- Soporte para nuevas BD

### Frontend (Qt)
- Mejora de UX
- Nuevos controles
- Responsividad
- Temas

### Documentación
- Guías de usuario
- Tutoriales
- API documentation
- Comentarios en código

### Testing
- Test cases
- Automatización
- CI/CD improvements
- Reportes de calidad

### Community
- Responder preguntas
- Crear tutoriales
- Organizar eventos
- Escribir artículos

## Recursos Útiles

- 📖 [Documentación del proyecto](../README.md)
- 🏗️ [Arquitectura técnica](ARQUITECTURA.md)
- 📚 [Características](CARACTERISTICAS.md)
- ❓ [FAQ](FAQ.md)
- 💻 [Código fuente en GitHub](https://github.com/falbujer/AbanQ)

## Contacto

- **Issues**: [GitHub Issues](https://github.com/falbujer/AbanQ/issues)
- **Discussiones**: [GitHub Discussions](https://github.com/falbujer/AbanQ/discussions)
- **Mantainer**: [@falbujer](https://github.com/falbujer)

## Reconocimiento

Todos los contribuidores son reconocidos en:
- Lista de contribuidores de GitHub
- Página de créditos (cuando exista)
- Changelog del proyecto

¡Tu contribución, sin importar el tamaño, es valiosa para nosotros!

---

*Última actualización: 19 de enero de 2026*

**¡Gracias por contribuir a AbanQ! 🎉**
