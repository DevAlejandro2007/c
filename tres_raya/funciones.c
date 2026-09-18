#include <stdio.h> 
#include "juego.h"

int turno(int x) {
    return (x == 1) ? 2 : 1;
}

void mostrar_tablero(int filas[3][3]) {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        printf(" ");
        for (int j = 0; j < 3; j++) {
            if (filas[i][j] == 0) {
                printf("   ");
            } else if (filas[i][j] == 1) {
                printf(" X ");
            } else if (filas[i][j] == 2) {
                printf(" O ");
            }

            if (j < 2) {
                printf("|");
            }
        }
        printf("\n");
        if (i < 2) {
            printf("----+---+----\n");
        }
    }
    printf("\n");
}

int comprobar_ganador(int filas[3][3]) {

    for (int i = 0; i < 3; i++) {
        if (filas[i][0] == filas[i][1] && filas[i][1] == filas[i][2] && filas[i][0] != 0) {
            return filas[i][0];
        }   
        if (filas[0][i] == filas[1][i] && filas[1][i] == filas[2][i] && filas[0][i] != 0) {
            return filas[0][i];
        }
    }
    // Diagonales
    if ((filas[0][0] == filas[1][1] && filas[1][1] == filas[2][2]) || 
        (filas[0][2] == filas[1][1] && filas[1][1] == filas[2][0])) {
        if (filas[1][1] != 0) {
            return filas[1][1];
        }
    }
    return 0;
}

int tablero_lleno(int filas[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (filas[i][j] == 0) {
                return 0; 
            }
        }
    }
    return 3;
}