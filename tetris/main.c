#include "tetris.h"

const int PIEZAS[NUM_PIEZAS][PIEZA_SIZE][PIEZA_SIZE] = {
    {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, // I
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // O
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // T
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // L
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}, // J
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}, // S
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}  // Z
};

int puntaje = 0;

int main(void){
    srand((unsigned int)time(NULL)); 
    if (configurar_terminal() == -1) {
        fprintf(stderr, "No se pudo configurar la terminal para jugar.\n");
        return EXIT_FAILURE;
    }
    atexit(restaurar_terminal);
    printf("\033[2J\033[?25l"); 

    int rango[FIL][COL] = {0};
    
    PiezaActual pieza = generar_pieza_aleatoria();
    PiezaActual siguiente_pieza = generar_pieza_aleatoria();    

    int jugando = 1;
    int error_entrada = 0;

    while (jugando){
        // 1. Entrada de usuario
        int disponible = tecla_presionada();
        if (disponible < 0) {
            perror("Error al leer el teclado");
            error_entrada = 1;
            break;
        }
        if (disponible > 0){
            int c = leer_tecla();
            if (c == EOF) {
                fputs("No se pudo leer una tecla de la terminal.\n", stderr);
                error_entrada = 1;
                break;
            }
            if(c == 'w' || c == 'W'){
                rotar_pieza(&pieza, rango);
            }
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

        if (!colision(rango, pieza, pieza.x, pieza.y + 1)) {
            pieza.y++;
        } else {
            fijar_pieza(rango, pieza);
            int lineas = limpiar_lineas(rango);
            if(lineas>0){
                if (lineas == 1) puntaje += 100;
                else if (lineas == 2) puntaje += 300;
                else if (lineas == 3) puntaje += 500;
                else if (lineas == 4) puntaje += 800;
            }
            pieza = siguiente_pieza;
            siguiente_pieza = generar_pieza_aleatoria();

            if (colision(rango, pieza, pieza.x, pieza.y + 1)) {
                fijar_pieza(rango, pieza);
                limpiar_lineas(rango);

                pieza = siguiente_pieza;
                siguiente_pieza = generar_pieza_aleatoria();

                if(colision(rango, pieza, pieza.x, pieza.y)){
                    jugando = 0;
                }
            }
        }
        render(rango, pieza, siguiente_pieza, puntaje);
        pausa_ms(150);
    } 

    restaurar_terminal();
    if (error_entrada) {
        return EXIT_FAILURE;
    }
    game_over(jugando);
    return 0;
}