#ifdef _CHUNK_H_
typedef struct porte_s porte_t; 
#endif

#ifndef _PORTE_H_
#define _PORTE_H_
#include "chunk.h"

typedef struct porte_s{
    void (*lire)(const struct porte_s * );
    err_t (*detruire)(struct porte_s **);
    struct porte_s * porteDest;
    int position;
}porte_t;

/* FONCTIONS */
extern booleen_t porte_existe(porte_t * porte);
extern porte_t * porte_creer ( porte_t * porteDest, int pos);

#endif