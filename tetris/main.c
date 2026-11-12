#include "tetris.h"
#include "puntajes.h"
#include "ui.h"

const int PIEZAS[NUM_PIEZAS][PIEZA_SIZE][PIEZA_SIZE] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, // I
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // O
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // T
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // L
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // J
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // S
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}  // Z
};

static int puntaje_global = 0;

static int tecla_es(int c, int especial, char letra) {
    if (c == especial || c == letra) {
        return 1;
    }
    if (letra >= 'a' && letra <= 'z' && c == letra - ('a' - 'A')) {
        return 1;
    }
    return 0;
}

static void registrar_puntaje(int puntaje) {
    Puntaje lista[MAX_PUNTAJES];
    char nombre[MAX_NOMBRE + 1];
    int cantidad = cargar_puntajes(RUTA_PUNTAJES, lista, MAX_PUNTAJES);

    if (cantidad < 0) {
        pantalla_aviso("No se pudo leer puntajes.txt");
        return;
    }

    if (!puntaje_califica(lista, cantidad, puntaje, MAX_PUNTAJES)) {
        return;
    }

    pantalla_nombre(nombre, sizeof nombre);
    if (!insertar_puntaje(lista, &cantidad, nombre, puntaje, MAX_PUNTAJES)) {
        return;
    }

    if (guardar_puntajes(RUTA_PUNTAJES, lista, cantidad) != 0) {
        pantalla_aviso("No se pudo guardar puntajes.txt");
        return;
    }

    pantalla_records(lista, cantidad);
}

ResultadoJuego iniciar_juego(void) {
    puntaje_global = 0;

    int rango[FIL][COL] = {0};

    PiezaActual pieza = generar_pieza_aleatoria();
    PiezaActual siguiente_pieza = generar_pieza_aleatoria();

    int jugando = 1;
    int error_entrada = 0;

    limpiar_pantalla();

    while (jugando) {
        int disponible = tecla_presionada();

        if (disponible < 0) {
            perror("Error al leer el teclado");
            error_entrada = 1;
            break;
        }

        if (disponible > 0) {
            int c = leer_tecla();

            if (c == EOF) {
                fputs("No se pudo leer una tecla de la terminal.\n", stderr);
                error_entrada = 1;
                break;
            }

            if (tecla_es(c, TECLA_ESCAPE, 'p')) {
                switch (menu_pausa()) {
                    case PAUSA_CONTINUAR:
                        limpiar_pantalla();
                        continue;
                    case PAUSA_REINICIAR:
                        return JUEGO_REINICIAR;
                    case PAUSA_MENU:
                        return JUEGO_VOLVER_MENU;
                }
            }

            if (tecla_es(c, TECLA_ARRIBA, 'w')) {
                rotar_pieza(&pieza, rango);
            } else if (tecla_es(c, TECLA_IZQUIERDA, 'a')) {
                if (!colision(rango, pieza, pieza.x - 1, pieza.y)) {
                    pieza.x--;
                }
            } else if (tecla_es(c, TECLA_DERECHA, 'd')) {
                if (!colision(rango, pieza, pieza.x + 1, pieza.y)) {
                    pieza.x++;
                }
            } else if (tecla_es(c, TECLA_ABAJO, 's')) {
                if (!colision(rango, pieza, pieza.x, pieza.y + 1)) {
                    pieza.y++;
                }
            }
        }

        if (!colision(rango, pieza, pieza.x, pieza.y + 1)) {
            pieza.y++;
        } else {
            fijar_pieza(rango, pieza);
            int lineas = limpiar_lineas(rango);

            if (lineas > 0) {
                if (lineas == 1) puntaje_global += 100;
                else if (lineas == 2) puntaje_global += 300;
                else if (lineas == 3) puntaje_global += 500;
                else if (lineas == 4) puntaje_global += 800;
            }

            pieza = siguiente_pieza;
            siguiente_pieza = generar_pieza_aleatoria();

            if (colision(rango, pieza, pieza.x, pieza.y + 1)) {
                fijar_pieza(rango, pieza);
                limpiar_lineas(rango);

                pieza = siguiente_pieza;
                siguiente_pieza = generar_pieza_aleatoria();

                if (colision(rango, pieza, pieza.x, pieza.y)) {
                    jugando = 0;
                }
            }
        }

        render(rango, pieza, siguiente_pieza, puntaje_global);
        pausa_ms(150);
    }

    if (error_entrada) {
        return JUEGO_VOLVER_MENU;
    }

    /* Deja ver el tablero final antes de mostrar el menu. */
    pausa_ms(500);

    /* Descarta teclas pulsadas durante la pausa para no elegir sin querer. */
    while (tecla_presionada() > 0) {
        (void)leer_tecla();
    }

    registrar_puntaje(puntaje_global);

    return (menu_game_over(puntaje_global) == GAMEOVER_REINTENTAR)
         ? JUEGO_REINICIAR
         : JUEGO_VOLVER_MENU;
}

int main(void) {
    if (configurar_terminal() == -1) {
        fprintf(stderr, "No se pudo configurar la terminal para jugar.\n");
        return EXIT_FAILURE;
    }

    atexit(restaurar_terminal);
    instalar_manejadores();

    srand((unsigned int)time(NULL));

    entrar_pantalla_alterna();
    ocultar_cursor();

    int corriendo = 1;

    while (corriendo) {
        switch (menu_principal()) {
            case MENU_JUGAR: {
                ResultadoJuego resultado;
                do {
                    resultado = iniciar_juego();
                } while (resultado == JUEGO_REINICIAR);
                break;
            }
            case MENU_RECORDS: {
                Puntaje lista[MAX_PUNTAJES];
                int cantidad = cargar_puntajes(RUTA_PUNTAJES, lista,
                                               MAX_PUNTAJES);

                if (cantidad < 0) {
                    pantalla_aviso("No se pudo leer puntajes.txt");
                } else {
                    pantalla_records(lista, cantidad);
                }
                break;
            }
            case MENU_INSTRUCCIONES:
                pantalla_instrucciones();
                break;
            case MENU_SALIR:
                corriendo = 0;
                break;
        }
    }

    restaurar_terminal();
    printf("Hasta luego!\n");
    fflush(stdout);

    return 0;
}
