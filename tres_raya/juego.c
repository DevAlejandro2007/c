#include <stdio.h> 
#include <math.h>
#include "juego.h"

void jugar(int *turn, int *fila, int *columna, int filas[3][3]) {
    int valido = 0;

    while (!valido) {
        printf("\nJugador %d, escoge fila (1-3): ", *turn);
        if (scanf("%d", fila) != 1) {
            while (getchar() != '\n'); 
            printf("\n¡Entrada invalida! Ingresa un numero del 1 al 3.\n");
            continue;
        }

        printf("Jugador %d, escoge columna (1-3): ", *turn);
        if (scanf("%d", columna) != 1) {
            while (getchar() != '\n');
            printf("\n¡Entrada invalida! Ingresa un numero del 1 al 3.\n");
            continue;
        }

        if (*fila < 1 || *fila > 3 || *columna < 1 || *columna > 3) {
            printf("\n=========================================\n");
            printf(" LA CASILLA NO ES TAN GRANDE (Usa 1 a 3) \n");
            printf("=========================================\n");
        } else {
            int f = *fila - 1;
            int c = *columna - 1;

            if (filas[f][c] != 0) {
                printf("\n=========================================\n");
                printf(" CASILLA OCUPADA Escoge otra posición. \n");
                printf("=========================================\n");
            } else {
                filas[f][c] = *turn;
                *turn = turno(*turn);
                valido = 1; 
            }
        }
    }
}

int main(void) {
    printf("===================================\n");
    printf("  BIENVENIDOS A MI PRIMER JUEGO EN C \n");
    printf("===================================\n\n");

    int win = 0;
    int fila = 0, columna = 0;
    int turn = 1;
    int filas[3][3] = {0}; 

    mostrar_tablero(filas);

    while (win == 0) {
        jugar(&turn, &fila, &columna, filas);
        printf("\n-----------------------------------\n");
        mostrar_tablero(filas);
        
        win = comprobar_ganador(filas);     
        if (win == 0) {
            win = tablero_lleno(filas);
        }
    }

    printf("\n===================================\n");
    if (win == 1) {
        printf("    FELICIDADES X HA GANADO      \n");
    } else if (win == 2) {
        printf("    FELICIDADES O HA GANADO      \n");
    } else if (win == 3) {
        printf("            EMPATE               \n");
    }
    printf("===================================\n");
    printf("           FIN DEL JUEGO           \n\n");

    return 0;
}