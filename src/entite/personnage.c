#include <stdio.h>
#include <stdlib.h>

#include "../../lib/entite/personnage.h"

/* Fonctions */

static
err_t perso_depop(perso_t * perso){
	perso->chunk=NULL;
	perso->salle=NULL;
	return OK;
}

static 
err_t perso_copie_partiel(perso_t * persoDst, perso_t * persoSrc){
	persoDst->pos=persoSrc->pos;
	persoDst->vitesse_x=persoSrc->vitesse_x;
	persoDst->vitesse_y=persoSrc->vitesse_y;
	persoDst->salle=persoSrc->salle;
	persoDst->chunk=persoSrc->chunk;
	return OK;
}

extern
booleen_t perso_existe( perso_t * const personnage )
{
  if( personnage == NULL )
    return FAUX;
  else
    return VRAI;
}

/*
	perso_detruire
	paramètre:
		pointeur sur pointeur sur perso_t
	retourne OK si tout s'est bien passé
	libère l'espace du personnage et toutes les allocation reliées a lui
*/

static
err_t perso_detruire( perso_t ** personnage){  
	pos_t pos={0,0};
	entite_t * ent=NULL;
	entite_t * ent2=entite_creer("","",NULL,NULL,pos,0,0,0,0,0,0,0,0,0,NULL);
	if(!(*personnage))
		return OK;


	ent=realloc((entite_t*)(*personnage),sizeof(entite_t));
	*personnage=NULL;
	ent->detruire=ent2->detruire;
	ent2->detruire(&ent2);
	if(!ent){
		printf("La réallocation de personnage pour sa destruction a échouée");
		return PAS_DALLOC;
	}
	ent->detruire(&ent);
	return OK;
}

/*
	fait_partie_bin
	paramètres:
		int tot_bin : nombre total
		int nombre_puis_2 : puissance de 2 pour vérifier si un certain bit appartient a tot_bin
	retourne un nombre si vrai, 0 sinon
*/
extern 
int fait_partie_bin (int tot_bin, int nombre_puis_2){
	return (tot_bin & nombre_puis_2);
}

/*
	input_update_speed
	paramètres:	
		pointeur sur perso_t
		int tot_touche, le total des touches (ex: 110 donne KEY_LEFT et KEY_JUMP)
	
	En fonction du total de touches on actualise la vitesse du personnage
*/
static 
void input_update_speed (perso_t * perso, int tot_touche){
	float vit_tempo;
	if(fait_partie_bin(tot_touche,KEY_JUMP) && !perso->en_l_air((entite_t*)perso)){
		perso->vitesse_x=-perso->vitesse_saut;
	}
	
	if(!(fait_partie_bin(tot_touche, KEY_LEFT) && fait_partie_bin(tot_touche, KEY_RIGHT))){
		if(fait_partie_bin(tot_touche, KEY_LEFT)){
			perso->vitesse_y=-perso->vitesse_max_y;
		}
		else if(fait_partie_bin(tot_touche, KEY_RIGHT)){
			perso->vitesse_y=perso->vitesse_max_y;
		}
	}
	
}

static
void perso_prendre_coup(perso_t * personnage, int degats){
	personnage->vie -= degats;
}

static
booleen_t en_vie(perso_t * personnage){

    if(personnage->vie > 0)
        return VRAI;
    else
        return FAUX;

}

extern
perso_t * perso_creer(char * nom, 
					 char *description,
					 int vie,
					 salle_t * salle,
					 chunk_t * chunk,
					 pos_t pos,
					 float vitesse_x, float vitesse_y, float vitesse_max_y,float vitesse_saut,
				     int w, int h, 
					 int w_hitbox, int h_hitbox,
					 float secSprite,
					 float vit_attack, int degats,
					 int nbTextures,
					 SDL_Texture ** textures)
{
	perso_t * personnage = NULL;
	if ((personnage=(perso_t*)entite_creer(nom,description,salle,chunk,pos,vitesse_x,vitesse_y,vitesse_max_y,secSprite,w,h,w_hitbox,h_hitbox,nbTextures,textures))==NULL){
		return NULL;
	}
	personnage=realloc(personnage,sizeof(perso_t));
	if(personnage==NULL){
		printf("La réallocation du personnage %s n'a pas pu etre effectuée\n", nom);
		personnage->detruire(&personnage);
		return NULL;
	}
	personnage->vitesse_saut=vitesse_saut;
	personnage->vie = vie;
	personnage->vit_attack = vit_attack;
	personnage->degats = degats;

	personnage->detruire = perso_detruire;
	personnage->prendre_coup=perso_prendre_coup;
	personnage->update_speed=input_update_speed;
	personnage->copie_partiel=perso_copie_partiel;
	personnage->depop=perso_depop;

	return(personnage);
}

