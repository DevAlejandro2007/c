#include "tetris.h"

#if defined(_WIN32)
static HANDLE salida_consola;
static DWORD modo_consola_original;
static int terminal_configurado;

int configurar_terminal(void) {
    salida_consola = GetStdHandle(STD_OUTPUT_HANDLE);
    if (salida_consola == INVALID_HANDLE_VALUE ||
        !GetConsoleMode(salida_consola, &modo_consola_original)) {
        return -1;
    }

    if (!SetConsoleMode(salida_consola,
                        modo_consola_original | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        return -1;
    }

    terminal_configurado = 1;
    return 0;
}

void restaurar_terminal(void) {
    if (terminal_configurado) {
        printf("\033[?25h");
        fflush(stdout);
        SetConsoleMode(salida_consola, modo_consola_original);
        terminal_configurado = 0;
    }
}

int tecla_presionada(void) {
    return _kbhit();
}

int leer_tecla(void) {
    return _getch();
}

void pausa_ms(unsigned int milisegundos) {
    Sleep(milisegundos);
}
#else
static struct termios modo_terminal_original;
static int terminal_configurado;

int configurar_terminal(void) {
    struct termios modo_terminal;
    if (tcgetattr(STDIN_FILENO, &modo_terminal_original) == -1) {
        return -1;
    }

    modo_terminal = modo_terminal_original;
    modo_terminal.c_lflag &= (tcflag_t) ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &modo_terminal) == -1) {
        return -1;
    }

    terminal_configurado = 1;
    return 0;
}

void restaurar_terminal(void) {
    if (terminal_configurado) {
        printf("\033[?25h");
        fflush(stdout);
        tcsetattr(STDIN_FILENO, TCSANOW, &modo_terminal_original);
        terminal_configurado = 0;
    }
}

int tecla_presionada(void) {
    fd_set descriptores;
    struct timeval espera = {0, 0};

    FD_ZERO(&descriptores);
    FD_SET(STDIN_FILENO, &descriptores);
    return select(STDIN_FILENO + 1, &descriptores, NULL, NULL, &espera);
}

int leer_tecla(void) {
    unsigned char tecla;
    return read(STDIN_FILENO, &tecla, 1) == 1 ? tecla : EOF;
}

void pausa_ms(unsigned int milisegundos) {
    struct timeval espera;

    espera.tv_sec = (long)(milisegundos / 1000);
    espera.tv_usec = (long)(milisegundos % 1000) * 1000L;
    select(0, NULL, NULL, NULL, &espera);
}
#endif

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
