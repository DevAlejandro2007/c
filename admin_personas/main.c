#include <stdio.h>
#include <stdlib.h>
#include "persona.h"

int main(){

    int opcion;

    personas = malloc(5 * sizeof(struct Persona));
        if(personas == NULL){
        printf("Error de memoria\n");
        return 1;
    }
    
    while(1){
        printf("################\n");
        printf("#MENU PRINCIPAL#\n");
        printf("################\n");
        printf("1. Agregar\n");
        printf("2. Editar\n");
        printf("3. Ver\n");
        printf("4. Eliminar\n");
        printf("5. Salir\n");
        printf("################\n");
        scanf("%d", &opcion);

        if(opcion == 1){
            agregar();
        }
        else if(opcion == 2){
            editar();
        }
        else if(opcion == 3){
            ver();
        }
        else if(opcion == 4){
            eliminar();
        }
        else if(opcion == 5){
            break;
        }
    }


    return 0;
}               