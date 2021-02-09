#ifndef _NIV_H_
#define _NIV_H_
#include "salle.h"


typedef struct niveau_s{
    void (*lire)(const struct niveau_s * );
    err_t (*detruire)(struct niveau_s ** );
    int nbSalle;
    salle_t ** salles;
} niveau_t;


#endif