#include <stdio.h>
#include <stdlib.h>

#include "../../lib/personnages/personnage.h"

extern
booleen_t perso_existe( perso_t * const personnage )
{
  if( personnage == NULL )
    return FAUX;
  else
    return VRAI;
}

static
err_t perso_detruire( perso_t ** personnage){ //correspond à la mort du personnage ?

	free((*personnage)->nom);
	free((*personnage)->nom_sprite);
	free(*personnage);

	*personnage = NULL;

	return OK;
}

static
void perso_animer( perso_t * const personnage ){





}

static
void prendre_coup(perso_t * personnage, int degats){
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
perso_t * perso_creer(char * nom, int vie, int taille, pos_t * position, char * nom_sprite, int vitesse, int degats){

	perso_t * personnage = NULL;

	personnage = malloc(sizeof(perso_t));
	personnage->nom = malloc(sizeof(char)*25);
	personnage->vie = vie;
	personnage->taille = taille;
	personnage->nom_sprite = malloc(sizeof(char)*25);
	personnage->vitesse = vitesse;
	personnage->position->x;
	personnage->position->y;
	personnage->degats = degats;

	personnage->animer = perso_animer;
	personnage->detruire = perso_detruire;
	personnage->prendre_coup=prendre_coup;
	personnage->en_vie=en_vie;

	return(personnage);
}

