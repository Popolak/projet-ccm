#include <personnage.h>
#include <stdio.h>
#include <stdlib.h>

extern
void perso_existe( perso_t * const personnage )
{
  if( personnage == NULL )
    printf("FAUX\n");
  else
    printf("VRAI\n");
}

static
void perso_detruire( perso_t ** personnage){ //correspond à la mort du personnage ?

	free((*personnage)->nom);
	free((*personnage)->nom_sprite);
	free(*personnage);

	*personnage = NULL;

	return 0;
}

static
void perso_animer( perso_t * const personnage ){





}


extern
perso_t * perso_creer(char * nom, int vie, int taille, pos_t * position, char * nom_sprite, int vitesse){

	perso_t * personnage = NULL;

	personnage = malloc(sizeof(perso_t));
	personnage->nom = malloc(sizeof(char)*25);
	personnage->vie = vie;
	personnage->taille = taille;
	personnage->nom_sprite = malloc(sizeof(char)*25);
	personnage->vitesse = vitesse;
	personnage->position->x;
	personnage->position->y;

	personnage->animer = perso_animer;
	personnage->detruire = perso_detruire;

	return(personnage);
}
