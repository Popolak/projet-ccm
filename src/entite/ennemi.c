#include <stdlib.h>
#include <string.h>
#include "../../lib/entite/ennemi.h"

extern ennemi_t * ennemi_creer(char* nom, char* type, int taille, pos_t* position, char* nom_sprite, float vitesse, int vitAttaque, int attaque, int portee, int vie, int defense){
	ennemi_t * ennemi = NULL;

	ennemi = malloc(sizeof(perso_t));
	ennemi->nom = malloc(sizeof(char)*(strlen(nom)+1));
	ennemi->type = malloc(sizeof(char)*(strlen(type)+1));
	ennemi->taille = taille;
	ennemi->position = malloc(sizeof(pos_t));
	ennemi->nom_sprite = malloc(sizeof(char)*(strlen(nom_sprite))+1);
	ennemi->vitesse = vitesse;
	ennemi->vitAttaque = vitAttaque;
	ennemi->attaque = attaque;
	//ennemi->chunk = chunk;
	ennemi->portee = portee;
	ennemi->vie = vie;
	ennemi->defense = defense;

	return(ennemi);
}


extern err_t ennemi_detruire(ennemi_t ** ennemi){
	free((*ennemi)->nom);
	free((*ennemi)->position);
	free((*ennemi)->nom_sprite);
	free((*ennemi)->type);
	free((*ennemi));
	//inventaire_detruire
	
	(*ennemi) = NULL;

	return(OK);
}

extern err_t ennemi_effacer(ennemi_t ** ennemi){
	(*ennemi) = NULL;
	return(OK);
}

extern booleen_t ennemi_existe(ennemi_t * const ennemi){
	if(ennemi == NULL) 
		return(FAUX) ;
	else
		return(VRAI) ; 
}

extern void ennemi_afficher(ennemi_t * const ennemi){}

extern int ennemi_blesser(ennemi_t * attaquant, perso_t * cible){
	int dmg = ennemi_degats_sortant(attaquant);
	return cible->vie-=dmg;
}

extern int ennemi_degats_sortant(ennemi_t * ennemi){
	//Cette fonction calcule les degats que l'ennemi inflige avant reduction par des defenses
	int bonusAdd = 0;
	int bonusMult = 1;
	return ((ennemi->attaque*bonusMult)+bonusAdd);
}

extern int ennemi_degats_entrant(ennemi_t * ennemi, int dgts){
	//Cette fonction calcule la reduction par les defenses, soustrait le resultat a la vie de l'ennemi et le renvoie pour affichage eventuel
	int dgtFin = dgts - ennemi->defense;

	ennemi->vie -= dgtFin;
	return dgtFin;
}

extern pos_t ennemi_position(ennemi_t * ennemi){
	//Obtenir la position d'un ennemi
	pos_t position;
	position.x = ennemi->position->x;
	position.y = ennemi->position->y;
	return position;
}

extern pos_t ennemi_aller_vers(ennemi_t * ennemi, pos_t destination){
	//Fonction IA, l'ennemi choisi un chemin pour se rendre à un endroit precis
	pos_t posFin;
	posFin.x = ennemi->position->x;
	posFin.y = ennemi->position->y;
	return posFin;
}

extern pos_t mouvement_late(ennemi_t * ennemi, int direction){
	//Deplacement lateral
	pos_t posFin;
	posFin.x = ennemi->position->x;
	posFin.y = ennemi->position->y;
	return posFin;
}

extern pos_t mouvement_verti(ennemi_t * ennemi){
	//Deplacement vertical
	pos_t posFin;
	posFin.x = ennemi->position->x;
	posFin.y = ennemi->position->y;
	return posFin;
}
