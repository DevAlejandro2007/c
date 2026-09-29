#include "tetris.h"

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
        for (int j = 0; j < PIEZA_SIZE; j++){ 
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
    return 0; 
}

void rotar_pieza(PiezaActual *pieza, int rango[FIL][COL]){

    int temp[PIEZA_SIZE][PIEZA_SIZE];

    for (int i = 0; i < PIEZA_SIZE; i++ ){
        for (int j = 0; j < PIEZA_SIZE; j++){
            temp[j][PIEZA_SIZE - 1 - i] = pieza -> forma[i][j];
        }
    }
    PiezaActual prueba = *pieza;
    for (int i = 0; i< PIEZA_SIZE; i++){
        for (int j = 0; j < PIEZA_SIZE; j++){
            prueba.forma[i][j] = temp[i][j];
        }
    }

    if(!colision(rango,prueba,prueba.x,prueba.y)){
        for(int i = 0; i < PIEZA_SIZE; i++){
            for (int j=0; j < PIEZA_SIZE; j++){
                pieza ->forma[i][j] = temp[i][j];
            }
        }
    }

}

int limpiar_lineas(int rango[FIL][COL]){
    int lineas_limpiadas = 0;
    
    for (int i = FIL - 1; i >=0 ; i--){
        int fila_llena = 1;
    
        for (int j = 0; j < COL; j++){
            if(rango[i][j] == 0){
                fila_llena = 0;
                break;
            }
        }
        if (fila_llena){
                lineas_limpiadas++;

            for (int k = i; k > 0; k--) {
                for (int j = 0; j < COL; j++) {
                    rango[k][j] = rango[k - 1][j];
                    }
                }
            
                for (int j = 0; j < COL; j++) {
                    rango[0][j] = 0;
                    }
            
                i++;
        
            }
        }      

        return lineas_limpiadas;

    }

