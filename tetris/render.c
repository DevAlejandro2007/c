#include "tetris.h"
#include <stdlib.h>
#include <time.h>


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

}

void mostrar_menu(void){
    printf("\033[2J\033[H");
    printf("======= TETRIS =======\n");
    printf("======================\n\n");
    printf("      1. JUGAR\n");
    printf("      2. INSTRUCCIONES\n");
    printf("      3. SALIR\n\n");
    printf("======================\n");
    printf("  Selecciona una opcion: ");
    fflush(stdout);
}

void mostrar_instrucciones(void){
    printf("\033[2J\033[H");
    printf("======= INSTRUCCIONES =======\n");
    printf("==============================\n\n");
    printf(" OBJETIVO:\n");
    printf("  Completa filas para eliminarlas y\n");
    printf("  evitar que las piezas lleguen arriba.\n\n");
    printf(" CONTROLES:\n");
    printf("  A / a - Mover a la izquierda\n");
    printf("  D / d - Mover a la derecha\n");
    printf("  S / s - Caida rapida (soft drop)\n");
    printf("  W / w - Rotar pieza\n\n");
    printf(" PUNTUACION:\n");
    printf("  1 linea = +100\n");
    printf("  2 lineas = +300\n");
    printf("  3 lineas = +500\n");
    printf("  4 lineas (TETRIS) = +800\n\n");
    printf("==============================\n");
    printf("  Presiona cualquier tecla para volver...");
    fflush(stdout);
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
