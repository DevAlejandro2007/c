# ADR-005: Persistir los records de Tetris en un archivo de texto plano

## Estado
Aceptado

## Fecha
2026-11-10

## Contexto

`tetris` ya tenía un sistema de puntuación, pero al cerrar el programa se perdía todo. El siguiente paso natural era guardar un top 10 con el nombre del jugador y el puntaje, sin añadir dependencias externas y manteniendo la separación entre lógica, UI y sistema operativo.

El proyecto se usa como material de aprendizaje: el formato debía poder inspeccionarse, editarse a mano y entenderse sin herramientas adicionales. También debía funcionar igual en Linux y Windows.

## Decisión

Guardar los records en un archivo de texto plano llamado `puntajes.txt`, en el directorio desde el que se ejecuta el juego, con el formato:

```text
# tetris-scores v1
# nombre|puntaje
Ana|1200
Anonymous|900
```

Las líneas que empiezan por `#` son comentarios y se ignoran. El formato incluye una cabecera propia (`# tetris-scores v1`) para poder cambiar el formato en el futuro sin adivinar qué contiene cada archivo.

### Reglas de la tabla

- Se conservan como máximo 10 records (`MAX_PUNTAJES`).
- Se ordenan de mayor a menor puntaje.
- Si dos records empatan, se ordenan por nombre alfabético.
- Un puntaje `0` o negativo no se guarda.
- Si hay menos de 10 records, cualquier puntaje positivo entra.
- Si ya hay 10, hay que superar estrictamente al décimo para entrar.
- Un archivo inexistente se trata como una tabla vacía.
- Las líneas corruptas, sin separador o con puntajes inválidos se ignoran.
- Un error real de lectura o escritura se informa con `-1` y la UI muestra un aviso.

### Nombres

El nombre se pide solo cuando el puntaje califica. Si se deja vacío o se pulsa `Esc`, se usa `Anonymous`. Antes de guardar:

- se recortan los espacios de los extremos;
- se eliminan los caracteres de control;
- se sustituye `|` por `-` para no romper el formato;
- se limita a 20 caracteres ASCII (`MAX_NOMBRE`).

### Guardado

El archivo se escribe primero en `puntajes.txt.tmp` y después se reemplaza el original. En Linux/POSIX el reemplazo es atómico con `rename()`. En Windows, `rename()` no sobrescribe un archivo existente, así que se intenta primero y, si falla, se elimina el destino y se reintenta. Esa diferencia de sistema operativo vive en `platform/term.c`, dentro de `reemplazar_archivo()`, para que `puntajes.c` siga siendo portable.

### Separación de responsabilidades

- `puntajes.c` / `puntajes.h` conocen el formato, el saneado, el orden y la persistencia.
- `main.c` decide cuándo cargar, cuándo preguntar el nombre y cuándo guardar.
- `ui.c` solo dibuja la pantalla de nombre, la tabla de records y los avisos; no hace E/S de archivos.
- `platform/` concentra la diferencia de `rename()` entre Windows y POSIX.

El archivo `puntajes.txt` está ignorado por Git porque es un dato local de cada jugador.

## Alternativas consideradas

### JSON

- Pros: formato estándar, fácil de leer desde otros lenguajes.
- Contras: habría que escribir o añadir un parser; el proyecto evita dependencias externas y no necesita estructuras anidadas.
- Rechazada: demasiado peso para una lista de dos campos.

### CSV

- Pros: estándar y legible.
- Contras: obliga a escapar comas, comillas y saltos de línea; el caso de uso es más simple que un CSV real.
- Rechazada: `nombre|puntaje` es suficiente si se sanea el separador.

### SQLite

- Pros: consultas, orden, integridad y concurrencia resueltos.
- Contras: dependencia externa, binario opaco y mucha complejidad para un top 10.
- Rechazada: no aporta valor didáctico para este subproyecto y rompe la regla de no usar dependencias externas.

### Archivo binario

- Pros: lectura/escritura directa y tamaño compacto.
- Contras: no se puede inspeccionar ni editar con un editor de texto; depende del endianness y del tamaño de los tipos.
- Rechazada: la legibilidad y la portabilidad son más importantes que el tamaño.

## Consecuencias

- El archivo se puede abrir, entender y corregir a mano.
- No hace falta ninguna biblioteca externa.
- El formato depende de que el nombre no contenga `|`; por eso se sanea antes de guardar.
- No hay bloqueo entre procesos: si dos instancias del juego guardan a la vez, la última en reemplazar el archivo gana.
- El guardado con archivo temporal reduce el riesgo de dejar `puntajes.txt` a medias si falla una escritura.
- Si en el futuro se necesita otro formato, la cabecera `# tetris-scores v1` permite detectar la versión y migrar.
- Borrar `puntajes.txt` reinicia la tabla de records.
