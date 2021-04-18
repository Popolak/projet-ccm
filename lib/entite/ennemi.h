#ifndef _ENNEMI_H_
#define _ENNEMI_H_

#include "personnage.h"
#include "../commun.h"
#include "entite.h"

//Definition de la structure ennemi
//Certains éléments sont hérité de l'entité
typedef struct ennemi_s{
	char * type;
	int vit_attaque;
	int attaque;
	int portee;
	int vie;
	int defense;

	err_t (*ennemi_detruire)(struct ennemi_s ** ent);   
	booleen_t (*envie)(struct ennemi_s * ennemi);
	pos_t (*mouvement_verti)(ennemi_s * );
	pos_t (*mouvement_late)(ennemi_t * , int);
	pos_t (*ennemi_aller_vers)(ennemi_t * , pos_t );
	pos_t (*ennemi_position)(ennemi_t * );
	int (*ennemi_degats_entrant)(ennemi_t * , int);
	int (*ennemi_degats_sortant)(ennemi_t * );
	int (*ennemi_blesser)(ennemi_t * , perso_t * );
	err_t (*ennemi_effacer)(ennemi_t ** );


	//inventaire * objets;
} ennemi_t;


//Fonctions
extern ennemi_t * ennemi_creer(char * nom, 
					 char *description,
					 char * type,
					 int vie,
					 int defense,
					 salle_t * salle,
					 chunk_t * chunk,
					 pos_t pos,
					 float vitesse_x, float vitesse_y, float vitesse_max_y,float vitesse_saut,
				     int w, int h, 
					 int w_hitbox, int h_hitbox,int offset_hitbox,
					 float secSprite,
					 float vit_attack,
					 int degats,
					 char * nom_attaque,
					 int nbTextures,
					 SDL_Texture ** textures) ;

extern booleen_t ennemi_existe(ennemi_t * const ennemi) ;

