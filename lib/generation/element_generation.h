/*Par Matthis */

#ifndef _ELEM_GEN_H_
#define _ELEM_GEN_H_

#include "../entite/ennemi.h"
#include "../entite/attaque.h"

extern 
void * creer_entite_chaine(SDL_Renderer *ren,int *n, const perso_t const * joueur , char * chaine,  FILE * index, char * appel);


#endif