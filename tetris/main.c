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
                jugando = 0; 
                
            }
        }
        

        // 3. Renderizado y Frame Rate
        render(rango, pieza);
        PAUSA_MS(150);
    } 

    printf("\033[?25h");
    restaurar_terminal();
    game_over(jugando);
    return 0;
}