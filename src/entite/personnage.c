#include <stdio.h>
#include <stdlib.h>

#include "../../lib/entite/personnage.h"

extern
booleen_t perso_existe( perso_t * const personnage )
{
  if( personnage == NULL )
    return FAUX;
  else
    return VRAI;
}

static
err_t perso_detruire( perso_t ** personnage){ //ré-alloue l'espace donné au personnage, meme après la mort d'un personnage on peut garder cet espace 
	pos_t pos={0,0};
	entite_t * ent=NULL;
	entite_t * ent2=entite_creer("oui","oui",pos,0,0,0,0,0,NULL,NULL,NULL,0,0);
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

static
void perso_animer( perso_t * const personnage ){





}

static
void perso_prendre_coup(perso_t * personnage, int degats){
	personnage->vie -= degats;
}

extern
perso_t * perso_creer(char * nom, char * desc,
					 int vie,
					 pos_t position,
					 int w, int h, int w_hitbox, int h_hitbox,
					 int nbTextures,SDL_Texture ** textures,
					 float vitesse_x, float vitesse_y, float vitesse_max, 
					 float vit_attack, int degats, 
					 chunk_t *chunk, salle_t * salle){

	perso_t * personnage = NULL;
	if ((personnage=(perso_t*)entite_creer(nom,desc,position,w,h,w_hitbox,h_hitbox,nbTextures,textures,chunk,salle,vitesse_x,vitesse_y))==NULL){
		return NULL;
	}
	personnage=realloc(personnage,sizeof(perso_t));
	if(personnage==NULL){
		printf("La réallocation du personnage %s n'a pas pu etre effectuée\n", nom);
		personnage->detruire(&personnage);
		return NULL;
	}
	personnage->vie = vie;
	personnage->vitesse_max = vitesse_max;
	personnage->vit_attack = vit_attack;
	personnage->degats = degats;

	personnage->animer = perso_animer;
	personnage->detruire = perso_detruire;
	personnage->prendre_coup=perso_prendre_coup;

	return(personnage);
}

