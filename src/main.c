#include <stdio.h>
#include <stdlib.h>
#include "../lib/entite/personnage.h"
#include "../lib/entite/joueur.h"
#include "../lib/entite/entite.h"
#include "../SDL/include/SDL.h"
#include "../lib/commun.h"

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
	pos_t position;
	position.x = 50;
	position.y = 50;

	perso_t roulette[2];//sélection personnage
	roulette[0] = (*tomate);
	roulette[1] = (*carotte);
	roulette[2] = (*viande);


	int i = 0;


	menu();

	while((en_vie(tomate) && en_vie(carotte)) && en_vie(viande)){//boucle de gameplay

		SDL_EnableKeyRepeat(perso_control.vitesse_x,perso_control.vitesse_y); //définie la vitesse de déplacement

		switch(events.key.keysym.sym){

			case SDLK_e: //évènement provoquant un changement de perso
				if(i+1 > 2)
					perso_control = roulette[0];
				else
					perso_control = roulette[i++];
				//SDL_Log("je controle %s", perso_control.nom);
			break;

			//déplacement horizontal
			case SDLK_d:
				deplacement(&perso_control.position, DROITE);
			break;

			case SDLK_q:
				deplacement(&perso_control.position, GAUCHE);
			break;

			//appel de la fonction de saut
			case SDLK_SPACE:
				saut(&perso_control.position);
			break;

			//appel des fonctions d'attaque, reste à gérer la vitesse d'attaque 
			case SDLK_k:
				SDL_EnableKeyRepeat(perso_control.vit_attack, perso_control.vit_attack);//définie la vitesse d'attaque
				attaque1(perso_control);
			break;

			case SDLK_l:
				SDL_EnableKeyRepeat(perso_control.vit_attack, perso_control.vit_attack);//définie la vitesse d'attaque
				attaque2(perso_control);
			break;


		}

	}
	return 0;
}