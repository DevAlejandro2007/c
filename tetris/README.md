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
- [Records Persistentes](#records-persistentes)
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
- **Records persistentes**: Top 10 con nombre y puntaje guardado en `puntajes.txt`.
- **Sistema de puntuación**: Bonificación por limpiar múltiples líneas a la vez.
- **Previsualización de pieza siguiente**: Muestra la próxima pieza antes de aparecer.
- **Sin dependencias externas**: Únicamente librerías estándar de C.

## Arquitectura del Proyecto

```text
tetris/
├── main.c           # Flujo del programa, bucle del juego y decisiones de pausa/game over
├── logica.c         # Lógica pura: colisiones, rotaciones, limpieza de líneas y generación de piezas
├── render.c         # Renderizado del tablero, pieza actual y siguiente pieza
├── ui.c             # Menús interactivos, instrucciones, pausa, game over y records
├── ui.h             # Interfaz de la capa de UI
├── puntajes.c       # Persistencia de records en texto plano
├── puntajes.h       # Interfaz de la capa de records
├── tetris.h         # Definiciones, estructuras, constantes y prototipos comunes
├── platform/
│   ├── platform.h   # Interfaz de la capa de abstracción del sistema
│   └── term.c       # Implementación multiplataforma (terminal, input, timing, pantalla alterna, archivos)
├── Makefile         # Build para Linux/UNIX
└── build.bat        # Build para Windows (CMD/PowerShell)
```

### Responsabilidades

| Archivo | Descripción |
| --- | --- |
| `main.c` | Controla el flujo entre menú, juego, pausa y records; ejecuta el bucle principal. |
| `logica.c` | Todo lo algorítmico: `colision()`, `rotar_pieza()`, `limpiar_lineas()`, `fijar_pieza()` y `generar_pieza_aleatoria()`. |
| `render.c` | Pinta el tablero, el HUD y la pieza siguiente. No conoce menús. |
| `ui.c` / `ui.h` | Menú principal, instrucciones, pausa, game over y records. Solo depende de `platform/`. |
| `puntajes.c` / `puntajes.h` | Carga, ordena, sanea y guarda el top 10 en `puntajes.txt`. No conoce la UI. |
| `platform/term.c` | Única capa que conoce el SO. Aquí vive todo lo específico de Windows o Linux. |

## Diseño Multiplataforma

Gracias a la capa `platform/`, `main.c`, `logica.c`, `render.c`, `ui.c` y `puntajes.c` **no tienen ni un solo `#ifdef`**. Únicamente consumen la interfaz de `platform/platform.h`.

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
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c logica.c render.c ui.c puntajes.c platform/term.c -o tetris.exe
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
|   Records                            |
|   Instrucciones                      |
|   Salir                              |
+--------------------------------------+

  Flechas o W/S para mover | Enter selecciona | Q sale
```

- **Jugar** - Inicia una nueva partida.
- **Records** - Muestra el top 10 guardado en `puntajes.txt`. Vuelve con `Enter` o `Esc`.
- **Instrucciones** - Muestra controles, objetivo y puntuación. Vuelve con `Enter` o `Esc`.
- **Salir** - Cierra el programa restaurando la terminal y la pantalla original.

Durante la partida, `Esc` o `P` abre el menú de pausa con `Continuar`, `Reiniciar` y `Volver al menu`. Al terminar, el game over permite `Reintentar` o volver al menú.

## Records Persistentes

El juego guarda los **10 mejores puntajes** en un archivo de texto plano llamado `puntajes.txt`, en el directorio desde el que se ejecuta el juego.

Solo se pide el nombre cuando el puntaje entra en el top 10. Si el nombre se deja vacío o se pulsa `Esc`, se guarda como `Anonymous`. El nombre se recorta, se limita a 20 caracteres ASCII, se le quitan los caracteres de control y se sustituye `|` por `-`.

### Formato del archivo

```text
# tetris-scores v1
# nombre|puntaje
Ana|1200
Anonymous|900
```

Las líneas que empiezan por `#` son comentarios. Los registros se ordenan de mayor a menor puntaje y, si hay empate, por nombre alfabético.

### Reglas

- Un puntaje `0` o negativo no se guarda.
- Si hay menos de 10 records, cualquier puntaje positivo entra.
- Si ya hay 10, hay que superar al décimo para entrar.
- Un archivo inexistente se trata como una tabla vacía.
- Las líneas corruptas o con puntajes inválidos se ignoran.
- Si hay un error real de lectura o escritura, la UI muestra un aviso.

### Guardado seguro

El guardado se hace primero en `puntajes.txt.tmp` y luego se reemplaza el archivo original. En Linux/POSIX el reemplazo es atómico con `rename()`. En Windows, si `rename()` no puede sobrescribir, se elimina el destino y se reintenta; esa diferencia vive en `platform/term.c`.

Para borrar todos los records, elimina `puntajes.txt`. El archivo está ignorado por Git porque es dato local de cada jugador.

## Controles

### En el menú

| Tecla | Acción |
| --- | --- |
| `↑` / `W` | Mover la selección hacia arriba |
| `↓` / `S` | Mover la selección hacia abajo |
| `Enter` | Seleccionar la opción |
| `Q` / `Esc` | Salir o cancelar |

### Al escribir un nombre

| Tecla | Acción |
| --- | --- |
| Caracteres imprimibles | Añadir al nombre (máximo 20) |
| `Backspace` | Borrar el último carácter |
| `Enter` | Confirmar el nombre |
| `Esc` | Cancelar y usar `Anonymous` |

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
- **Teclas normalizadas**: `platform/term.c` traduce flechas, `Enter`, `Esc` y `Backspace` a un enum común; la UI no sabe si corre en Linux o Windows.
- **Menús reutilizables**: `ui.c` expone un único `menu_seleccion()` interno para el menú principal, la pausa y el game over.
- **Persistencia aislada**: `puntajes.c` es el único módulo que conoce el formato de `puntajes.txt`; `main.c` decide cuándo guardar y `ui.c` solo muestra la tabla.
- **Guardado seguro**: se escribe un archivo temporal y se reemplaza el original mediante `reemplazar_archivo()` en `platform/`.
- **Pantalla alterna**: La UI usa el buffer alterno de la terminal y restaura el contenido original al salir.
- **Limpieza segura**: `atexit(restaurar_terminal)` y los manejadores de `SIGINT`/`SIGTERM` (o `SetConsoleCtrlHandler` en Windows) restauran cursor, pantalla y modo de terminal.

## Mejoras a Futuro

- [ ] **Ghost Piece** - Silueta de dónde caerá la pieza
- [ ] **Hold Piece** - Guardar pieza para usar más adelante
- [ ] **Hard Drop** - Caída instantánea con `Espacio`
- [ ] **Sistema SRS** - Super Rotation System
- [ ] **Niveles y velocidad dinámica**
- [ ] **Manejo preciso con `clock_gettime()`**
- [ ] **Detección de tamaño de terminal** - Avisar si el tablero no cabe

## Autor

**DevAlejandro2007**  

*Proyecto con fines educativos y de portfolio, con énfasis en portabilidad, abstracción y programación a bajo nivel en C.*
