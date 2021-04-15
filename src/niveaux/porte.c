#include <stdlib.h>
#include "../../lib/niveaux/porte.h"

/* FONCTIONS */

/* fonction pos_correcte
    paramètre:
        pos: int, position de la porte
    retourne VRAI si la position est correcte, FAUX sinon
*/

extern
booleen_t pos_correcte(int pos){
    if(pos>=0 && pos<4)
        return VRAI;
    printf("Position incorrecte:%d\n", pos);
    return FAUX;
}

/* fonction porte_lire

*/

static void porte_lire(const porte_t * porte){
}

/*  fonction porte detruire
    parametre:
        porte : pointeur sur pointeur sur porte_t
    reourne OK si tout s'est bien passé 
*/

static err_t porte_detruire(porte_t ** porte){
    if(*porte){
        (*porte)->porteDest=NULL;
        free(*porte);
        *porte=NULL;
    }
    return OK;
}

/*  fonction porte_existe
    paramètre:
        porte: pointeur sur porte_t
    Retourne VRAI si la porte existe, sinon FAUX
*/

extern booleen_t porte_existe(porte_t * porte){
    if (porte)
        return VRAI;
    return FAUX;
}

/*  fonction porte_creer
    paramètres:     
        porteDest: pointeur sur porte_t
        pos: int
    Retourne un pointeur sur porte_t si la création s'est bien passé, FAUX sinon 
*/

extern porte_t * porte_creer (porte_t * porteDest, int pos, chunk_t *chunk){
    porte_t * porte= malloc(sizeof(porte_t));
    if (!porte || !pos_correcte(pos)){
        if(porte)
            free(porte);
        return NULL;
    }
    porte->detruire=porte_detruire;             //affectation des méthodes
    porte->lire=porte_lire;
    porte->porteDest=porteDest;
    porte->position=pos;
    return porte;
}
