#ifndef _CHUNK_H_
#define _CHUNK_H_
#include "unite.h"
#include "porte.h"

static const int CHUNKH=300, CHUNKW=533;  //Hauteur et épaisseur des chunks en unité (ratio ~16/9)

typedef struct chunk_s{
    void (*lire)(const struct chunk_s * );
    err_t (*detruire)(struct chunk_s ** );                                  //méthodes
    void (*lire_partiel)(const struct chunk_s * ,int ,int ,int ,int );
    porte_t * (*chercher_porte)(const struct chunk_s *, int);
    
    
    unite_t *** chunk;  //matrice d'unités
    porte_t ** portes;  //Tableau de portes
    int nb_portes;      //Taille du tableau
    pos_t position;     //Position du chunk dans la salle
} chunk_t;

extern booleen_t chunk_existe(chunk_t * chunk);
extern chunk_t * chunk_creer(int x,int y,int nb_portes, char * type);
extern void chunk_afficher_ref();
#endif