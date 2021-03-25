#include <stdio.h>
#include <stdlib.h>
#include "../../lib/entite/joueur.h"
#include "../../lib/affichage/room_rendering.h"

/*
extern
joueur_t * joueur_creer(salle_t * salle, chunk_t * chunk,SDL_Renderer * ren){
	int i;
	pos_t pos_i={TAILLE_MUR+21,CHUNKH-CHUNKH*ratioSol-31};
	joueur_t * joueur=NULL;
	joueur= malloc(sizeof(joueur_t));
	if(!joueur){
		printf("L'allocation du joueur n'a pas fonctionnée\n");
		return NULL;
	}
	joueur->personnages=malloc(sizeof(perso_t)*NB_PERSO);
	if(!joueur->personnages){
		printf("L'allocation du tableau de personnages pour le joueur n'a pas pu être effectuée");
		
	}
	joueur->personnages[0]=perso_creer("Tom","tomate",70,salle,chunk,pos_i,0,0,300,700,40,60,30,50,0,0,1,)

}*/



extern
void creation(SDL_Renderer * ren,perso_t * tomate, perso_t  * carotte, perso_t * viande,chunk_t * chunk_dep, salle_t *salle_dep){

	pos_t position;
	position.x = CHUNKH-CHUNKH*ratioSol-31;
	position.y = TAILLE_MUR+21;
	SDL_Texture * textureTom=creer_texture_image(ren,"../../graphics/sprite/personnage_sprites/Tom neutre.png");
	tomate = perso_creer("Tom","tomate",80,salle_dep,chunk_dep,position,0,0,300,700,60,40,50,30,0,0,1,&textureTom);
}

extern
void deplacement(perso_t * perso_control, int tot_touche){
	perso_control->update_speed(perso_control,tot_touche);
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