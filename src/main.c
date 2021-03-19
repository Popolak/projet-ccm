#include <stdio.h>
#include <stdlib.h>
#include "../lib/entite/personnage.h"
#include "../lib/entite/joueur.h"
#include "../lib/entite/entite.h"
#include "../SDL/include/SDL.h"
#include
#include "../lib/commun.h"

SDL_Event event; 

void menu_accueil(){//menu d'accueil du jeu

	
	//création de tout les potentielles surfaces
	SDL_Surface *ecran =NULL;
	SDL_Surface *affichage =NULL;
	SDL_Surface *menu[3] =NULL;
	SDL_Surface *jouer =NULL;
	SDL_Surface *quit =NULL;

	SDL_Rect positionMenu;

	int continuer = 1;
	int i = 0;

	SDL_Init(SDL_INIT_VIDEO);

		ecran = SDL_SetVideoMode(1920,1080,32, SDL_HWSURFACE | SDL_DOUBLEBUF);
		SDL_WM_SetCaption("Roguelike", NULL);

	menu[0] = IMG_Load("../graphics/menu/menu.png");
	menu[1] = IMG_Load("../graphics/menu/jouer.png");
	menu[2] = IMG_Load("../graphics/menu/quit.png");

	affichage = menu[0];

	positionMenu.x=0
	positionMenu.y=0;

	while(continuer){

		//contrôle par clavier
		SDL_WaitEvent(&event);
		switch(events.key.keysym.sym){
			case SDLK_s: 
				affichage = menu[++i];break;
			case SDLK_z:
				affichage = menu[--i];break;
			case SDLK_SPACE:
				if(affichage == jouer){
					continuer = 0;
					break;
				}
				else if(affichage == quit){	
				SDL_FreeSurface(menu); 
				SDL_QUIT(); 
				exit(1); 
				}
			default: break;
		}
		//affichage et raffraichissement
		SDL_BlitSurface(affichage, NULL,ecran,&positionMenu);
		SDL_flip(ecran);
	}
	SDL_FreeSurface(menu);
	SDL_QUIT();
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

	menu_accueil(); //le menu se contrôle avec S,Z et espace

	/*
	//
	// Initialisation SDL
	// Ouverture fenêtre de jeu
	*/

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