#include <stdlib.h>
#include <string.h>
#include "../../lib/entite/ennemi.h"



static err_t ennemi_detruire(ennemi_t ** ennemi){
	pos_t pos={0,0};
	entite_t * ent=NULL;
	if(!(*ennemi))
		return OK;


	ent=realloc((entite_t*)(*ennemi),sizeof(entite_t));
	*ennemi=NULL;
	if(!ent){
		printf("La réallocation de l'ennemi pour sa destruction a échouée");
		return PAS_DALLOC;
	}
	ent->detruire_ent(&ent);
	return OK;
}

static err_t ennemi_effacer(ennemi_t ** ennemi){
	ennemi->chunk=NULL;
	ennemi->salle=NULL;
	(*ennemi) = NULL;
	return(OK);
}

extern booleen_t ennemi_existe(ennemi_t * const ennemi){
	if(ennemi == NULL) 
		return(FAUX) ;
	else
		return(VRAI) ; 
}

//static void ennemi_afficher(ennemi_t * const ennemi){}

static int ennemi_blesser(ennemi_t * attaquant, perso_t * cible){
	int dmg = ennemi_degats_sortant(attaquant);
	return cible->vie-=dmg;
}

static int ennemi_degats_sortant(ennemi_t * ennemi){
	//Cette fonction calcule les degats que l'ennemi inflige avant reduction par des defenses
	int bonusAdd = 0;
	int bonusMult = 1;
	return ((ennemi->degats*bonusMult)+bonusAdd);
}

static int ennemi_degats_entrant(ennemi_t * ennemi, int dgts){
	//Cette fonction calcule la reduction par les defenses, soustrait le resultat a la vie de l'ennemi et le renvoie pour affichage eventuel
	int dgtFin = dgts - ennemi->defense;

	ennemi->vie -= dgtFin;
	return dgtFin;
}

static pos_t ennemi_position(ennemi_t * ennemi){
	//Obtenir la position d'un ennemi
	pos_t position;
	position.x = ennemi->position->x;
	position.y = ennemi->position->y;
	return position;
}

static pos_t ennemi_aller_vers(ennemi_t * ennemi, pos_t destination){
	//Fonction IA, l'ennemi choisi un chemin pour se rendre à un endroit precis
	pos_t posFin;
	posFin.x = ennemi->position->x;
	posFin.y = ennemi->position->y;
	return posFin;
}

static pos_t mouvement_late(ennemi_t * ennemi, int direction){
	//Deplacement lateral
	pos_t posFin;
	posFin.x = ennemi->position->x;
	posFin.y = ennemi->position->y;
	return posFin;
}

static pos_t mouvement_verti(ennemi_t * ennemi){
	//Deplacement vertical
	pos_t posFin;
	posFin.x = ennemi->position->x;
	posFin.y = ennemi->position->y;
	return posFin;
}

static
booleen_t en_vie(perso_t * ennemi){

    if(ennemi->vie > 0)
        return VRAI;
    else
        return FAUX;

}

extern ennemi_t * ennemi_creer(char * nom, 
					 char *description,
					 char * type,
					 int vie,
					 int defense,
					 salle_t * salle,
					 chunk_t * chunk,
					 pos_t pos,
					 float vitesse_x, float vitesse_y, float vitesse_max_y,float vitesse_saut,
				     int w, int h, 
					 int w_hitbox, int h_hitbox,int offset_hitbox,
					 float secSprite,
					 float vit_attack,
					 int degats,
					 char * nom_attaque,
					 int nbTextures,
					 SDL_Texture ** textures){
	ennemi_t * ennemi = NULL;
	if ((ennemi=(ennemi_t*)perso_creer(nom,description,salle,chunk,pos,vitesse_x,vitesse_y,vitesse_max_y,secSprite,w,h,w_hitbox,h_hitbox,offset_hitbox,vit_attack,degats,nom_attaque,nbTextures,textures))==NULL){
		return NULL;
	}
	ennemi=realloc(ennemi,sizeof(perso_t));
	if(ennemi==NULL){
		printf("La réallocation de l'ennemi %s n'a pas pu etre effectuée\n", nom);
		ennemi->ennemi_detruire(&ennemi);
		return NULL;
	}

	ennemi->type = malloc(sizeof(char)*(strlen(type)+1));
	ennemi->defense = defense;

	ennemi->ennemi_detruire = ennemi_detruire;
	ennemi->ennemi_blesser = ennemi_blesser;
	ennemi->ennemi_degats_sortant = ennemi_degats_sortant;
	ennemi->ennemi_degats_entrant = ennemi_degats_entrant;
	ennemi->mouvement_verti = mouvement_verti;
	ennemi->mouvement_late = mouvement_late;
	ennemi->envie = en_vie;
	ennemi->ennemi_aller_vers = ennemi_aller_vers;
	ennemi->ennemi_position = ennemi_position;
	ennemi->ennemi_effacer = ennemi_effacer;

	return(ennemi);
}
