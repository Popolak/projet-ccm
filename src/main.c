#include <stdio.h>
#include <stdlib.h>
#include "../lib/entite/personnage.h"
#include "../lib/entite/joueur.h"
#include "../lib/entite/entite.h"
#include "../SDL/include/SDL.h"

SDL_Event events; 

void menu(){//menu d'accueil du jeu


}

int main(int argc, char* argv[]){

	chunk_t chunk_dep;
	salle_t salle_dep;

	perso_t * tomate = NULL;
	perso_t * carotte = NULL;
	perso_t * viande = NULL;
	creation(tomate, carotte, viande, chunk_dep, salle_dep); //création des personnages jouables

	perso_t perso_control;

	perso_t roulette[2];//sélection personnage
	roulette[0] = (*tomate);
	roulette[1] = (*carotte);
	roulette[2] = (*viande);


	int i = 0;

	menu();

	while((en_vie(*tomate) && en_vie(*carotte)) && en_vie(*viande)){//boucle de gameplay

		if(events.key.keysym.sym == SDLK_e){ //évènement provoquant un changement de perso
			if(i+1 > 2)
				perso_control = roulette[0];
			else
				perso_control = roulette[i++];
			//SDL_Log("je controle %s", perso_control.nom);
		}





		//appel des fonctions d'attaque, reste à gérer la vitesse d'attaque 
		if(events.key.keysym.sym == SDLK_k){
			attaque1(perso_control);
		}
		if(events.key.keysym.sym == SDLK_l){
			attaque2(perso_control);
		}





	}
	return 0;
}