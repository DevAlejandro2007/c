# ADR-003: Aislar el código dependiente del sistema en la capa `platform/` de Tetris

## Estado
Aceptado

## Fecha
2026-11-09

## Contexto

`tetris` debe funcionar en Linux y Windows. La lectura de teclas sin bloquear, la configuración de la terminal y las pausas cambian según el sistema operativo:

- **Linux**: `termios`, `select()` y `read()`.
- **Windows**: `_kbhit()`, `_getch()`, `Sleep()` y `SetConsoleMode()`.

La lógica del juego (`logica.c`) y el renderizado (`render.c`) no deberían contener código específico de plataforma.

## Decisión

Concentrar todo el código dependiente del sistema en `platform/term.c`, detrás de la interfaz `platform/platform.h`, que declara:

- `configurar_terminal()`
- `restaurar_terminal()`
- `pausa_ms()`
- `tecla_presionada()`
- `leer_tecla()`

`main.c`, `logica.c` y `render.c` consumen solo esa interfaz y no usan `#ifdef` de plataforma.

## Alternativas consideradas

### Repartir `#ifdef _WIN32` por todo el código

- Contras: duplica condiciones en varios módulos y hace difícil seguir la lógica.
- Rechazada.

### Mantener dos versiones completas del juego

- Contras: duplica la lógica y las correcciones deben aplicarse dos veces.
- Rechazada.

### Usar una biblioteca externa (ncurses/PDCurses)

- Pros: entrada y terminal resueltas.
- Contras: añade una dependencia que el proyecto evita; el objetivo es practicar C y la interacción con el sistema.
- Rechazada: el proyecto se mantiene con bibliotecas estándar y APIs del sistema.

## Consecuencias

- Portar el juego a otro sistema implica implementar las cinco funciones de `platform.h`.
- Los módulos de juego permanecen sin `#ifdef`.
- `platform/term.c` es el único archivo que necesita revisarse al cambiar de sistema operativo.
