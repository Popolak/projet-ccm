#include <stdio.h>
#include <stdlib.h>
#include "../lib/entite/personnage.h"
#include "../lib/entite/joueur.h"
#include "../lib/entite/entite.h"
#include "../SDL/include/SDL.h"

SDL_Event events; 

extern
void creation(perso_t * tomate, perso_t  * carotte, perso_t * viande,chunk_t chunk_dep, salle_t salle_dep){

	pos_t position;
	position.x = 50;
	position.y = 50;

	tomate = perso_creer("tomate",50, 10, &position, "tomate.png", 10,1,5, chunk_dep, salle_dep);
	carotte = perso_creer("carotte", 50, 10, &position, "carotte.png", 10, 5, chunk_dep, salle_dep);
	viande = perso_creer("viande", 50, 10, &position, "viande.png", 10, 5, chunk_dep, salle_dep);
}

extern
void deplacement(perso_t *perso_control.position, int direction){

	switch(direction){
		case GAUCHE:
		*perso_control.position->x--;
		break;

		case DROITE:
		*perso_control.position->x++;
		break;
	}
}

extern

void saut(*perso_control.position){

	double v_x = 1.5;
	double v_saut = -4;
	double v_gravite = 0.08;
	double v_y = v_saut;

	*perso_control.position.x += v_x;
	*perso_control.position.y += v_y;

	v_y += v_grav;

	if (*perso_control.position.y > 300)
	v_y = v_saut;

}

extern
perso_t attaque1(perso_t perso_control){ //actuellement théorique

	//if(perso_control == *tomate){ //couteau
		//animation attaque;

		//if(hitbox_joueur == hitbox_ennemi){?????????

			//prendre_coup(ennemi, 10);

		//}


	//}

}

extern
perso_t attaque2(perso_t perso_control){


}