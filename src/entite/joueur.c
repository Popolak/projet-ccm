#include <stdio.h>
#include <stdlib.h>
#include "../../lib/entite/personnage.h"
#include "../../lib/entite/joueur.h"
#include "../../SDL/include/SDL.h"

SDL_Event events; 

extern
void creation(perso_t * tomate, perso_t  * carotte, perso_t * viande){

	pos_t position;
	position.x = 50;
	position.y = 50;

	tomate = perso_creer("tomate", 50, 10, &position, "tomate.png", 10, 5);
	carotte = perso_creer("carotte", 50, 10, &position, "carotte.png", 10, 5);
	viande = perso_creer("viande", 50, 10, &position, "viande.png", 10, 5);
}

extern
void deplacement(){


}

extern
void control(perso_t * tomate, perso_t  * carotte, perso_t * viande){ //choix du personnage qu'on controle, probablement a mettre dans la boucle de jeu plutôt que dans une fonction

	perso_t perso_control;

	perso_t roulette[2];
	int i = 0;

	roulette[0] = (*tomate);
	roulette[1] = (*carotte);
	roulette[2] = (*viande);

	if(events.key.keysym.sym == SDLK_e){
		if(i+1 > 2)
			perso_control = roulette[0];
		else
			perso_control = roulette[i++];
		SDL_Log("je controle %s", perso_control.nom);
	}
}

