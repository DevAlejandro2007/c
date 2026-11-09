# ADR-002: Declarar cada subproyecto con cabeceras locales `.h`

## Estado
Aceptado

## Fecha
2026-11-09

## Contexto

Los subproyectos necesitan compartir estructuras, constantes y prototipos entre sus archivos `.c`:

- `tetris` usa `tetris.h` para `FIL`, `COL`, `PiezaActual` y los prototipos de `logica.c`, `render.c` y `platform/`.
- `admin_personas` usa `persona.h` para `struct Persona` y sus funciones.
- `tres_raya` usa `juego.h` para el tablero y las funciones del juego.

## Decisión

Cada subproyecto mantiene una cabecera propia (o una por módulo) que:

- usa guardas de inclusión (`#ifndef NOMBRE_H`);
- declara estructuras, constantes y prototipos;
- centraliza las bibliotecas estándar que necesitan sus módulos.

Las implementaciones viven en archivos `.c` que incluyen esa cabecera. No existe una cabecera global que mezcle subproyectos.

## Alternativas consideradas

### Duplicar las declaraciones en cada `.c`

- Contras: riesgo de que las firmas se desincronicen.
- Rechazada.

### Una única cabecera global para todo el repositorio

- Contras: acopla subproyectos independientes y obliga a recompilar más de lo necesario.
- Rechazada.

### Cabeceras generadas o un sistema de construcción avanzado

- Contras: complejidad innecesaria para programas de un único ejecutable.
- Rechazada.

## Consecuencias

- Al añadir o cambiar una función pública hay que actualizar el `.h` correspondiente.
- Las guardas de inclusión evitan declaraciones duplicadas.
- Los módulos de un subproyecto dependen de su cabecera, no de las de otros subproyectos.
