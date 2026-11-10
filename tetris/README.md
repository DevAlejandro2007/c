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

- **Menú principal interactivo**: Navegación con flechas o `W/S`, selección con `Enter` y salida con `Q`/`Esc`.
- **Multiplataforma**: Funciona tanto en Linux como en Windows sin modificar la lógica del juego.
- **Interfaz TUI**: Renderizado puramente en terminal con códigos ANSI y menús ASCII.
- **Arquitectura modular**: Separación clara entre lógica, renderizado, UI e interfaz con el sistema.
- **Entrada no bloqueante**: Permite mover/rotar la pieza sin pausar el juego.
- **Pausa**: `Esc` o `P` abre un menú con `Continuar`, `Reiniciar` y `Volver al menu`.
- **Game Over interactivo**: Detecta el final de la partida y permite reintentar o volver al menú.
- **Sistema de puntuación**: Bonificación por limpiar múltiples líneas a la vez.
- **Previsualización de pieza siguiente**: Muestra la próxima pieza antes de aparecer.
- **Sin dependencias externas**: Únicamente librerías estándar de C.

## Arquitectura del Proyecto

```text
tetris/
├── main.c           # Flujo del programa, bucle del juego y decisiones de pausa/game over
├── logica.c         # Lógica pura: colisiones, rotaciones, limpieza de líneas y generación de piezas
├── render.c         # Renderizado del tablero, pieza actual y siguiente pieza
├── ui.c             # Menús interactivos, instrucciones, pausa y game over
├── ui.h             # Interfaz de la capa de UI
├── tetris.h         # Definiciones, estructuras, constantes y prototipos comunes
├── platform/
│   ├── platform.h   # Interfaz de la capa de abstracción del sistema
│   └── term.c       # Implementación multiplataforma (terminal, input, timing, pantalla alterna)
├── Makefile         # Build para Linux/UNIX
└── build.bat        # Build para Windows (CMD/PowerShell)
```

### Responsabilidades

| Archivo | Descripción |
| --- | --- |
| `main.c` | Controla el flujo entre menú, juego y pausa; ejecuta el bucle principal. |
| `logica.c` | Todo lo algorítmico: `colision()`, `rotar_pieza()`, `limpiar_lineas()`, `fijar_pieza()` y `generar_pieza_aleatoria()`. |
| `render.c` | Pinta el tablero, el HUD y la pieza siguiente. No conoce menús. |
| `ui.c` / `ui.h` | Menú principal, instrucciones, pausa y game over. Solo depende de `platform/`. |
| `platform/term.c` | Única capa que conoce el SO. Aquí vive todo lo específico de Windows o Linux. |

## Diseño Multiplataforma

Gracias a la capa `platform/`, `main.c`, `logica.c`, `render.c` y `ui.c` **no tienen ni un solo `#ifdef`**. Únicamente consumen la interfaz de `platform/platform.h`.

### Linux (`!_WIN32`)

- **Configuración de terminal**: `termios` para poner `stdin` en modo *raw* (sin esperar `Enter`, sin eco).
- **Entrada no bloqueante**: `select()` con `fd_set` para detectar teclas sin detener el bucle.
- **Lectura de tecla**: `read()` sobre `STDIN_FILENO`.
- **Flechas**: se parsean las secuencias ANSI (`ESC [ A/B/C/D`) con un timeout corto para distinguir `Esc` de una flecha.
- **Temporizador**: `select()` con `timeval` como delay entre iteraciones.
- **Señales**: `SIGINT`/`SIGTERM` restauran la terminal antes de salir.

### Windows (`_WIN32`)

- **Configuración de terminal**: Habilita `ENABLE_VIRTUAL_TERMINAL_PROCESSING` con `SetConsoleMode()` para usar códigos ANSI.
- **Entrada no bloqueante**: `_kbhit()` para detectar teclas y `_getch()` para leerlas.
- **Flechas**: se interpretan los códigos extendidos (`0`/`224` + `72/80/75/77`).
- **Temporizador**: `Sleep()` de `windows.h`.
- **Señales**: `SetConsoleCtrlHandler()` restaura la terminal ante `Ctrl+C`.

### Pantalla alterna

La UI entra en la pantalla alterna (`\033[?1049h`) y la abandona al salir (`\033[?1049l`). Así el menú y el juego no ensucian el historial de la terminal, igual que hacen `vim` o `less`.

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
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c logica.c render.c ui.c platform/term.c -o tetris.exe
tetris.exe
```

> En Linux, usa `./tetris` ya que el directorio actual no está en tu `$PATH`.

## Menú Principal

Al iniciar el juego aparece un menú navegable con el elemento seleccionado resaltado:

```text
+--------------------------------------+
|             T E T R I S              |
+--------------------------------------+
| > Jugar                              |
|   Instrucciones                      |
|   Salir                              |
+--------------------------------------+

  Flechas o W/S para mover | Enter selecciona | Q sale
```

- **Jugar** - Inicia una nueva partida.
- **Instrucciones** - Muestra controles, objetivo y puntuación. Vuelve con `Enter` o `Esc`.
- **Salir** - Cierra el programa restaurando la terminal y la pantalla original.

Durante la partida, `Esc` o `P` abre el menú de pausa con `Continuar`, `Reiniciar` y `Volver al menu`. Al terminar, el game over permite `Reintentar` o volver al menú.

## Controles

### En el menú

| Tecla | Acción |
| --- | --- |
| `↑` / `W` | Mover la selección hacia arriba |
| `↓` / `S` | Mover la selección hacia abajo |
| `Enter` | Seleccionar la opción |
| `Q` / `Esc` | Salir o cancelar |

### En partida

| Tecla | Acción |
| --- | --- |
| `←` / `A` | Mover a la izquierda |
| `→` / `D` | Mover a la derecha |
| `↓` / `S` | Caída rápida (*soft drop*) |
| `↑` / `W` | Rotar pieza |
| `Esc` / `P` | Pausa |

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
- **Teclas normalizadas**: `platform/term.c` traduce flechas, `Enter` y `Esc` a un enum común; la UI no sabe si corre en Linux o Windows.
- **Menús reutilizables**: `ui.c` expone un único `menu_seleccion()` interno para el menú principal, la pausa y el game over.
- **Pantalla alterna**: La UI usa el buffer alterno de la terminal y restaura el contenido original al salir.
- **Limpieza segura**: `atexit(restaurar_terminal)` y los manejadores de `SIGINT`/`SIGTERM` (o `SetConsoleCtrlHandler` en Windows) restauran cursor, pantalla y modo de terminal.

## Mejoras a Futuro

- [ ] **Ghost Piece** - Silueta de dónde caerá la pieza
- [ ] **Hold Piece** - Guardar pieza para usar más adelante
- [ ] **Hard Drop** - Caída instantánea con `Espacio`
- [ ] **Sistema SRS** - Super Rotation System
- [ ] **Niveles y velocidad dinámica**
- [ ] **High Score persistente** - Guardar récord en archivo
- [ ] **Manejo preciso con `clock_gettime()`**
- [ ] **Detección de tamaño de terminal** - Avisar si el tablero no cabe

## Autor

**DevAlejandro2007**  

*Proyecto con fines educativos y de portfolio, con énfasis en portabilidad, abstracción y programación a bajo nivel en C.*
