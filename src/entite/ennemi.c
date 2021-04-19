/*Par Matthis*/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../lib/entite/ennemi.h"
#include "../../lib/generation/element_generation.h"

static
booleen_t in_range(ennemi_t * ennemi,entite_t *ent ){
	if(  (ennemi->range >abs( ennemi->pos.y-(ent->pos.y + ent->offset_hitbox* (ent->dir==DROITE ? 1 :-1))) && (ennemi->h/2 >= abs(ent->pos.x-ennemi->pos.x))))
		return VRAI;
	return FAUX;
}

static
int ennemi_creer_ajouter_attaque(SDL_Renderer *ren,attaque_t * tab[NB_MAX_AFF],err_t (*tab_destr[NB_MAX_AFF])(void ** ), perso_t * perso, FILE * index, char * appel ){
	int n;
	char str[30];
	strcpy(str,perso->nom_attaque);
	strcat(str," -1 -1 0 0");
	attaque_t * attaque= (attaque_t*) creer_entite_chaine(ren,&n,perso,str,index,appel);
	ajouter_tableaux((void**)tab,tab_destr,attaque,attaque->detruire);

	return 0;
}

static 
int ennemi_input_update_speed (void * element,void * element_joueur, int tot_touche){
	ennemi_t *perso=(ennemi_t*) element;
	perso_t *joueur=(perso_t*) element_joueur;
	if(joueur->pos.y > perso->pos.y){
		perso->dir=DROITE;
	}
	else {
		perso->dir=GAUCHE;
	}
	if(abs(perso->pos.y-(joueur->pos.y + joueur->offset_hitbox*(joueur->dir == DROITE ? 1 : -1)) ) < perso->range)
		return 0;
	
	else if(perso->temps_att>perso->vit_attack/2 || perso->temps_att < 0){
		if((joueur->pos.y + joueur->offset_hitbox*(joueur->dir == DROITE ? 1 : -1)) > perso->pos.y)
			perso->vitesse_y=perso->vitesse_max_y;
		else if ((joueur->pos.y + joueur->offset_hitbox*(joueur->dir == DROITE ? 1 : -1)) < perso->pos.y)
			perso->vitesse_y=-perso->vitesse_max_y;
	}
	return 0;
}

static 
int ennemi_deplacement(void * element,double temps ){
    pos_t pos_mur;
    chunk_t * chunk;
    booleen_t deja=FAUX;
    int w,h;
    float vitesse_tempo;
	perso_t * perso = (perso_t *) element;
	entite_t * ent = (entite_t *)perso;

	perso->lastSprite+=temps;
	if(perso->temps_inv >=0){
        perso->temps_inv+=temps;
    }
	if(perso->temps_att >=0){
		perso->temps_att+=temps;
	}
	if(perso->temps_att > perso->vit_attack)
		perso->temps_att=-1;
	
	if(perso->temps_inv > S_INV)
		perso->temps_inv=-1;


    if(ent->vitesse_y > 0){                          //On adapte la direction de l'entité en fonction de sa vitesse
        if(ent->dir==GAUCHE)
            (ent->pos.y)-=(2*ent->offset_hitbox);
        ent->dir=DROITE;                            //Si elle est nulle, on ne change rien 
    }
    else if(ent->vitesse_y < 0){
        if(ent->dir==DROITE)
            (ent->pos.y)+=(2*ent->offset_hitbox);
        ent->dir=GAUCHE;
    }

    (ent->pos.y)+= (ent->vitesse_y)*temps;                  //On actualise la position horizontale grace a vitesse_y
                                                                                           
	if(coord_correcte(ent->pos.x,ent->pos.y)){				//Si on se retrouve dans un mur, on se replace
		pos_mur=mur_a_gauche(ent);   
		if(pos_mur.x!=-1){
			replacer(ent,pos_mur,GAUCHE);
		}
	}
	if(coord_correcte(ent->pos.x,ent->pos.y)){
		pos_mur=mur_a_droite(ent);                              //Pareil
		if(pos_mur.x!=-1){
			replacer(ent,pos_mur,DROITE);
		}
	}
	(ent->pos.x)+= (ent->vitesse_x)*temps; 
	if(coord_correcte(ent->pos.x,ent->pos.y)){               //Puis on actualise la position verticale
		pos_mur=mur_en_bas(ent);
		if(pos_mur.x!=-1){
			replacer(ent,pos_mur,BAS);                          //Si on se retrouve dans un mur, on se replace
		}
	}
	if(coord_correcte(ent->pos.x,ent->pos.y)){ 
		pos_mur=pont_en_bas(ent);
		if(pos_mur.x!=-1){
			replacer(ent,pos_mur,BAS);                          //Si on se retrouve dans un mur, on se replace
		}
	}
	if(coord_correcte(ent->pos.x,ent->pos.y)){ 
		pos_mur=mur_en_haut(ent);
		if(pos_mur.x!=-1 ){
			replacer(ent,pos_mur,HAUT);                         //Pareil
		}
	}
	if(ent->gravite){
		if(ent->en_l_air(ent)){                                 //Si on est en l'air, on tombe (La vitesse maximale est atteinte après 2 secondes de chute avec v0=0)
			ent->vitesse_x= (ent->vitesse_x + GRAVITE*temps > GRAVITE*2) ? GRAVITE*2:ent->vitesse_x + GRAVITE*temps;
		}
	}
	
		//Si la vitesse de l'entité n'est pas actualisée (soit par un input de l'utilisateur, soit par l'algorithme des ennemis)
    //Alors on la change grace a la décélération
    if(ent->vitesse_y){
        if(!ent->en_l_air(ent))
            ent->vitesse_y= ent->vitesse_y -  DECEL*COEFF_DECEL_SOL* temps * (ent->vitesse_y>0 ? 1 : -1)  ; //Sur le sol, la décélération est COEFF_DECEL_SOL fois plus grande

        else
            ent->vitesse_y= ent->vitesse_y -  DECEL*temps * (ent->vitesse_y>0 ? 1 : -1)  ;                  //Que dans l'air

        if(ent->vitesse_y > 0 && ent->dir == GAUCHE || ent->vitesse_y < 0 && ent->dir == DROITE)
            ent->vitesse_y=0;
    }
	

    if(! coord_correcte(ent->pos.x,ent->pos.y)){
		return 1;
	}
	return 0;
	
	
    

    
}


