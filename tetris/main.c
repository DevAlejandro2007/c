#include "tetris.h"
#include <stdlib.h>
#include <time.h>
#include <conio.h>

const int PIEZAS[NUM_PIEZAS][PIEZA_SIZE][PIEZA_SIZE] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, // I
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // O
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // T
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // L
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // J
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // S
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}  // Z
};

#if !(defined(_WIN32) || defined(_WIN64))
    static struct termios old_term;

    void configurar_terminal(void) {
        struct termios new_term;
        tcgetattr(STDIN_FILENO, &old_term);
        new_term = old_term;
        new_term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    }

    void restaurar_terminal(void) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    }
#else
    void configurar_terminal(void) {}
    void restaurar_terminal(void) {}
#endif

int tecla_presionada(void) {
    return _kbhit();
}

char leer_tecla(void) {
    return (char)_getch();
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

int main(void){
    srand((unsigned int)time(NULL)); 
    configurar_terminal();
    printf("\033[2J\033[?25l"); 

    int rango[FIL][COL] = {0};
    PiezaActual pieza = generar_pieza_aleatoria();
    int jugando = 1;

    while (jugando){
        // 1. Entrada de usuario
        if (tecla_presionada()){
            char c = leer_tecla();
            if (c == 'a' || c == 'A'){
                if (!colision(rango, pieza, pieza.x - 1, pieza.y)){
                    pieza.x--;
                }
            } else if (c == 'd' || c == 'D'){
                if (!colision(rango, pieza, pieza.x + 1, pieza.y)) {
                    pieza.x++;
                }
            } else if (c == 's' || c == 'S') {
                if (!colision(rango, pieza, pieza.x, pieza.y + 1)) {
                    pieza.y++;
                }
            }
        }

        // 2. Caída Automática (Lógica fuera de la condición del teclado)
        if (!colision(rango, pieza, pieza.x, pieza.y + 1)) {
            pieza.y++;
        } else {
            fijar_pieza(rango, pieza);
            pieza = generar_pieza_aleatoria();

            if (colision(rango, pieza, pieza.x, pieza.y)) {
                jugando = 0; // Game Over
            }
        }
        

        // 3. Renderizado y Frame Rate
        render(rango, pieza);
        PAUSA_MS(150);
    } 

    printf("\033[?25h");
    restaurar_terminal();
    return 0;
}