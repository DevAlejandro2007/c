#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>

/* Detectamos el sistema operativo */
#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <windows.h>
    #define PAUSA_MS(ms) Sleep(ms)
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/select.h>
    #define PAUSA_MS(ms) usleep((ms) * 1000)
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


void limpiar_matriz(int rango[FIL][COL]);
PiezaActual generar_pieza_aleatoria(void);

void configurar_terminal(void);
void restaurar_terminal(void);
int tecla_presionada(void);
char leer_tecla(void);

#endif 
