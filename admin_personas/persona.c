#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persona.h"

struct Persona *personas;

int cantidad = 0;
int agregar(){

    if(cantidad>= 5){
        printf("NO SE PUEDE AGREGAR A MAS PERSONAS");
        return 0;   
    }
    
    printf(
        "**************************\n"
        "MENU PARA AGREGAR PERSONAS\n"
        "**************************\n"
        "RECUERDA QUE ES UN MAXIMO DE 5\n"
    );
    printf("NOMBRE \n");
    scanf("%49s", personas[cantidad].nombre); 
   

    printf("EDAD: \n");
    scanf("%d", &personas[cantidad].edad);
    cambio(&personas[cantidad].edad);

    printf("CORREO ELECTRONICO: \n");
    scanf("%49s", personas[cantidad].correo);

    printf("NUMERO DE TELEFONO: \n");
    scanf("%19s", personas[cantidad].telefono);

    printf("NOMBRE : %s EDAD: %d  CORREO: %s TELEFONO %s \n", personas[cantidad].nombre,personas[cantidad].edad,personas[cantidad].correo,personas[cantidad].telefono);   
    cantidad ++;

    return 1;

}

int editar(){
     if (cantidad == 0) {
        printf("No hay personas para editar.\n");
        return 0;
    }
    int num_persona;
    int repeticion;
    int editar;
    while(1){
        
        printf(
            "**************************\n"
            "MENU PARA EDITAR PERSONAS\n"
            "**************************\n"
        );
        printf("QUE PERSONA QUIERES EDITAR \n");
        ver();
        scanf("%d", &num_persona);
        if (num_persona < 1 || num_persona > cantidad){
            printf("Persona no encontrada. Intenta de nuevo.\n");
            continue;
        }
        num_persona --;
        while (1){
            printf("QUE QUIERES EDITAR DE %s\n", personas[num_persona].nombre);
            printf("1. NOMBRE\n");
            printf("2. EDAD\n");
            printf("3. CORREO\n");
            printf("4. TELEFONO\n");
            printf("PARA SALIR ESCRIBE 0\n");
            scanf("%d", &editar);

            if(editar == 1){
                printf("ESCRIBE UN NUEVO NOMBRE PARA %s\n", personas[num_persona].nombre);
                scanf("%49s", personas[num_persona].nombre);
            }else if(editar == 2){
                    printf("ESCRIBE LA EDAD DE %s\n", personas[num_persona].nombre);
                    scanf("%d", &personas[num_persona].edad);
                    cambio(&personas[num_persona].edad);

            }else if(editar == 3){
                printf("ESCRIBE UN NUEVO CORREO PARA %s\n", personas[num_persona].nombre);
                scanf("%49s", personas[num_persona].correo);

            }else if(editar == 4){
                    printf("ESCRIBE NUEVO TELEFONO PARA %s\n", personas[num_persona].nombre);
                    scanf("%19s", personas[num_persona].telefono);
                
            }else if(editar == 0){
                break;
            }
        }
        printf("QUIERES EDITAR A OTRA PERSONA?\n");
        printf("1. SI\n");
        printf("0. NO\n");
        scanf("%d", &repeticion);
        if (repeticion == 0 ){
            break;
        }
    }
    return 1;
    
}

int ver(){
    if (cantidad == 0){
        printf("NO HAY PERSONAS EN LA LISTA.\n");
        return 0;
    }
    for(int i = 0; i < cantidad; i++){
        printf("Persona %d\n", i + 1);
        mpersonas(&personas[i]);
        }
    return 1;

}

int eliminar(){
int eliminar;
    int verdad;
    if (ver() == 1){
        while (1)
        {   
            if (cantidad == 0) {
        printf("No hay personas para eliminar.\n");
        return 0;
    }


            printf("¿QUE PERSONA QUIERES ELIMINAR ?(Ingresa el número de la persona, 1-%d)\n", cantidad);
            printf("SI TE EQUIVOCASTE ESCRIBE 00\n");
            scanf("%d",&eliminar);

            if (eliminar == 00 ){
                break;
            }
            if (eliminar < 1 || eliminar > cantidad) {
                printf("Número inválido. Intenta de nuevo.\n");
                continue;
            }
            eliminar --;

            for (int i = eliminar; i < cantidad -1 ; i ++){
                personas[i] = personas[i+1];
            }

            cantidad --;
            printf("SE ELIMINO CORRECTAMENTE\n"); 
            printf("----------------------\n");
            printf("DESEAS ELIMINAR A ALGUIEN MAS ?\n");
            printf("1. SI\n");
            printf("0. NO\n");
            scanf("%d", &verdad);
            if(verdad == 0){
                break;
            }
        }
    }
    return 1;

}

void cambio(int *x){
    *x = *x +1;

}

void mpersonas(struct Persona *p)
{
        printf("Nombre: %s\n", (*p).nombre);
        printf("Edad: %d\n", (*p).edad);
        printf("Correo: %s\n", (*p).correo);
        printf("Telefono: %s\n",(*p).telefono);
        printf("----------------------\n");
        printf("----------------------\n");   
};