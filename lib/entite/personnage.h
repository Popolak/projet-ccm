#ifndef _PERSO_H_
#define _PERSO_H_

#include "../commun.h"
#include "entite.h"

typedef struct perso_s
{
	err_t (*detruire)(struct perso_s ** ent);
    void (*lire)(struct perso_s * ent);
	err_t (*afficher_chunk)(SDL_Renderer *ren,struct perso_s *entite,int WINH,int WINW);
    err_t (*afficher_fenetre)(SDL_Renderer * ren,struct perso_s * entite, int w, int h, int x, int y, SDL_Texture * texture);
	int (*contact_obstacle)(struct perso_s * );
	#include "attribut_perso.h"
	void (*prendre_coup)(struct perso_s * , int );
	void (*animer)(struct perso_s * const);
	
}perso_t;

extern
perso_t * perso_creer(char * nom, char * desc,
					 int vie,
					 pos_t position,
					 int w, int h, int w_hitbox, int h_hitbox,
					 int nbTextures,SDL_Texture ** textures,
					 int vitesse_x, int vitesse_y, int vitesse_max, 
					 int vit_attack, int degats, 
					 chunk_t *chunk, salle_t * salle);

extern booleen_t perso_existe( perso_t * const personnage );

#endif