#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "platform/platform.h"

#define FIL 20
#define COL 10
#define NUM_PIEZAS 7
#define PIEZA_SIZE 4

typedef struct {
    int tipo;
    int x;
    int y;
    int forma[PIEZA_SIZE][PIEZA_SIZE];
} PiezaActual;

typedef enum {
    JUEGO_REINICIAR,
    JUEGO_VOLVER_MENU
} ResultadoJuego;

extern const int PIEZAS[NUM_PIEZAS][PIEZA_SIZE][PIEZA_SIZE];

PiezaActual generar_pieza_aleatoria(void);
void render(int rango[FIL][COL], PiezaActual pieza, PiezaActual siguiente, int puntaje);
ResultadoJuego iniciar_juego(void);
int colision(int rango[FIL][COL], PiezaActual pieza, int nueva_x, int nueva_y);
void fijar_pieza(int rango[FIL][COL], PiezaActual pieza);
void rotar_pieza(PiezaActual *pieza, int rango[FIL][COL]);
int limpiar_lineas(int rango[FIL][COL]);

#endif
