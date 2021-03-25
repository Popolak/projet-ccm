#ifndef _JOUEUR_H_
#define _JOUEUR_H_

#include "personnage.h"

#define NB_PERSO 3

typedef struct joueur_s{
    perso_t ** personnages;
    int perso_c;
}joueur_t;



extern void creation(SDL_Renderer * ren,perso_t * tomate, perso_t  * carotte, perso_t * viande,chunk_t * chunk_dep, salle_t *salle_dep);
extern void deplacement(perso_t *perso_control, int direction);
extern perso_t attaque1(perso_t perso_control);
extern perso_t attaque2(perso_t perso_control);


#endif