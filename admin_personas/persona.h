#ifndef PERSONA_H
#define PERSONA_H

struct Persona
{
    char nombre[50];
    char correo[50];
    char telefono[20];
    int edad;
};


extern struct Persona *personas;
extern int cantidad;


int agregar();
int editar();
int ver();
int eliminar();
void cambio(int *x);
void mpersonas(struct Persona *p);

#endif