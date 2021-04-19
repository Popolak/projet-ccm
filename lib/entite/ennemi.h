/*Par Matthis*/

#ifndef _ENNEMI_H_
#define _ENNEMI_H_

#include "personnage.h"
#include "../commun.h"
#include "entite.h"

//Definition de la structure ennemi
typedef struct ennemi_s{
	err_t (*detruire_ent)(struct entite_s ** );
	#include "attribut_entite.h"
	err_t (*detruire_perso)(struct perso_s ** ); 
	#include "attribut_perso.h"
	err_t (*detruire)(struct ennemi_s ** );

	booleen_t (*dans_range)(struct ennemi_s * ennemi,entite_t *ent );
	
	
	int range;

} ennemi_t;


//Fonctions
extern
ennemi_t * ennemi_creer(char * nom, 
					 char *description,
					 int vie,
					 salle_t * salle,
					 chunk_t * chunk,
					 pos_t pos,
					 float vitesse_x, float vitesse_y, float vitesse_max_y,float vitesse_saut,
				     int w, int h, 
					 int w_hitbox, int h_hitbox,int offset_hitbox,
					 float secSprite,
					 float vit_attack, int degats, char * nom_attaque,int range,
					 int nbTextures,
					 SDL_Texture ** textures);


#endif
