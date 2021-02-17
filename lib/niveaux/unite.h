#ifndef _UNITE_H_
#define _UNITE_H_

#include "../commun.h"

/*Structure unite*/

typedef struct unite_s{
    int (*lire) (const struct unite_s * );
    err_t (*detruire)(struct unite_s ** );
    err_t (*ecrire)(struct unite_s ** , int );
    int contenu;
}unite_t;

/* FONCRIONS EXTERNES */
extern unite_t * unite_creer(int contenu);
extern booleen_t unite_existe(unite_t * unite);
extern void unite_afficher_ref();

#endif