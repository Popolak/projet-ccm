#include <stdio.h>
#include <stdlib.h>
#include "../../lib/niveaux/porte.h"

static void porte_lire(const porte_t * porte){
}

static err_t porte_detruire(porte_t ** porte){
    printf("%p\n", (*porte));
    if(*porte){
        (*porte)->porteDest=NULL;
        free(*porte);
        *porte=NULL;
    }
    return OK;
}


extern booleen_t porte_existe(porte_t * porte){
    if (porte)
        return VRAI;
    return FAUX;
}

extern porte_t * porte_creer (porte_t * porteDest, int pos){
    porte_t * porte= malloc(sizeof(porte_t));
    if (!porte)
        return NULL;
    porte->porteDest=NULL;
    porte->detruire=porte_detruire;
    porte->lire=porte_lire;
    porte->porteDest=porteDest;
    porte->position=pos;
    return porte;
}
