#include "tetris.h"
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void render(int rango[FIL][COL], PiezaActual pieza){
    int tablero_temporal[FIL][COL];
    for (int i = 0; i < FIL; i++){
        for(int j = 0; j < COL; j++){
            tablero_temporal[i][j] = rango[i][j];   
        }
    }

    for (int i = 0; i < PIEZA_SIZE; i++){
        for (int j = 0; j < PIEZA_SIZE; j++){
            if(pieza.forma[i][j] == 1 ){
                int pos_y = pieza.y + i;
                int pos_x = pieza.x + j;
                if (pos_y >= 0 && pos_y < FIL && pos_x >= 0 && pos_x < COL) {
                    tablero_temporal[pos_y][pos_x] = 1;
                }
            }
        }
    }

    printf("\033[H"); 
    printf("----- TETRIS ---- \n");
    for (int i = 0; i < FIL; i++) {
        printf("[");
        for (int j = 0; j < COL; j++) {
            if (tablero_temporal[i][j] == 1) {
                printf("*"); 
            } else {
                printf(" ");
            }
        }
        printf("]\n");
    }
    printf("--------------\n");
    fflush(stdout);
}

void fijar_pieza(int rango[FIL][COL], PiezaActual pieza){
    for (int i = 0; i < PIEZA_SIZE; i++){
        for (int j = 0; j < PIEZA_SIZE; j++){ // Corregido: j = 0
            if (pieza.forma[i][j] == 1){
                int pos_y = pieza.y + i;
                int pos_x = pieza.x + j;
                if (pos_y >= 0 && pos_y < FIL && pos_x >= 0 && pos_x < COL) {
                    rango[pos_y][pos_x] = 1;
                }
            }
        }
    }
}
