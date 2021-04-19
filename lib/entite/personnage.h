
/* Par Matthis */

typedef struct perso_s perso_t;

#ifndef _PERSO_H_
#define _PERSO_H_

#include "../commun.h"
#include "entite.h"

/* Constantes */

#define NO_KEYS 0		//Touches pour le déplacement (changera peut etre de fichier)
#define KEY_RIGHT 1
#define KEY_LEFT 2
#define KEY_JUMP 4
#define KEY_ATT 8
#define KEY_CHANGE_R 128
#define KEY_CHANGE_L 256
#define KEY_HB 1024
#include "entite.h"
#include "attaque.h"

/* Structure */

typedef struct perso_s
{
	err_t (*detruire_ent)(struct entite_s ** );
	#include "attribut_entite.h" 
	err_t (*detruire)(struct perso_s ** ent); 
	#include "attribut_perso.h"
	
}perso_t;

/* Fonctions */
extern
perso_t * perso_creer(char * nom, 
					 char *description,
					 int vie,
					 salle_t * salle,
					 chunk_t * chunk,
					 pos_t pos,
					 float vitesse_x, float vitesse_y, float vitesse_max_y,float vitesse_saut,
				     int w, int h, 
					 int w_hitbox, int h_hitbox,int offset_hitbox,
					 float secSprite,
					 float vit_attack, int degats, char * nom_attaque,
					 int nbTextures,
					 SDL_Texture ** textures);

extern booleen_t perso_existe( perso_t * const personnage );

/*
	fait_partie_bin
	paramètres:
		int tot_bin : nombre total
		int nombre_puis_2 : puissance de 2 pour vérifier si un certain bit appartient a tot_bin
	retourne un nombre si vrai, 0 sinon
*/
extern int fait_partie_bin (int tot_bin, int nombre_puis_2);

extern 
err_t perso_detruire( perso_t ** personnage);

static
int creer_ajouter_attaque(SDL_Renderer *ren,attaque_t ** tab, perso_t * perso, FILE * index, char * appel );


#endif