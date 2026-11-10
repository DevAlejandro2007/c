# ADR-004: Construir una TUI propia con ANSI en lugar de usar ncurses

## Estado
Aceptado

## Fecha
2026-11-09

## Contexto

El menú original de `tetris` era numérico: el usuario escribía `1`, `2` o `3` y pulsaba `Enter`. Funciona, pero no se siente como una aplicación de terminal y no ejercita el control de la terminal, que es parte del objetivo de aprendizaje del repositorio (sistemas operativos, entrada/salida y programación a bajo nivel).

Se quiere una interfaz navegable con flechas, selección resaltada, `Enter` para confirmar y `Esc`/`Q` para salir. Además, el juego debe seguir siendo multiplataforma (Linux y Windows) y mantener la separación entre lógica, renderizado, UI y sistema operativo.

## Decisión

Construir una TUI propia sobre la capa `platform/`, usando únicamente:

- códigos ANSI para limpiar pantalla, mover el cursor, resaltar y usar la pantalla alterna;
- `termios` + `select` en Linux y `conio` + `windows.h` en Windows para la entrada;
- un enum de teclas normalizado (`TECLA_ARRIBA`, `TECLA_ABAJO`, `TECLA_ENTER`, `TECLA_ESCAPE`, etc.) en `platform/platform.h`;
- un módulo `ui.c`/`ui.h` con un menú reutilizable para el menú principal, la pausa y el game over.

Los menús usan bordes ASCII (`+`, `-`, `|`) y texto sin acentos para evitar problemas de página de códigos. El elemento seleccionado se resalta con video inverso (`\033[7m`).

## Alternativas consideradas

### Mantener el menú numérico

- Pros: menos código y menos casos borde.
- Contras: no es una TUI, no ejercita el control de terminal y la experiencia es peor.
- Rechazada: el objetivo del subproyecto es practicar entrada/salida y portabilidad.

### Usar ncurses/PDCurses

- Pros: menús, ventanas, colores y entrada resueltos por la biblioteca.
- Contras: añade una dependencia externa, oculta los detalles de `termios`/consola y obliga a instalar la biblioteca en Windows.
- Rechazada: el repositorio evita dependencias externas y el objetivo es aprender cómo funciona la terminal por dentro.

### Usar una biblioteca TUI moderna (notcurses, FTXUI, etc.)

- Pros: interfaces muy completas.
- Contras: mayor complejidad, dependencias y menos valor didáctico para este proyecto.
- Rechazada por ahora.

## Consecuencias

- Hay que parsear las flechas a mano: secuencias ANSI en Linux y códigos extendidos en Windows.
- Hay que distinguir `Esc` de una flecha con un timeout corto (`ESPERA_SECUENCIA_MS`).
- La UI debe restaurar cursor, pantalla alterna y modo de terminal al salir, incluso con `Ctrl+C`.
- El código de `ui.c` permanece portable: solo depende de `platform/platform.h`.
- Si en el futuro se necesita una interfaz mucho más compleja, se puede reconsiderar ncurses sin cambiar la lógica del juego.
