#include <stdlib.h>
#include "../../lib/niveaux/porte.h"

/**
* \file porte.c
* \brief Module porte
* \verion 1.0
* \date Avril 2021
*/

/* FONCTIONS */

/**
    \brief Vérification de position d'une porte
    \param int Position de la porte
    \return Retourne VRAI si la position est correcte, FAUX sinon
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

/**
    \brief Destruction d'une porte
    \param porte_t Pointeur sur pointeur sur la porte à détruire
    \return Retourne OK après la suppression 
*/
static err_t porte_detruire(porte_t ** porte){
    if(*porte){
        (*porte)->porteDest=NULL;
        free(*porte);
        *porte=NULL;
    }
    return OK;
}

/**
    \brief Vérification d'existence d'une porte
    \param porte_t Pointeur sur la porte à tester
    \return Retourne VRAI si la porte existe, sinon FAUX
*/
extern booleen_t porte_existe(porte_t * porte){
    if (porte)
        return VRAI;
    return FAUX;
}

/**
    \brief Création d'une porte
    \param porte_t Pointeur sur porte de destination
    \param int Position de la porte
    \param salle_t Salle où est placée la porte
    \param chunk_t Chunk de la porte
    \return Retourne un pointeur sur porte_t si la création s'est bien passé, NULL sinon 
*/
extern porte_t * porte_creer (porte_t * porteDest, int pos, salle_t * salle,chunk_t *chunk){
    porte_t * porte= malloc(sizeof(porte_t));
    if (!porte || !pos_correcte(pos)){
        if(porte)
            free(porte);
        return NULL;
    }
    porte->detruire=porte_detruire;             //affectation des méthodes
    porte->lire=porte_lire;
    porte->porteDest=porteDest;
    porte->salle=salle;
    porte->chunk=chunk;
    porte->position=pos;
    return porte;
}
