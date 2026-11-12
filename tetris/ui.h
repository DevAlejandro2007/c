#ifndef UI_H
#define UI_H

#include "puntajes.h"

typedef enum {
    MENU_JUGAR,
    MENU_RECORDS,
    MENU_INSTRUCCIONES,
    MENU_SALIR
} OpcionMenu;

typedef enum {
    PAUSA_CONTINUAR,
    PAUSA_REINICIAR,
    PAUSA_MENU
} OpcionPausa;

typedef enum {
    GAMEOVER_REINTENTAR,
    GAMEOVER_MENU
} OpcionGameOver;

OpcionMenu menu_principal(void);
void pantalla_instrucciones(void);
void pantalla_nombre(char *destino, int tam);
void pantalla_records(const Puntaje *lista, int cantidad);
void pantalla_aviso(const char *mensaje);
OpcionPausa menu_pausa(void);
OpcionGameOver menu_game_over(int puntaje);

#endif
