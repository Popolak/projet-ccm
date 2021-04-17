#include <stdio.h>
#include <stdlib.h>

#include "../../lib/entite/personnage.h"
#include "../../lib/generation/element_generation.h"

/* Par Matthis */
/* Fonctions */

static
err_t perso_depop(perso_t * perso){
	perso->chunk=NULL;
	perso->salle=NULL;
	return OK;
}

static 
err_t perso_copie_partiel(perso_t * persoDst, perso_t * persoSrc){
	persoDst->pos=persoSrc->pos;
	persoDst->vitesse_x=persoSrc->vitesse_x;
	persoDst->vitesse_y=persoSrc->vitesse_y;
	persoDst->salle=persoSrc->salle;
	persoDst->chunk=persoSrc->chunk;
	persoDst->dir=persoSrc->dir;
	return OK;
}

extern
booleen_t perso_existe( perso_t * const personnage )
{
  if( personnage == NULL )
    return FAUX;
  else
    return VRAI;
}


static err_t perso_afficher_dans_chunk(SDL_Renderer *ren,perso_t *entite,int WINH,int WINW){
    SDL_Texture * a_afficher=NULL;
    int w,h, w_immo,h_immo;
    float ratio_h, ratio_w;

	if(entite->temps_att >=0 && entite->temps_att < entite->vit_attack/2){
		entite->lastSprite=0;
		if(entite->nbTextures < ATTAQUE1){
			a_afficher=entite->textures[IMMO];
		}
		else
			a_afficher=entite->textures[ATTAQUE1];
	}
	else {
		if(entite->lastSprite >= 7*entite->secSprite ){
			entite->lastSprite=0;
		}
		if(entite->vitesse_y==0){
			entite->lastSprite=0;
			a_afficher=entite->textures[IMMO];
		}

		if(entite->vitesse_x < 0 ){
			if(entite->nbTextures <= SAUT)
				a_afficher= entite->textures[IMMO];
			else 
				a_afficher=entite->textures[SAUT];
		}
		else if(entite->vitesse_x > 0 ){
			if(entite->nbTextures <= TOMBE)
				a_afficher= entite->textures[IMMO];
			else 
				a_afficher=entite->textures[TOMBE];
		}
		else {

			if(abs(entite->vitesse_y) && entite->nbTextures > IMMO && !entite->en_l_air(entite)){
				if(entite->secSprite > entite->lastSprite){
					if(entite->nbTextures <= POS_MOUV1)
						a_afficher=entite->textures[IMMO];
					else   
						a_afficher=entite->textures[POS_MOUV1];
				}
				
				else if (entite->lastSprite > 4 * entite->secSprite && entite->lastSprite < 5 * entite->secSprite ){
					if(entite->nbTextures <= POS_MOUV2)
						a_afficher=entite->textures[IMMO];
					else   
						a_afficher=entite->textures[POS_MOUV2];
				}
				else {
					if(entite->nbTextures <= NEUTRE)
						a_afficher=entite->textures[IMMO];
					else    
						a_afficher=entite->textures[NEUTRE];
				}
			}
		}
	}
    if(a_afficher==NULL){
        a_afficher= entite->textures[IMMO];
    }
    
    SDL_QueryTexture(a_afficher,NULL,NULL,&w,&h);
    SDL_QueryTexture(entite->textures[IMMO],NULL,NULL,&w_immo,&h_immo);
    ratio_h=(1.0*h/h_immo) ;
    ratio_w=1.0*w/w_immo;
    entite->afficher_fenetre(ren,entite,(entite->w*WINW/CHUNKW)*ratio_w,(entite->h*WINW/CHUNKW )/ratio_h,entite->pos.x*WINW/CHUNKW, entite->pos.y*WINW/CHUNKW, a_afficher);
}

/*
	perso_detruire
	paramètre:
		pointeur sur pointeur sur perso_t
	retourne OK si tout s'est bien passé
	libère l'espace du personnage et toutes les allocation reliées a lui
*/

extern 
err_t perso_detruire( perso_t ** personnage){  
	pos_t pos={0,0};
	entite_t * ent=NULL;
	if(!(*personnage))
		return OK;


	ent=realloc((entite_t*)(*personnage),sizeof(entite_t));
	*personnage=NULL;
	if(!ent){
		printf("La réallocation de personnage pour sa destruction a échouée");
		return PAS_DALLOC;
	}
	ent->detruire(&ent);
	return OK;
}

/*
	fait_partie_bin
	paramètres:
		int tot_bin : nombre total
		int nombre_puis_2 : puissance de 2 pour vérifier si un certain bit appartient a tot_bin
	retourne un nombre si vrai, 0 sinon
*/
extern 
int fait_partie_bin (int tot_bin, int nombre_puis_2){
	return (tot_bin & nombre_puis_2);
}

