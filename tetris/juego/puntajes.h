#ifndef PUNTAJES_H
#define PUNTAJES_H

/*
 * Persistencia de records en texto plano.
 *
 * El archivo tiene una linea por record con el formato:
 *     nombre|puntaje
 *
 * Las lineas que empiezan por '#' son comentarios y se ignoran.
 */

#define MAX_PUNTAJES 10
#define MAX_NOMBRE 20
#define RUTA_PUNTAJES "puntajes.txt"
#define NOMBRE_POR_DEFECTO "Anonymous"

typedef struct {
    char nombre[MAX_NOMBRE + 1];
    int puntaje;
} Puntaje;

/*
 * Copia 'entrada' en 'salida' recortando espacios, quitando caracteres
 * de control y sustituyendo '|' por '-'. Si el resultado queda vacio,
 * usa NOMBRE_POR_DEFECTO. 'tam' es el tamano total del buffer de salida.
 */
void sanear_nombre(const char *entrada, char *salida, int tam);

/*
 * Carga hasta 'maximo' records ordenados de mayor a menor puntaje.
 * Devuelve la cantidad cargada, 0 si el archivo no existe y -1 si
 * hubo un error real de lectura.
 */
int cargar_puntajes(const char *ruta, Puntaje *lista, int maximo);

/*
 * Guarda 'cantidad' records en 'ruta' usando un archivo temporal.
 * Devuelve 0 si todo salio bien y -1 si hubo un error de escritura.
 */
int guardar_puntajes(const char *ruta, const Puntaje *lista, int cantidad);

/*
 * Indica si un puntaje entra en la tabla. La lista debe estar ordenada.
 * Los puntajes <= 0 no califican.
 */
int puntaje_califica(const Puntaje *lista, int cantidad, int puntaje, int maximo);

/*
 * Inserta un puntaje si califica y mantiene la lista ordenada.
 * Devuelve 1 si se inserto y 0 si no entro en la tabla.
 */
int insertar_puntaje(Puntaje *lista, int *cantidad, const char *nombre,
                     int puntaje, int maximo);

#endif
