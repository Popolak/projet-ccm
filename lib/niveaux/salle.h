#ifndef _SALLE_H_
#define _SALLE_H_
#include "chunk.h"

typedef struct salle_s{
    void (*lire)(const struct salle_s * );
    err_t (*detruire)(struct salle_s ** );
    chunk_t ** chunks;
    char * type;
    int  nb_chunk, id_salle;
    int x,y;
}salle_t;

extern booleen_t salle_existe (salle_t *salle);
extern salle_t * salle_creer_type();
extern salle_t * salle_creer(char * type);
extern void salle_afficher_ref();

#endif
