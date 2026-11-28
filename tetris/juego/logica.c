#include "tetris.h"

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

int colision(int rango[FIL][COL], PiezaActual pieza, int nueva_x, int nueva_y) {
  for (int i = 0; i < PIEZA_SIZE; i++) {
    for (int j = 0; j < PIEZA_SIZE; j++) {
      if (pieza.forma[i][j] == 1) {
        int pos_x = nueva_x + j;
        int pos_y = nueva_y + i;

        if (pos_x < 0 || pos_x >= COL || pos_y >= FIL) {
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

void fijar_pieza(int rango[FIL][COL], PiezaActual pieza) {
  for (int i = 0; i < PIEZA_SIZE; i++) {
    for (int j = 0; j < PIEZA_SIZE; j++) {
      if (pieza.forma[i][j] == 1) {
        int pos_y = pieza.y + i;
        int pos_x = pieza.x + j;
        if (pos_y >= 0 && pos_y < FIL && pos_x >= 0 && pos_x < COL) {
          rango[pos_y][pos_x] = 1;
        }
      }
    }
  }
}

void rotar_pieza(PiezaActual *pieza, int rango[FIL][COL]) {
  int min_i = PIEZA_SIZE, max_i = -1;
  int min_j = PIEZA_SIZE, max_j = -1;

  for (int i = 0; i < PIEZA_SIZE; i++) {
    for (int j = 0; j < PIEZA_SIZE; j++) {
      if (pieza->forma[i][j] == 1) {
        if (i < min_i) min_i = i;
        if (i > max_i) max_i = i;
        if (j < min_j) min_j = j;
        if (j > max_j) max_j = j;
      }
    }
  }

  if (max_i < 0) {
    return;
  }

  /* Se rota solo la caja que ocupa la pieza para no desplazarla. */
  int alto = max_i - min_i + 1;
  int ancho = max_j - min_j + 1;
  int original[PIEZA_SIZE][PIEZA_SIZE] = {0};
  int girada[PIEZA_SIZE][PIEZA_SIZE] = {0};

  for (int i = 0; i < alto; i++) {
    for (int j = 0; j < ancho; j++) {
      original[i][j] = pieza->forma[min_i + i][min_j + j];
    }
  }

  for (int i = 0; i < alto; i++) {
    for (int j = 0; j < ancho; j++) {
      girada[j][alto - 1 - i] = original[i][j];
    }
  }

  /* La caja girada mide ancho x alto; se ajusta para seguir dentro de 4x4. */
  int dest_i = min_i;
  int dest_j = min_j;
  if (dest_i + ancho > PIEZA_SIZE) {
    dest_i = PIEZA_SIZE - ancho;
  }
  if (dest_j + alto > PIEZA_SIZE) {
    dest_j = PIEZA_SIZE - alto;
  }

  PiezaActual prueba = *pieza;
  for (int i = 0; i < PIEZA_SIZE; i++) {
    for (int j = 0; j < PIEZA_SIZE; j++) {
      prueba.forma[i][j] = 0;
    }
  }

  for (int i = 0; i < ancho; i++) {
    for (int j = 0; j < alto; j++) {
      prueba.forma[dest_i + i][dest_j + j] = girada[i][j];
    }
  }

  if (!colision(rango, prueba, prueba.x, prueba.y)) {
    *pieza = prueba;
  }
}

int limpiar_lineas(int rango[FIL][COL]) {
  int lineas_limpiadas = 0;

  for (int i = FIL - 1; i >= 0; i--) {
    int fila_llena = 1;

    for (int j = 0; j < COL; j++) {
      if (rango[i][j] == 0) {
        fila_llena = 0;
        break;
      }
    }
    if (fila_llena) {
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
