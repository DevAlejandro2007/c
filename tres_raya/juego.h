#ifndef JUEGO_H
#define JUEGO_H

int turno(int x);
void mostrar_tablero(int filas[3][3]);
int comprobar_ganador(int filas[3][3]);
int tablero_lleno(int filas[3][3]);
void jugar(int *turn, int *fila, int *columna, int filas[3][3]);

#endif 
