#include "tetris.h"
#include <stdlib.h>
#include <time.h>
#include <conio.h>

int tecla_presionada(void) {
    return _kbhit();
}

char leer_tecla(void) {
    return (char)_getch();
}

void game_over(int jugando){
    if(jugando == 0){
        printf("\033[?25h");
        printf("*** FIN DEL JUEGO ***\n");
        printf("** NO HAS DESPEJADO LAS CASILLAS **");
    }
}

PiezaActual generar_pieza_aleatoria(void) {
    PiezaActual nueva;
    nueva.tipo = rand() % NUM_PIEZAS;
    nueva.x = 3; 
    nueva.y = 0;

    for (int i = 0; i < PIEZA_SIZE; i++) {
        for (int j = 0; j < PIEZA_SIZE; j++) {
            nueva.forma[i][j] = PIEZAS[nueva.tipo][i][j];
        }
    }
    return nueva;
}


int colision(int rango[FIL][COL], PiezaActual pieza, int nueva_x, int nueva_y){
    for (int i = 0; i < PIEZA_SIZE; i++){
        for (int j = 0; j < PIEZA_SIZE; j++){ // Corregido: j = 0
            if (pieza.forma[i][j] == 1){
                int pos_x = nueva_x + j;
                int pos_y = nueva_y + i;    
                
                if (pos_x < 0 || pos_x >= COL || pos_y >= FIL ){
                    return 1; 
                }
                if (pos_y >= 0 && rango[pos_y][pos_x] == 1) {
                    return 1;
                }
            }
        }
    }
    return 0; // Corregido: fuera de los bucles
}
