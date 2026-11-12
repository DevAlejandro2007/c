#include "ui.h"

#include <stdio.h>
#include <string.h>

#include "platform/platform.h"

#define ANCHO_CAJA 38

static void imprimir_borde(void) {
    printf("+");
    for (int i = 0; i < ANCHO_CAJA; i++) {
        putchar('-');
    }
    printf("+\n");
}

static void imprimir_titulo(const char *titulo) {
    int longitud = (int)strlen(titulo);
    int izquierda;
    int derecha;

    if (longitud > ANCHO_CAJA) {
        longitud = ANCHO_CAJA;
    }

    izquierda = (ANCHO_CAJA - longitud) / 2;
    derecha = ANCHO_CAJA - longitud - izquierda;

    printf("|");
    for (int i = 0; i < izquierda; i++) {
        putchar(' ');
    }
    printf("%.*s", longitud, titulo);
    for (int i = 0; i < derecha; i++) {
        putchar(' ');
    }
    printf("|\n");
}

static void imprimir_linea(const char *texto) {
    printf("| %-*.*s |\n", ANCHO_CAJA - 2, ANCHO_CAJA - 2, texto);
}

static int menu_seleccion(const char *titulo, const char *items[], int cantidad,
                          int seleccion, const char *ayuda) {
    if (cantidad <= 0) {
        return -1;
    }

    for (;;) {
        limpiar_pantalla();
        imprimir_borde();
        imprimir_titulo(titulo);
        imprimir_borde();

        for (int i = 0; i < cantidad; i++) {
            if (i == seleccion) {
                printf("|\033[7m > %-*s \033[0m|\n", ANCHO_CAJA - 4, items[i]);
            } else {
                printf("|   %-*s |\n", ANCHO_CAJA - 4, items[i]);
            }
        }

        imprimir_borde();
        printf("\n  %s\n", ayuda);
        fflush(stdout);

        int c = leer_tecla();
        if (c == EOF) {
            return -1;
        }

        if (c == TECLA_ARRIBA || c == 'w' || c == 'W') {
            seleccion = (seleccion + cantidad - 1) % cantidad;
        } else if (c == TECLA_ABAJO || c == 's' || c == 'S') {
            seleccion = (seleccion + 1) % cantidad;
        } else if (c == TECLA_ENTER) {
            return seleccion;
        } else if (c == TECLA_ESCAPE || c == 'q' || c == 'Q') {
            return -1;
        }
    }
}

OpcionMenu menu_principal(void) {
    static const char *items[] = {"Jugar", "Records", "Instrucciones", "Salir"};
    int seleccion = menu_seleccion(
        "T E T R I S",
        items,
        4,
        0,
        "Flechas o W/S para mover | Enter selecciona | Q sale");

    return (seleccion < 0) ? MENU_SALIR : (OpcionMenu)seleccion;
}

void pantalla_instrucciones(void) {
    limpiar_pantalla();
    imprimir_borde();
    imprimir_titulo("INSTRUCCIONES");
    imprimir_borde();
    imprimir_linea("OBJETIVO:");
    imprimir_linea(" Completa filas para eliminarlas");
    imprimir_linea(" y evita que las piezas lleguen");
    imprimir_linea(" arriba.");
    imprimir_linea("");
    imprimir_linea("CONTROLES:");
    imprimir_linea(" A / Izquierda - mover");
    imprimir_linea(" D / Derecha   - mover");
    imprimir_linea(" S / Abajo     - caida rapida");
    imprimir_linea(" W / Arriba    - rotar");
    imprimir_linea(" Esc / P       - pausa");
    imprimir_linea("");
    imprimir_linea("PUNTUACION:");
    imprimir_linea(" 1 linea  = 100");
    imprimir_linea(" 2 lineas = 300");
    imprimir_linea(" 3 lineas = 500");
    imprimir_linea(" 4 lineas = 800");
    imprimir_borde();
    printf("\n  Enter o Esc para volver...\n");
    fflush(stdout);

    for (;;) {
        int c = leer_tecla();
        if (c == EOF || c == TECLA_ENTER || c == TECLA_ESCAPE ||
            c == 'q' || c == 'Q') {
            break;
        }
    }
}

