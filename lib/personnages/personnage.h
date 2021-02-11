#ifndef _PERSO_H_
#define _PERSO_H_

#include "../../lib/commun.h"

typedef struct perso_s
{
	char * nom;
	int vie;
	int taille;
	pos_t * position;
	char * nom_sprite; //pour animer après
	int vitesse; //gestion de déplacement
	int degats;

	void (*animer)(struct perso_s * const);
    err_t (*detruire)(struct perso_s ** );
	void (*prendre_coup)(struct perso_s * , int );
	booleen_t (*en_vie)(struct perso_s *);

}perso_t;

extern perso_t * perso_creer(char * nom, int vie, int taille, pos_t * position, char * nom_sprite, int vitesse, int degats);
extern booleen_t perso_existe( perso_t * const personnage );

#endif