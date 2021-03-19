#ifndef _JOUEUR_H_
#define _JOUEUR_H_

#include "personnage.h"

extern void creation(perso_t * tomate, perso_t  * carotte, perso_t * viande, chunk_t chunk_dep, salle_t salle_dep);
extern void deplacement(perso_t *perso_control.position, int direction);
extern void saut(*perso_control.position);
extern perso_t attaque1(perso_t perso_control);
extern perso_t attaque2(perso_t perso_control);

#endif