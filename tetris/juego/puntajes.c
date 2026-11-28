#include "puntajes.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform/platform.h"

#define TAM_LINEA 256

static int es_linea_ignorable(const char *linea) {
    while (*linea == ' ' || *linea == '\t') {
        linea++;
    }
    return *linea == '\0' || *linea == '#';
}

static int analizar_puntaje(const char *texto, int *valor) {
    char *fin;
    long numero;

    if (texto == NULL || valor == NULL || *texto == '\0') {
        return 0;
    }

    errno = 0;
    numero = strtol(texto, &fin, 10);
    if (fin == texto || errno == ERANGE) {
        return 0;
    }

    while (*fin == ' ' || *fin == '\t' || *fin == '\r') {
        fin++;
    }

    if (*fin != '\0' || numero <= 0 || numero > INT_MAX) {
        return 0;
    }

    *valor = (int)numero;
    return 1;
}

void sanear_nombre(const char *entrada, char *salida, int tam) {
    const unsigned char *p = (const unsigned char *)entrada;
    int j = 0;

    if (salida == NULL || tam <= 0) {
        return;
    }

    if (p != NULL) {
        while (*p == ' ') {
            p++;
        }

        while (*p != '\0' && j < tam - 1) {
            unsigned char c = *p++;

            if (c < 32 || c == 127) {
                continue;
            }
            if (c == '|') {
                c = '-';
            }
            salida[j++] = (char)c;
        }
    }

    while (j > 0 && salida[j - 1] == ' ') {
        j--;
    }
    salida[j] = '\0';

    if (j == 0) {
        const char *por_defecto = NOMBRE_POR_DEFECTO;
        int k = 0;

        while (por_defecto[k] != '\0' && k < tam - 1) {
            salida[k] = por_defecto[k];
            k++;
        }
        salida[k] = '\0';
    }
}

int puntaje_califica(const Puntaje *lista, int cantidad, int puntaje, int maximo) {
    if (puntaje <= 0 || maximo <= 0) {
        return 0;
    }
    if (cantidad < 0) {
        cantidad = 0;
    }
    if (cantidad < maximo) {
        return 1;
    }
    if (lista == NULL) {
        return 0;
    }
    return puntaje > lista[maximo - 1].puntaje;
}

int insertar_puntaje(Puntaje *lista, int *cantidad, const char *nombre,
                     int puntaje, int maximo) {
    char nombre_limpio[MAX_NOMBRE + 1];
    int total;
    int posicion;

    if (lista == NULL || cantidad == NULL || maximo <= 0 || puntaje <= 0) {
        return 0;
    }

    total = *cantidad;
    if (total < 0) {
        total = 0;
    } else if (total > maximo) {
        total = maximo;
    }

    sanear_nombre(nombre, nombre_limpio, sizeof nombre_limpio);

    if (!puntaje_califica(lista, total, puntaje, maximo)) {
        return 0;
    }

    posicion = 0;
    while (posicion < total &&
           (lista[posicion].puntaje > puntaje ||
            (lista[posicion].puntaje == puntaje &&
             strcmp(lista[posicion].nombre, nombre_limpio) <= 0))) {
        posicion++;
    }

    if (total < maximo) {
        for (int i = total; i > posicion; i--) {
            lista[i] = lista[i - 1];
        }
        *cantidad = total + 1;
    } else {
        for (int i = maximo - 1; i > posicion; i--) {
            lista[i] = lista[i - 1];
        }
        *cantidad = maximo;
    }

    snprintf(lista[posicion].nombre, sizeof lista[posicion].nombre, "%s",
             nombre_limpio);
    lista[posicion].puntaje = puntaje;
    return 1;
}

int cargar_puntajes(const char *ruta, Puntaje *lista, int maximo) {
    FILE *archivo;
    char linea[TAM_LINEA];
    int cantidad = 0;

    if (ruta == NULL || lista == NULL || maximo <= 0) {
        return -1;
    }

    archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        return (errno == ENOENT) ? 0 : -1;
    }

    while (fgets(linea, sizeof linea, archivo) != NULL) {
        size_t largo = strlen(linea);
        int completa = (largo > 0 && linea[largo - 1] == '\n');

        if (!completa && !feof(archivo)) {
            int c;

            while ((c = fgetc(archivo)) != EOF && c != '\n') {
            }
            continue;
        }

        while (largo > 0 && (linea[largo - 1] == '\n' || linea[largo - 1] == '\r')) {
            linea[--largo] = '\0';
        }

        if (es_linea_ignorable(linea)) {
            continue;
        }

        char *separador = strchr(linea, '|');
        if (separador == NULL) {
            continue;
        }
        *separador = '\0';

        int valor;
        if (!analizar_puntaje(separador + 1, &valor)) {
            continue;
        }

        (void)insertar_puntaje(lista, &cantidad, linea, valor, maximo);
    }

    if (ferror(archivo)) {
        fclose(archivo);
        return -1;
    }

    fclose(archivo);
    return cantidad;
}

int guardar_puntajes(const char *ruta, const Puntaje *lista, int cantidad) {
    char temporal[1024];
    FILE *archivo;
    int escritura_ok = 1;
    int largo;

    if (ruta == NULL || cantidad < 0 || (lista == NULL && cantidad > 0)) {
        return -1;
    }

    largo = snprintf(temporal, sizeof temporal, "%s.tmp", ruta);
    if (largo < 0 || largo >= (int)sizeof temporal) {
        return -1;
    }

    archivo = fopen(temporal, "w");
    if (archivo == NULL) {
        return -1;
    }

    if (fprintf(archivo, "# tetris-scores v1\n# nombre|puntaje\n") < 0) {
        escritura_ok = 0;
    }

    for (int i = 0; escritura_ok && i < cantidad; i++) {
        char nombre[MAX_NOMBRE + 1];

        sanear_nombre(lista[i].nombre, nombre, sizeof nombre);
        if (fprintf(archivo, "%s|%d\n", nombre, lista[i].puntaje) < 0) {
            escritura_ok = 0;
        }
    }

    if (fclose(archivo) != 0) {
        escritura_ok = 0;
    }

    if (!escritura_ok) {
        remove(temporal);
        return -1;
    }

    if (reemplazar_archivo(temporal, ruta) != 0) {
        remove(temporal);
        return -1;
    }

    return 0;
}
