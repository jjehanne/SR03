#include "defobj.h"
#include <stdlib.h>
#include <string.h>
#define TABLEN 3

obj* iniobj(){
    int j;

    //Creation d'une liste d'objet vide
    obj* liste = (obj*) malloc(TABLEN*sizeof(obj));
    if (liste == NULL){
        printf("malloc: Erreur d'allocation memoire");
        return NULL;
    }

    //Remplissage de la liste
    for(j=0; j < TABLEN; j++){
        strcpy(liste[j].id,"");
        strcpy(liste[j].desc,"");
        liste[j].ii = 0;
        liste[j].jj = 0;
        liste[j].dd = 0;
        liste[j].iqt = 0;
    }
    return liste;

}

obj* createList(){
    //Creation d'une liste d'objet vide
    obj* liste = (obj*) malloc(TABLEN*sizeof(obj));
    if (liste == NULL){
        printf("malloc: Erreur d'allocation memoire");
        return NULL;
    }

    //Remplissage de la liste
    
    strcpy(liste[0].id,"ident_o1");
    strcpy(liste[0].desc,"description_o1");
    liste[0].ii = 11;
    liste[0].jj = 12;
    liste[0].dd = 10.2345;
    liste[0].iqt = 0;

    strcpy(liste[1].id,"ident_o2");
    strcpy(liste[1].desc,"description_o2");
    liste[1].ii = 21;
    liste[1].jj = 22;
    liste[1].dd = 20.2345;
    liste[1].iqt = 0;

    strcpy(liste[2].id,"ident_o3");
    strcpy(liste[2].desc,"description_o3");
    liste[2].ii = 31;
    liste[2].jj = 32;
    liste[2].dd = 30.2345;
    liste[2].iqt = -1;



    return liste;

}
