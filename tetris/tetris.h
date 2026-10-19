#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(_WIN32)
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/select.h>
#endif

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

extern const int PIEZAS[NUM_PIEZAS][PIEZA_SIZE][PIEZA_SIZE];

int configurar_terminal(void);
void restaurar_terminal(void);
void pausa_ms(unsigned int milisegundos);
PiezaActual generar_pieza_aleatoria(void);
int tecla_presionada(void);
int leer_tecla(void);
void render(int rango[FIL][COL], PiezaActual pieza, PiezaActual siguiente, int puntaje);
int colision(int rango[FIL][COL], PiezaActual pieza, int nueva_x, int nueva_y);
void game_over(int jugando);
void fijar_pieza(int rango[FIL][COL], PiezaActual pieza);
void rotar_pieza(PiezaActual *pieza, int rango[FIL][COL]);
int limpiar_lineas(int rango[FIL][COL]);

#endif