/*
	input_update_speed
	paramètres:	
		pointeur sur perso_t
		int tot_touche, le total des touches (ex: 110 donne KEY_LEFT et KEY_JUMP)
	
	En fonction du total de touches on actualise la vitesse du personnage
*/
static 
void input_update_speed (perso_t * perso, int tot_touche){
	float vit_tempo;
	if(fait_partie_bin(tot_touche,KEY_ATT) && (perso->temps_att < 0|| perso->temps_att > perso->vit_attack)){
		perso->temps_att=0;
	}
	if(perso->temps_att>perso->vit_attack/2 || perso->temps_att < 0){
		if(fait_partie_bin(tot_touche,KEY_JUMP) && !perso->en_l_air((entite_t*)perso)){
			perso->vitesse_x=-perso->vitesse_saut;
		}
		if(!(fait_partie_bin(tot_touche, KEY_LEFT) && fait_partie_bin(tot_touche, KEY_RIGHT))){
			if(fait_partie_bin(tot_touche, KEY_LEFT)){
				perso->vitesse_y=-perso->vitesse_max_y;
			}
			else if(fait_partie_bin(tot_touche, KEY_RIGHT)){
				perso->vitesse_y=perso->vitesse_max_y;
			}
		}
	}
	
}


static 
int perso_deplacement(void * element,double temps, void *tab[NB_MAX_AFF], err_t (*tab_destr[NB_MAX_AFF])(void ** ) ){
    pos_t pos_mur;
    chunk_t * chunk;
    booleen_t deja=FAUX;
    int w,h;
    float vitesse_tempo;
	perso_t * perso = (perso_t *) element;
	entite_t * ent = (entite_t *)perso;
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

	if(ent->en_l_air(ent)){                                 //Si on est en l'air, on tombe (La vitesse maximale est atteinte après 2 secondes de chute avec v0=0)
		ent->vitesse_x= (ent->vitesse_x + GRAVITE*temps > GRAVITE*2) ? GRAVITE*2:ent->vitesse_x + GRAVITE*temps;
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
	if(coord_correcte(ent->pos.x, ent->pos.y)){
		if (ent->contact_porte(ent)){
			return 2;
		}
		return 0;
	}
	

    while(! coord_correcte(ent->pos.x,ent->pos.y)){
		if(ent->pos.y>=CHUNKW){                             //Si le personnage se retrouve trop a droite, il sort du chunk 
			if((chunk=ent->salle->chercher_chunk(ent->salle,ent->chunk->position.x, ent->chunk->position.y+1))!=NULL){ //alors on vérifie s'il y en a un a droite
				ent->chunk=chunk;                           //Le chunk a droite devient le nouveau chunk de l'entité
				ent->pos.y=ent->pos.y-CHUNKW;               //Sa position change en fonction
				return 1;
			}
			else{
				pos_mur.y=CHUNKW-1;
				replacer(ent,pos_mur,DROITE);
			}
		}               

															//Meme systeme ensuite
		if(ent->pos.y<0){
			if((chunk=ent->salle->chercher_chunk(ent->salle,ent->chunk->position.x, ent->chunk->position.y-1))!=NULL){
				ent->chunk=chunk;
				ent->pos.y=CHUNKW+ent->pos.y;
				return 1;
			}
			else{
				pos_mur.y=0;
				replacer(ent,pos_mur,GAUCHE);
			}
		}


		if(ent->pos.x>=CHUNKH){
			if((chunk=ent->salle->chercher_chunk(ent->salle,ent->chunk->position.x+1, ent->chunk->position.y))!=NULL){
				ent->chunk=chunk;
				ent->pos.x=ent->pos.x-CHUNKH;
				return 1;
			}
			else{
				pos_mur.x=CHUNKH-1;
				replacer(ent,pos_mur,BAS);
			}
		}


		if(ent->pos.x<0){
			if((chunk=ent->salle->chercher_chunk(ent->salle,ent->chunk->position.x-1, ent->chunk->position.y))!=NULL){
				ent->chunk=chunk;
				ent->pos.x=CHUNKH+ent->pos.x;
				return 1;
			}
			else{
				pos_mur.x=0;
				replacer(ent,pos_mur,HAUT);
			}
		}
		return 0;
	}
	
	
    
	
}


int perso_pos_relative(perso_t * perso){
	if(perso->pos.x < CHUNKH/2){
		if(perso->pos.y < CHUNKW/2){
			return HG;
		}
		return HD;
	}
	if(perso->pos.x > CHUNKH/2){
		if(perso->pos.y < CHUNKW/2){
			return BG;
		}
		return BD;
	}
}

static
err_t perso_change_salle(perso_t * perso){
	int posit_porte;
	porte_t * porte=NULL;
	posit_porte=perso_pos_relative(perso);
	porte=perso->chunk->chercher_porte(perso->chunk,posit_porte);
	perso->salle=porte->porteDest->salle;
	perso->chunk=porte->porteDest->chunk;
	switch(porte->porteDest->position){
		case (HG):
			perso->pos.x=TAILLE_MUR+TAILLE_PORTE-perso->h/2;
			perso->pos.y=6;
			break;
		case (BG):
			perso->pos.x=CHUNKH-CHUNKH*ratioSol-perso->h/2;
			perso->pos.y=6;
			break;
		case(HD):
			perso->pos.x=TAILLE_MUR+TAILLE_PORTE-perso->h/2;
			perso->pos.y=CHUNKW-6;
			break;
		case(BD):
			perso->pos.x=CHUNKH-ratioSol*CHUNKH-perso->h/2;
			perso->pos.y=CHUNKW-6;
			break;
	}
	return OK;

}

static
void perso_prendre_coup(perso_t * personnage, int degats){
	personnage->vie -= degats;
}

static 
void perso_attaque(void * perso_courant, void * entite_subit){

}

extern
err_t remplir_tableaux(SDL_Renderer * ren,perso_t *perso, void * tab[NB_MAX_AFF], err_t (*tab_destr[NB_MAX_AFF])(void ** ),char * appel ,FILE * index, FILE * file_gen){
	void * element;
	int n;
	if(!file_gen){
		printf("Pas de fichier de génération \n");
		return 1;
	}
	char str[300];
	int sx,sy,cx,cy;
	while(!feof(file_gen)){
		sx=fgetc(file_gen)-'0';
		sy=fgetc(file_gen)-'0';
		cx=fgetc(file_gen)-'0';
		cy=fgetc(file_gen)-'0';

		fgets(str,299,file_gen);
		if(perso->salle->position.x == sx && perso->salle->position.y == sy && perso->chunk->position.x == cx && perso->chunk->position.y == cy){
			element = creer_entite_chaine(ren ,&n,perso,str, index,appel);
			if(n==1){
				ajouter_tableaux(tab,tab_destr,element,((perso_t*)element)->detruire);
			}
			else 
				ajouter_tableaux(tab,tab_destr,element,((entite_t*)element)->detruire);
			
		}
	}
	fseek(file_gen,0,SEEK_SET);
	fseek(index,0,SEEK_SET);
	return 0;
}

static 
err_t perso_change_chunk( SDL_Renderer * ren, perso_t * perso,  void * tab[NB_MAX_AFF], err_t (*tab_destr[NB_MAX_AFF])(void ** ), FILE *index, FILE * file_gen, char * appel){
	vider_tableaux(tab, tab_destr);
	remplir_tableaux(ren,perso,tab,tab_destr,appel,index,file_gen);
}

static
booleen_t en_vie(perso_t * personnage){

    if(personnage->vie > 0)
        return VRAI;
    else
        return FAUX;

}

extern
perso_t * perso_creer(char * nom, 
					 char *description,
					 int vie,
					 salle_t * salle,
					 chunk_t * chunk,
					 pos_t pos,
					 float vitesse_x, float vitesse_y, float vitesse_max_y,float vitesse_saut,
				     int w, int h, 
					 int w_hitbox, int h_hitbox,int offset_hitbox,
					 float secSprite,
					 float vit_attack, int degats,
					 int nbTextures,
					 SDL_Texture ** textures)
{
	perso_t * personnage = NULL;
	if ((personnage=(perso_t*)entite_creer(nom,description,salle,chunk,pos,vitesse_x,vitesse_y,vitesse_max_y,secSprite,w,h,w_hitbox,h_hitbox,offset_hitbox,nbTextures,textures))==NULL){
		return NULL;
	}
	personnage=realloc(personnage,sizeof(perso_t));
	if(personnage==NULL){
		printf("La réallocation du personnage %s n'a pas pu etre effectuée\n", nom);
		personnage->detruire(&personnage);
		return NULL;
	}
	personnage->vitesse_saut=vitesse_saut;
	personnage->vie = vie;
	personnage->vit_attack = vit_attack;
	personnage->degats = degats;
	personnage->temps_att=-1;

	
	personnage->detruire= perso_detruire;
	personnage->prendre_coup=perso_prendre_coup;
	personnage->update_speed=input_update_speed;
	personnage->copie_partiel=perso_copie_partiel;
	personnage->depop=perso_depop;
	personnage->envie=en_vie;
	personnage->deplacer=perso_deplacement;
	personnage->attaque=perso_attaque;
	personnage->change_chunk=perso_change_chunk;
	personnage->change_salle=perso_change_salle;
	personnage->afficher_chunk=perso_afficher_dans_chunk;

	return(personnage);
}

