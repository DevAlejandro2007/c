#include "tetris.h"


void render(int rango[FIL][COL], PiezaActual pieza, PiezaActual siguiente,int puntaje ){
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
    fflush(stdout);
}
