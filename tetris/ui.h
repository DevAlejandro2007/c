#ifndef UI_H
#define UI_H

typedef enum {
    MENU_JUGAR,
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
OpcionPausa menu_pausa(void);
OpcionGameOver menu_game_over(int puntaje);

#endif
