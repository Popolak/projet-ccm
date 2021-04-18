/*Par Matthis */

#ifndef _ELEM_GEN_H_
#define _ELEM_GEN_H_

#include "../entite/personnage.h"
#include "../entite/attaque.h"
#include "../entite/ennemi.h"

extern 
void * creer_entite_chaine(SDL_Renderer *ren,int *n, const void const * joueur , char * chaine,  FILE * index, char * appel);


#endif