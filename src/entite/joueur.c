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

	tomate = perso_creer("tomate", 50, 10, &position, "tomate.png", 10,1,5, chunk_dep, salle_dep);
	carotte = perso_creer("carotte", 50, 10, &position, "carotte.png", 10, 5, chunk_dep, salle_dep);
	viande = perso_creer("viande", 50, 10, &position, "viande.png", 10, 5, chunk_dep, salle_dep);
}

extern
void deplacement(){


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