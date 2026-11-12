# Aprendizaje del lenguaje C

> Proyecto: una serie de subproyectos con el objetivo de aprender C.

![C](https://img.shields.io/badge/lenguaje-C-blue?logo=c)
![gcc](https://img.shields.io/badge/gcc-16.1.0-brightgreen?logo=gnu)
![Git](https://img.shields.io/badge/Git-orange?logo=git&logoColor=white)
![GitHub](https://img.shields.io/badge/GitHub-grey?logo=github&logoColor=white)

## Tabla de contenidos

- [Descripción](#descripción)
- [Características](#características)
- [Subproyectos](#subproyectos)
- [Tecnologías](#tecnologías)
- [Requisitos previos](#requisitos-previos)
- [Instalación](#instalación)
- [Compilación y ejecución](#compilación-y-ejecución)
- [Arquitectura y decisiones](#arquitectura-y-decisiones)
- [Cómo contribuir](#cómo-contribuir)

## Descripción

En este repositorio se recogen distintos programas escritos en C, desde lo básico hasta lo avanzado. Cada carpeta es un subproyecto independiente con su propio `main()` y se ejecuta en terminal o PowerShell.

## Características

- Solo lenguaje C.
- Diseñado para estudiantes en español.
- Se utilizan archivos `.h` para declarar funciones, estructuras y bibliotecas.
- Cada subproyecto se compila y ejecuta de forma independiente.

## Subproyectos

| Carpeta | Descripción | Ejecutable |
| --- | --- | --- |
| [`admin_personas`](admin_personas) | Gestión de personas en memoria: agregar, editar, ver y eliminar. | `gestor` |
| [`tetris`](tetris) | Tetris con menús TUI, records persistentes, renderizado ANSI y capa de plataforma para Linux y Windows. | `tetris` / `tetris.exe` |
| [`tres_raya`](tres_raya) | Tres en raya para dos jugadores en la terminal. | `juego` |

`tetris` es multiplataforma: incluye `Makefile` para Linux y `build.bat` para Windows. El resto de subproyectos se compila con `gcc` directamente.

## Tecnologías

### Lenguaje

- C con `gcc` (versión de referencia: 16.1.0)
- C11

### Bibliotecas por subproyecto

| Subproyecto | Bibliotecas |
| --- | --- |
| `admin_personas` | `stdio.h`, `stdlib.h`, `string.h` |
| `tetris` | `stdio.h`, `stdlib.h`, `string.h`, `time.h`, `errno.h`, `limits.h`; en Linux `termios.h`, `unistd.h`, `sys/select.h`, `signal.h`; en Windows `conio.h`, `windows.h` |
| `tres_raya` | `stdio.h`, `math.h` |

## Requisitos previos

- Git.
- `gcc` con soporte para C11.
- `make` (solo para compilar `tetris` en Linux).
- En Windows: MinGW/MSYS2 o un compilador compatible.

## Instalación

1. Clona el repositorio:

   ```bash
   git clone https://github.com/DevAlejandro2007/c.git
   cd c
   ```

2. Entra en la carpeta del subproyecto que quieras probar.

## Compilación y ejecución

Los comandos se ejecutan desde la carpeta de cada subproyecto.

### `admin_personas`

```bash
cd admin_personas
gcc main.c persona.c -o gestor
./gestor
```

En Windows, el ejecutable se genera como `gestor.exe`.

### `tetris`

En Linux:

```bash
cd tetris
make
./tetris
```

Para limpiar los binarios generados:

```bash
make clean
```

En Windows:

```bat
cd tetris
build.bat
tetris.exe
```

También se puede compilar directamente:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c logica.c render.c ui.c puntajes.c platform/term.c -o tetris
```

En Windows, usa `-o tetris.exe` y ejecuta `tetris.exe`.

### `tres_raya`

```bash
cd tres_raya
gcc juego.c funciones.c -o juego
./juego
```

> Ejecuta cada programa desde la carpeta de su subproyecto para que las rutas de compilación coincidan con las que se muestran aquí.

## Arquitectura y decisiones

```text
.
├── admin_personas/   # Gestión de personas
├── tetris/           # Juego + capa platform/
├── tres_raya/        # Tres en raya
└── docs/decisions/   # Registros de decisiones de arquitectura (ADR)
```

Cada subproyecto es autónomo y declara sus interfaces en cabeceras locales. Las decisiones de diseño se registran como ADR:

| ADR | Decisión |
| --- | --- |
| [ADR-001](docs/decisions/0001-subproyectos-independientes.md) | Organizar el repositorio en subproyectos independientes. |
| [ADR-002](docs/decisions/0002-cabeceras-locales.md) | Declarar cada subproyecto con cabeceras locales `.h`. |
| [ADR-003](docs/decisions/0003-capa-plataforma-tetris.md) | Aislar el código dependiente del sistema en `platform/`. |
| [ADR-004](docs/decisions/0004-tui-ansi-propia.md) | Construir una TUI propia con ANSI en lugar de usar ncurses. |
| [ADR-005](docs/decisions/0005-persistencia-records-texto.md) | Persistir los records de `tetris` en un archivo de texto plano. |

Para más detalles sobre `tetris`, consulta su propio [README](tetris/README.md).

## Cómo contribuir

- Compila y prueba los cambios antes de ejecutarlos.
- No versiones binarios: `.gitignore` ya excluye `*.exe`, `*.o`, `*.out` y los ejecutables de cada subproyecto.
- Si cambias una función pública, actualiza la cabecera `.h` correspondiente.
- Si tomas una decisión de arquitectura relevante, añade un ADR en [`docs/decisions/`](docs/decisions) siguiendo la numeración existente.
