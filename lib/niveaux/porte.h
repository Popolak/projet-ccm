/* Par Matthis */

typedef struct porte_s porte_t;

#ifndef _PORTE_H_
#define _PORTE_H_
#include "../commun.h"
#include "chunk.h"

enum {HG,BG,HD,BD};


typedef struct porte_s{
    void (*lire)(const struct porte_s * );
    err_t (*detruire)(struct porte_s **);
    struct porte_s * porteDest;                   //Porte de destination
    chunk_t *chunk;
    int position;                                 //0 :haut a gauche, 1: en bas a gauche, 2: en haut a droite, 3: en bas a droite
}porte_t;

/* FONCTIONS */
extern booleen_t porte_existe(porte_t * porte);
extern porte_t * porte_creer ( porte_t * porteDest, int pos, chunk_t *chunk);
extern booleen_t pos_correcte(int pos);

#endif