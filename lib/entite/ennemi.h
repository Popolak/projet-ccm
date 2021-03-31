#ifndef _ENNEMI_H_
#define _ENNEMI_H_

#include "personnage.h"

//Definition de la structure ennemi
typedef struct ennemi_s{
	char * nom;
	char * type;
	int taille;
	pos_t* position;
	char * nom_sprite; //pour animer après
	float vitesse; //gestion de déplacement
	int vitAttaque;
	int attaque;
	int portee;
	int vie;
	int defense;
	//chunk_t * chunk;

	//inventaire * objets;
} ennemi_t;


//Fonctions
extern ennemi_t * ennemi_creer(char* nom, char* type, int taille, pos_t* position, char* nom_sprite, float vitesse, int vitAttaque, int attaque, int portee, int vie, int defense) ;
extern err_t ennemi_detruire(ennemi_t ** ennemi) ;
extern err_t ennemi_effacer(ennemi_t ** ennemi) ;

extern booleen_t ennemi_existe(ennemi_t * const ennemi) ;

extern void ennemi_afficher(ennemi_t * const ennemi) ;

extern int ennemi_blesser(ennemi_t * attaquant, perso_t * cible);

extern int ennemi_degats_sortant(ennemi_t * ennemi);

extern int ennemi_degats_entrant(ennemi_t * ennemi, int dgts);

extern pos_t ennemi_position(ennemi_t * ennemi);

extern pos_t ennemi_aller_vers(ennemi_t * ennemi, pos_t destination);

extern pos_t mouvement_late(ennemi_t * ennemi, int direction);

extern pos_t mouvement_verti(ennemi_t * ennemi);
#endif