void pantalla_nombre(char *destino, int tam) {
    char entrada[MAX_NOMBRE + 1] = "";
    int longitud = 0;
    int maximo;

    if (destino == NULL || tam <= 0) {
        return;
    }

    maximo = tam - 1;
    if (maximo > MAX_NOMBRE) {
        maximo = MAX_NOMBRE;
    }

    for (;;) {
        limpiar_pantalla();
        imprimir_borde();
        imprimir_titulo("NUEVO RECORD");
        imprimir_borde();
        imprimir_linea("Felicitaciones! Entraste al top 10.");
        imprimir_linea("");
        imprimir_linea("Escribe tu nombre:");
        imprimir_linea(entrada);
        imprimir_borde();
        printf("\n  Enter guarda | Esc usa %s | Backspace borra\n",
               NOMBRE_POR_DEFECTO);
        fflush(stdout);

        int c = leer_tecla();
        if (c == EOF || c == TECLA_ESCAPE) {
            snprintf(destino, (size_t)tam, "%s", NOMBRE_POR_DEFECTO);
            return;
        }
        if (c == TECLA_ENTER) {
            if (longitud == 0) {
                snprintf(destino, (size_t)tam, "%s", NOMBRE_POR_DEFECTO);
            } else {
                snprintf(destino, (size_t)tam, "%s", entrada);
            }
            return;
        }
        if (c == TECLA_BACKSPACE) {
            if (longitud > 0) {
                entrada[--longitud] = '\0';
            }
        } else if ((c >= 32 && c <= 126) || c == TECLA_ESPACIO) {
            if (longitud < maximo) {
                entrada[longitud++] = (c == TECLA_ESPACIO) ? ' ' : (char)c;
                entrada[longitud] = '\0';
            }
        }
    }
}

void pantalla_records(const Puntaje *lista, int cantidad) {
    char linea[64];

    if (cantidad < 0) {
        cantidad = 0;
    }
    if (cantidad > MAX_PUNTAJES) {
        cantidad = MAX_PUNTAJES;
    }

    limpiar_pantalla();
    imprimir_borde();
    imprimir_titulo("RECORDS");
    imprimir_borde();

    if (cantidad == 0 || lista == NULL) {
        imprimir_linea("Todavia no hay records.");
        imprimir_linea("Juega para aparecer aqui.");
    } else {
        imprimir_linea("POS NOMBRE               PUNTOS");
        for (int i = 0; i < cantidad; i++) {
            snprintf(linea, sizeof linea, "%2d. %-20.20s %6d",
                     i + 1, lista[i].nombre, lista[i].puntaje);
            imprimir_linea(linea);
        }
    }

    imprimir_borde();
    printf("\n  Enter o Esc para volver...\n");
    fflush(stdout);

    for (;;) {
        int c = leer_tecla();
        if (c == EOF || c == TECLA_ENTER || c == TECLA_ESCAPE ||
            c == 'q' || c == 'Q') {
            break;
        }
    }
}

void pantalla_aviso(const char *mensaje) {
    limpiar_pantalla();
    imprimir_borde();
    imprimir_titulo("AVISO");
    imprimir_borde();
    imprimir_linea((mensaje != NULL) ? mensaje : "");
    imprimir_borde();
    printf("\n  Enter o Esc para continuar...\n");
    fflush(stdout);

    for (;;) {
        int c = leer_tecla();
        if (c == EOF || c == TECLA_ENTER || c == TECLA_ESCAPE ||
            c == 'q' || c == 'Q') {
            break;
        }
    }
}

OpcionPausa menu_pausa(void) {
    static const char *items[] = {"Continuar", "Reiniciar", "Volver al menu"};
    int seleccion = menu_seleccion(
        "P A U S A",
        items,
        3,
        0,
        "Enter selecciona | Esc continua");

    if (seleccion < 0) {
        return PAUSA_CONTINUAR;
    }
    return (OpcionPausa)seleccion;
}

OpcionGameOver menu_game_over(int puntaje) {
    static const char *items[] = {"Reintentar", "Volver al menu"};
    char ayuda[96];
    int seleccion;

    snprintf(ayuda, sizeof ayuda,
             "Puntaje: %d | Enter selecciona | Esc vuelve al menu", puntaje);

    seleccion = menu_seleccion("FIN DEL JUEGO", items, 2, 0, ayuda);
    return (seleccion < 0) ? GAMEOVER_MENU : (OpcionGameOver)seleccion;
}
