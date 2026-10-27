#ifndef PLATFORM_H
#define PLATFORM_H

#include "../tetris.h"

int configurar_terminal(void);
void restaurar_terminal(void);
void pausa_ms(unsigned int milisegundos);
int tecla_presionada(void);
int leer_tecla(void);

#endif
