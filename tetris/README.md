# Tetris

Tetris de consola para Linux y Windows. Necesita un compilador de C y una
terminal interactiva compatible con secuencias ANSI.

## Linux

Instala GCC y Make con el gestor de paquetes de tu distribución. Desde este
directorio, compila y ejecuta:

```sh
make
./tetris
```

Para eliminar el ejecutable:

```sh
make clean
```

## Windows

Instala GCC para Windows, por ejemplo mediante MSYS2 (entorno UCRT64), y
ejecuta `build.bat` desde este directorio. Inicia el juego en Windows 10/11
desde una terminal con soporte para secuencias ANSI:

```bat
tetris.exe
```

## Controles

- `A`: mover a la izquierda
- `D`: mover a la derecha
- `S`: bajar
- `W`: rotar
