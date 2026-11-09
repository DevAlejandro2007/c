# Tetris en C (Multiplataforma)

![Language](https://img.shields.io/badge/Lenguaje-C-blue?logo=c)
![Platform](https://img.shields.io/badge/Plataformas-Linux%20%7C%20Windows-brightgreen)
![Build](https://img.shields.io/badge/Build-Make%20%7C%20Batch-orange)

## Índice

- [Descripción](#descripción)
- [Características](#características)
- [Arquitectura del Proyecto](#arquitectura-del-proyecto)
- [Diseño Multiplataforma](#diseño-multiplataforma)
- [Requisitos](#requisitos)
- [Compilación y Ejecución](#compilación-y-ejecución)
- [Menú Principal](#menú-principal)
- [Controles](#controles)
- [Sistema de Puntuación](#sistema-de-puntuación)
- [Detalles Técnicos](#detalles-técnicos)
- [Mejoras a Futuro](#mejoras-a-futuro)
- [Autor](#autor)

## Descripción

Implementación del clásico **Tetris** escrita 100% en C con un fuerte enfoque a **portabilidad, abstracción y bajo nivel**. 

El objetivo va más allá de hacer un juego: demostrar un diseño limpio separando la lógica del juego de todo lo dependiente del sistema operativo. De ahí nace la capa `platform/`, pensada con mentalidad orientada a **Sistemas/Kernel**.

## Características

- **Menú principal**: Interfaz clara con opciones `Jugar`, `Instrucciones` y `Salir`.
- **Multiplataforma**: Funciona tanto en Linux como en Windows sin modificar la lógica del juego.
- **Interfaz TUI**: Renderizado puramente en terminal con códigos ANSI.
- **Arquitectura modular**: Separación clara entre lógica, renderizado e interfaz con el sistema.
- **Entrada no bloqueante**: Permite mover/rotar la pieza sin pausar el juego.
- **Sistema de puntuación**: Bonificación por limpiar múltiples líneas a la vez.
- **Previsualización de pieza siguiente**: Muestra la próxima pieza antes de aparecer.
- **Game Over funcional**: Detecta cuando no hay espacio para la nueva pieza y permite volver al menú.
- **Sin dependencias externas**: Únicamente librerías estándar de C.

## Arquitectura del Proyecto

```text
tetris/
├── main.c           # Menú principal, flujo del programa e inicio del juego
├── logica.c         # Lógica pura: colisiones, rotaciones, limpieza de líneas, generación de piezas, game over
├── render.c         # Renderizado del tablero, pieza actual, siguiente pieza, menú e instrucciones
├── tetris.h         # Definiciones, estructuras, constantes y prototipos comunes
├── platform/
│   ├── platform.h   # Interfaz de la capa de abstracción del sistema
│   └── term.c       # Implementación multiplataforma (terminal, input, timing)
├── Makefile         # Build para Linux/UNIX
└── build.bat        # Build para Windows (CMD/PowerShell)
```

### Responsabilidades

| Archivo | Descripción |
| --- | --- |
| `main.c` | Controla el menú, llama a `iniciar_juego()` y gestiona el flujo entre pantallas. |
| `logica.c` | Todo lo algorítmico: `colision()`, `rotar_pieza()`, `limpiar_lineas()`, `fijar_pieza()`, `generar_pieza_aleatoria()`, `game_over()`. |
| `render.c` | Se encarga de pintar tablero, HUD, `mostrar_menu()` y `mostrar_instrucciones()`. |
| `platform/term.c` | Única capa que conoce el SO. Aquí vive todo lo específico de Windows o Linux. |

## Diseño Multiplataforma

Gracias a la capa `platform/`, `main.c`, `logica.c` y `render.c` **no tienen ni un solo `#ifdef`**. Únicamente consumen la interfaz de `platform/platform.h`.

### Linux (`!_WIN32`)

- **Configuración de terminal**: `termios` para poner `stdin` en modo *raw* (sin esperar `Enter`, sin eco).
- **Entrada no bloqueante**: `select()` con `fd_set` para detectar teclas sin detener el bucle.
- **Lectura de tecla**: `read()` sobre `STDIN_FILENO`.
- **Temporizador**: `select()` con `timeval` como delay entre iteraciones.

### Windows (`_WIN32`)

- **Configuración de terminal**: Habilita `ENABLE_VIRTUAL_TERMINAL_PROCESSING` con `SetConsoleMode()` para usar códigos ANSI.
- **Entrada no bloqueante**: `_kbhit()` para detectar teclas y `_getch()` para leerlas.
- **Temporizador**: `Sleep()` de `windows.h`.

## Requisitos

- **Compilador**: `gcc` con soporte para C11
- **Linux**: `make`, `termios`, `sys/select.h` (disponibles por defecto)
- **Windows**: MinGW/MSYS2 o compilador compatible

## Compilación y Ejecución

### En Linux

```bash
cd ~/Documentos/c/c/tetris
make
./tetris
```

Para limpiar:

```bash
make clean
```

### En Windows

```bat
build.bat
tetris.exe
```

O directamente:

```bat
gcc -std=c11 -Wall -Wextra -O2 main.c logica.c render.c platform/term.c -o tetris.exe
tetris.exe
```

> En Linux, usa `./tetris` ya que el directorio actual no está en tu `$PATH`.

## Menú Principal

Al iniciar el juego aparecerá el siguiente menú:

```text
======= TETRIS =======
======================

      1. JUGAR
      2. INSTRUCCIONES
      3. SALIR

======================
  Selecciona una opcion:
```

- **1. JUGAR** - Inicia una nueva partida
- **2. INSTRUCCIONES** - Muestra controles, objetivo y puntuación. Vuelve al menú con cualquier tecla
- **3. SALIR** - Cierra el programa restaurando la terminal correctamente

## Controles

| Tecla | Acción |
| --- | --- |
| `A` / `a` | Mover a la izquierda |
| `D` / `d` | Mover a la derecha |
| `S` / `s` | Caída rápida (*soft drop*) |
| `W` / `w` | Rotar pieza |

## Sistema de Puntuación

| Líneas limpiadas | Puntuación |
| --- | --- |
| 1 | +100 |
| 2 | +300 |
| 3 | +500 |
| 4 (*Tetris*) | +800 |

## Detalles Técnicos

- **Estructuras**: `PiezaActual` contiene tipo, posición `(x,y)` y matriz `forma[4][4]`.
- **Tablero**: Matriz `int rango[FIL][COL]` (`1` = bloque fijo, `0` = vacío).
- **Rotación**: Rotación matricial 90° horario con validación por colisión antes de aplicar.
- **Limpieza de líneas**: Al completar una fila, se desplazan las superiores hacia abajo.
- **Game Loop**: Caída automática por tiempo + procesamiento de input no bloqueante.
- **Renderizado**: Uso de `\033[H` para reposicionar cursor sin limpiar pantalla completa constantemente.
- **Limpieza segura**: `atexit(restaurar_terminal)` restaura la terminal al salir.

## Mejoras a Futuro

- [ ] **Ghost Piece** - Silueta de dónde caerá la pieza
- [ ] **Hold Piece** - Guardar pieza para usar más adelante
- [ ] **Sistema SRS** - Super Rotation System
- [ ] **Niveles y velocidad dinámica**
- [ ] **High Score persistente** - Guardar récord en archivo
- [ ] **Manejo preciso con `clock_gettime()`**
- [ ] **Soporte a flechas con secuencias ANSI**

## Autor

**DevAlejandro2007**  

*Proyecto con fines educativos y de portfolio, con énfasis en portabilidad, abstracción y programación a bajo nivel en C.*
