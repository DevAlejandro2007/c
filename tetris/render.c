#include "tetris.h"
#include <stdlib.h>
#include <time.h>


void render(int rango[FIL][COL], PiezaActual pieza, PiezaActual siguiente,int puntaje ){
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
    printf("======= TETRIS =======\n");
    printf(" Puntaje: %d\n", puntaje);
    printf("======================\n");
    for (int i = 0; i < FIL; i++) {
        printf("[");
        for (int j = 0; j < COL; j++) {
            int es_pieza =0;
                if (i>= pieza.y && i < pieza.y + PIEZA_SIZE && 
                    j >= pieza.x && j < pieza.x + PIEZA_SIZE){
                        if (pieza.forma[i-pieza.y][j-pieza.x]==1){
                            es_pieza = 1; 
                        }
                    }
                    if (es_pieza || rango[i][j] == 1 ){
                        printf("[]");
                    }else{
                        printf("  ");
                    }
              
            }
        printf("]");

        if (i == 1) printf(" SIGUIENTE:");
        if (i >=3 && i < 3 +PIEZA_SIZE){
            printf(" ");
            int fila_sig = i - 3;
            for (int j = 0; j < PIEZA_SIZE; j++){
                if(siguiente.forma[fila_sig][j] == 1 ){
                    printf("[]");
                }else{
                    printf("  ");
                }
            }
        }
        printf("\n");
    }
printf("======================\n");

}

void fijar_pieza(int rango[FIL][COL], PiezaActual pieza){
    for (int i = 0; i < PIEZA_SIZE; i++){
        for (int j = 0; j < PIEZA_SIZE; j++){ 
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
