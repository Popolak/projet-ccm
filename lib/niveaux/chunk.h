#ifdef _PORTE_H_
typedef struct chunk_s chunk_t;
#endif

#ifndef _CHUNK_H_
#define _CHUNK_H_
#include "unite.h"
#include "porte.h"

static const int CHUNKH=20, CHUNKW=20;

typedef struct chunk_s{
    void (*lire)(const struct chunk_s * );
    err_t (*detruire)(struct chunk_s ** );
    void (*lire_partiel)(const struct chunk_s * ,int ,int ,int ,int );
    porte_t * (*chercher_porte)(const chunk_t *, int);
    unite_t *** chunk;
    porte_t ** portes;
    int nb_portes;
    int x,y;
} chunk_t;

extern booleen_t chunk_existe(chunk_t * chunk);
extern chunk_t * chunk_creer(int x,int y,int nb_portes, char * type);
extern void chunk_afficher_ref();
#endif