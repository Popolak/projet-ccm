#include <stdio.h>
#include <stdlib.h>
#include "porte.h"

static void porte_lire(const porte_t * porte){
}

static err_t porte_detruire(porte_t ** porte){
    if(*porte){
        if((*porte)->chunk){
            free(((*porte)->chunk));
            (*porte)->chunk=NULL;
        }
        if((*porte)->porteDest){
            free((*porte)->porteDest);
            (*porte)->porteDest=NULL;
        }
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

extern porte_t * porte_creer (chunk_t ** chunk, porte_t * porteDest, int pos){
    porte_t * porte= malloc(sizeof(porte_t));
    if (!porte)
        return NULL;
    porte->chunk=malloc(sizeof(chunk_t*));
    porte->porteDest=malloc(sizeof(porte_t*));
    if (!porte->chunk ){
        printf("L'allocation de la porte n'a pas eu lieu");
        porte_detruire(&porte);
        porte_detruire(&porteDest);
        return NULL;
    }
    porte->detruire=porte_detruire;
    porte->lire=porte_lire;
    porte->chunk=&chunk;
    porte->porteDest=&porteDest;
    porte->position=pos;
    return porte;
}
