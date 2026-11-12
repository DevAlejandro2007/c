#ifndef PLATFORM_H
#define PLATFORM_H

/*
 * Teclas especiales.
 * Usamos valores fuera del rango ASCII para no chocar con caracteres normales.
 */
enum Tecla {
    TECLA_ARRIBA = 0x100,
    TECLA_ABAJO,
    TECLA_IZQUIERDA,
    TECLA_DERECHA,
    TECLA_ENTER,
    TECLA_ESCAPE,
    TECLA_ESPACIO,
    TECLA_BACKSPACE
};

int configurar_terminal(void);
void restaurar_terminal(void);
void instalar_manejadores(void);
void pausa_ms(unsigned int milisegundos);
int tecla_presionada(void);
int leer_tecla(void);

void limpiar_pantalla(void);
void ocultar_cursor(void);
void mostrar_cursor(void);
void entrar_pantalla_alterna(void);
void salir_pantalla_alterna(void);

/*
 * Reemplaza 'destino' con 'origen'. En Windows, si rename() no puede
 * sobrescribir, se elimina el destino y se reintenta. Devuelve 0 si
 * todo salio bien y -1 si no se pudo completar el reemplazo.
 */
int reemplazar_archivo(const char *origen, const char *destino);

#endif