static
void ennemi_action_agit(SDL_Renderer * ren,void * ent_courante, void * ent_subit, void * tab[NB_MAX_AFF],err_t (*tab_destr[NB_MAX_AFF])(void ** ), FILE * index, char * appel ){
	
	if(((ennemi_t*)ent_courante)->dans_range(ent_courante, ent_subit) && (((ennemi_t*)ent_courante)->temps_att == -1 
		|| ((ennemi_t*)ent_courante)->temps_att > ((ennemi_t*)ent_courante)->vit_attack)){
		((ennemi_t*)ent_courante)->temps_att=0;
		((ennemi_t*)ent_courante)->nouvelle_attaque(ren,(attaque_t**)tab,tab_destr,((perso_t*)ent_courante),index,appel);
	}
	if(((entite_t*)ent_courante)->contact((entite_t*)ent_courante,(entite_t*)ent_subit))
		((entite_t*)ent_subit)->action_subit((entite_t*)ent_subit, ((ennemi_t*)ent_courante)->degats);
}

static
err_t ennemi_detruire(ennemi_t ** ennemi){
	perso_t * perso=NULL;
	if (!ennemi){
		return OK;
	}
	
	perso=realloc(*ennemi,sizeof(perso_t));
	if(perso==NULL){
		return ERR_DEB_MEMOIRE;
	}
	perso->detruire(&perso);
	return OK;
	
}


extern
ennemi_t * ennemi_creer(char * nom, 
					 char *description,
					 int vie,
					 salle_t * salle,
					 chunk_t * chunk,
					 pos_t pos,
					 float vitesse_x, float vitesse_y, float vitesse_max_y,float vitesse_saut,
				     int w, int h, 
					 int w_hitbox, int h_hitbox,int offset_hitbox,
					 float secSprite,
					 float vit_attack, int degats, char * nom_attaque,int range,
					 int nbTextures,
					 SDL_Texture ** textures)
{
	ennemi_t * ennemi = NULL;
	if ((ennemi=(ennemi_t*)perso_creer(nom,description,vie,salle,chunk,pos,vitesse_x,vitesse_y,vitesse_max_y,secSprite,w,h,w_hitbox,h_hitbox,offset_hitbox,secSprite,vit_attack,degats,nom_attaque,nbTextures,textures))==NULL){
		return NULL;
	}
	ennemi=realloc(ennemi,sizeof(ennemi_t));
	if(ennemi==NULL){
		printf("La réallocation de l'ennemi %s n'a pas pu etre effectuée\n", nom);
		ennemi->detruire(&ennemi);
		return NULL;
	}
	ennemi->range=range;
	

	ennemi->detruire=ennemi_detruire;
	ennemi->dans_range=in_range;
	ennemi->action_agit=ennemi_action_agit;
	ennemi->nouvelle_attaque=ennemi_creer_ajouter_attaque;
	ennemi->deplacer=ennemi_deplacement;
	ennemi->update_speed=ennemi_input_update_speed;

	return(ennemi);
}
