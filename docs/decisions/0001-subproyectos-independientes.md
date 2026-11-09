# ADR-001: Organizar el repositorio en subproyectos independientes

## Estado
Aceptado

## Fecha
2026-11-09

## Contexto

El repositorio es material de aprendizaje de C y reúne varios programas completos: `admin_personas` (gestión de personas), `tetris` (juego de consola) y `tres_raya` (tres en raya). Cada uno tiene su propio `main()` y su propio conjunto de archivos `.c` y `.h`; no comparten código entre sí.

Se necesita que cada ejemplo se pueda leer, compilar y ejecutar de forma aislada, y que añadir un ejercicio nuevo no obligue a modificar los existentes. El repositorio no dispone de un sistema de construcción raíz: `tetris` incluye `Makefile` y `build.bat`, mientras que `admin_personas` y `tres_raya` se compilan con `gcc` directamente.

## Decisión

Cada carpeta de primer nivel es un subproyecto autónomo:

- contiene su propio `main()` y sus propias fuentes;
- declara sus interfaces en cabeceras locales (ver ADR-002);
- se compila y ejecuta desde su propia carpeta, o indicando sus rutas;
- incluye su propio sistema de construcción cuando aporta valor (`tetris`: `Makefile` y `build.bat`).

El repositorio no define un ejecutable ni un `Makefile` unificado.

## Alternativas consideradas

### Un único programa con un menú que despache a cada juego

- Pros: un solo ejecutable.
- Contras: acopla ejercicios que hoy son independientes y obliga a recompilar todo para probar uno.
- Rechazada: cada subproyecto debe poder estudiarse y ejecutarse por separado.

### Un workspace con CMake en la raíz

- Pros: construcción centralizada y portable.
- Contras: introduce una herramienta y una configuración adicionales que hoy no se usan; los subproyectos son pequeños y no comparten dependencias.
- Rechazada: no compensa para el tamaño actual del repositorio.

### Una biblioteca común para código compartido

- Pros: evita duplicación futura.
- Contras: en la versión actual no hay módulos compartidos reales; cada programa usa sus propias estructuras.
- Rechazada por ahora: se puede reconsiderar cuando exista código verdaderamente común.

## Consecuencias

- Para compilar hay que entrar en la carpeta del subproyecto o pasar sus rutas a `gcc`.
- Los binarios generados no se versionan (ver `.gitignore`).
- Las convenciones compartidas (cabeceras, compilación) se documentan en este directorio y en el README raíz.
- Un subproyecto nuevo se crea como una carpeta independiente y no modifica a los demás.
