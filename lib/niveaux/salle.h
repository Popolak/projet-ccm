typedef struct salle_s salle_t;

#ifndef _SALLE_H_
#define _SALLE_H_
#include "./chunk.h"

typedef struct salle_s{
    void (*lire)(const struct salle_s * );
    err_t (*detruire)(struct salle_s ** );
    chunk_t * (*chercher_chunk)(const struct salle_s * , int , int );
    int (*max_x)(struct salle_s * );
    int (*max_y)(struct salle_s * );
    chunk_t ** chunks;    //Tableau de chunks
    int  nb_chunk;        //Nombre de chunks
    pos_t position;       //Position de la salle dans le niveau
}salle_t;

extern booleen_t salle_existe (salle_t *salle);
extern salle_t * salle_creer_type(char *type);
extern salle_t * salle_creer(char * type);
extern void salle_afficher_ref();
extern booleen_t type_correct(char * type);

#endif
