#ifndef _NIV_H_
#define _NIV_H_
#include "salle.h"


typedef struct niveau_s{
    void (*lire)(const struct niveau_s * );
    err_t (*detruire)(struct niveau_s ** );
    salle_t * (*chercher_salle)(const struct niveau_s * , int , int );
    int nbSalle;                //nombre de salle
    salle_t ** salles;          //tableau de salles
} niveau_t;

extern booleen_t niveau_existe(niveau_t * niveau);
extern niveau_t * niveau_creer(char *nom_fichier);
extern void relier_portes(FILE * fichier,niveau_t * niveau);
extern void niveau_afficher_ref();
#endif