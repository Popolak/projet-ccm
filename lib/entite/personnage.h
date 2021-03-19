#ifndef _PERSO_H_
#define _PERSO_H_

#include "../commun.h"
#include "entite.h"

typedef struct perso_s
{
	pos_t * position;
	err_t (*detruire)(struct perso_s ** ent);
	#include "attribut_perso.h"
	
}perso_t;

extern
perso_t * perso_creer(char * nom, char * desc,
					 int vie,
					 pos_t position,
					 int w, int h, int w_hitbox, int h_hitbox,
					 int nbTextures,SDL_Texture ** textures,
					 float vitesse_x, float vitesse_y, float vitesse_saut,float vitesse_max_y,
					 float vit_attack, int degats, 
					 chunk_t *chunk, salle_t * salle);

extern booleen_t perso_existe( perso_t * const personnage );
static void perso_prendre_coup(perso_t * personnage, int degats);
static booleen_t en_vie(perso_t * personnage);

#endif