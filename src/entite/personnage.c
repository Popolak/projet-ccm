#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../lib/entite/personnage.h"
#include "../../lib/generation/element_generation.h"
#include "../../lib/entite/ennemi.h"

/**
* \file personnage.c
* \brief Module personnage
* \verion 1.0
* \date Avril 2021
*/
/* Par Matthis */
/* Fonctions */

/**
    \brief Le personnage disparait de sa salle
    \param perso_t Le personnage à faire disparaitre
    \return Retourne OK après le traitement
*/
static
err_t perso_depop(perso_t * perso){
	perso->chunk=NULL;
	perso->salle=NULL;
	return OK;
}

/**
    \brief Copie le personnage persoSrc dans le personnage persoDst
    \param perso_t Le personnage dans lequel on copie
    \param perso_t Le personnage à copier
    \return Retourne OK après le traitement
*/
static 
err_t perso_copie_partiel(perso_t * persoDst, perso_t * persoSrc){
	persoDst->pos=persoSrc->pos;
	persoDst->pos.x-=  persoDst->h/2 -persoSrc->h/2 ;
	persoDst->pos.y-= persoDst->w/2 - persoSrc->w/2 ;
	persoDst->vitesse_x=persoSrc->vitesse_x;
	persoDst->vitesse_y=persoSrc->vitesse_y;
	persoDst->salle=persoSrc->salle;
	persoDst->chunk=persoSrc->chunk;
	persoDst->dir=persoSrc->dir;
	persoDst->temps_inv=persoSrc->temps_inv;
	return OK;
}

/**
    \brief Vérifie si le personnage passé en paramêtre existe
    \param perso_t Le personnage à tester
    \return Retourne VRAI si le personnage existe, FAUX sinon
*/
extern
booleen_t perso_existe( perso_t * const personnage )
{
  if( personnage == NULL )
    return FAUX;
  else
    return VRAI;
}

/**
    \brief Affiche la barre de vie du personnage
    \param SDL_Renderer Pointeur vers le renderer
    \param perso_t Le personnage dont on affiche la vie
    \param int Largeur de la fenêtre
    \param int Hauteur de la fenêtre
    \return Retourne VRAI si le personnage existe, FAUX sinon
*/
void perso_barre_vie(SDL_Renderer * ren,perso_t * personnage, int WINW, int WINH){

   //création de la barre de vie

	float ratioUtoP=1.0*WINW/CHUNKW;
	SDL_Rect fond_bdv={0,0,0,0}, bdv={0,0,0,0};
	if(strcmp(personnage->nom, "Kurt")==0 || strcmp(personnage->nom, "Tom")==0){
		fond_bdv.y=MAIN_BAR_X*ratioUtoP;
		fond_bdv.x=MAIN_BAR_Y*ratioUtoP;
		fond_bdv.w=MAIN_BAR_W*ratioUtoP;
		fond_bdv.h=MAIN_BAR_H*ratioUtoP;
	}
	else{
		fond_bdv.x=(personnage->pos.y-personnage->w/2)* ratioUtoP;
		fond_bdv.y=(personnage->pos.x-personnage->h/2 - H_HBAR )* ratioUtoP;
		fond_bdv.w=(personnage->w*ratioUtoP);
		fond_bdv.h=(H_HBAR*ratioUtoP);
	}
	bdv=fond_bdv;
	bdv.w*=1.0*personnage->vie/personnage->vie_max;
    SDL_SetRenderDrawColor(ren, 122, 122, 122, 255);
    SDL_RenderFillRect(ren, &fond_bdv);
    SDL_SetRenderDrawColor(ren, 50, 255, 0, 255);
    SDL_RenderFillRect(ren, &bdv);
}


/**
    \brief Affichage d'un personnage dans le chunk
    \param SDL_Renderer Pointeur vers le renderer
    \param void Entité à afficher
    \param int Hauteur de la fenêtre
    \param int Largeur de la fenêtre
    \return Retourne OK après le traitement
*/
static err_t perso_afficher_dans_chunk(SDL_Renderer *ren,void *element,int WINH,int WINW){
    SDL_Texture * a_afficher=NULL;
    int w,h, w_immo,h_immo;
    float ratio_h, ratio_w;
	perso_t * entite= (perso_t*) element;
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

			if(abs(entite->vitesse_y) && entite->nbTextures > IMMO && !entite->en_l_air((entite_t*)entite)){
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
    entite->afficher_fenetre(ren,(entite_t*)entite,(entite->w*WINW/CHUNKW)*ratio_w,(entite->h*WINW/CHUNKW )/ratio_h,entite->pos.x*WINW/CHUNKW, entite->pos.y*WINW/CHUNKW, a_afficher);
	perso_barre_vie(ren,entite,WINW,WINH);
}

/**
	\brief libère l'espace du personnage et toutes les allocation reliées a lui
	\param perso_t Pointeur sur pointeur sur perso_t
	\return Retourne OK si tout s'est bien passé
*/
extern 
err_t perso_detruire( perso_t ** personnage){  
	entite_t * ent=NULL;
	if((*personnage)==NULL)
		return OK;

	if((*personnage)->nom_attaque){
		free((*personnage)->nom_attaque);
		(*personnage)->nom_attaque=NULL;
	}
	ent=realloc((entite_t*)(*personnage),sizeof(entite_t));
	if(!ent){
		printf("La réallocation de personnage pour sa destruction a échouée");
		return PAS_DALLOC;
	}
	ent->detruire(&ent);
	return OK;
}

/**
	\brief Vérifie si un bit appartient à tot_bin
	\param int tot_bin : nombre total
	\param int nombre_puis_2 : puissance de 2 pour vérifier si un certain bit appartient a tot_bin
	\return Retourne un nombre si vrai, 0 sinon
*/
extern 
int fait_partie_bin (int tot_bin, int nombre_puis_2){
	return (tot_bin & nombre_puis_2);
}


/**
	\brief Ajoute L'attaque d'un personnage au tableau des attaques, ainsi que la fonction de destruction dans son tableau
	\param SDL_Renderer Pointeur sur le renderer
	\param attaque_t Tableau des attaques
	\param err_t Tableau des fonctions de destruction
	\param perso_t Personnage dont on ajoute les attauques
	\param FILE Ficher dans lequel chercher
	\param char appel de fichier
	\return Retourne 0 à l'issu de l'opération
*/
static
int perso_creer_ajouter_attaque(SDL_Renderer *ren,attaque_t * tab[NB_MAX_ATT],err_t (**tab_destr)(void ** ), perso_t * perso, FILE * index, char * appel ){
	int n;
	char str[30];
	strcpy(str,perso->nom_attaque);
	strcat(str," -1 -1 0 0");
	attaque_t * attaque= (attaque_t*) creer_entite_chaine(ren,&n,perso,str,index,appel);
	ajouter_attaque(tab,attaque);
	return 0;
}

/**
	\brief En fonction du total de touches on actualise la vitesse du personnage	
	\param void Pointeur sur perso_t
	\param void Pointeur sur Joueur
	\param int Le total des touches (ex: 110 donne KEY_LEFT et KEY_JUMP)
	\return 1 si on met à jour la vitesse, 0 sinon
*/
static 
int input_update_speed (void * element, void * joueur,int tot_touche){
	perso_t * perso=(perso_t*)element;
	if(perso->temps_inv < 0 || perso->temps_inv > 1.0*S_INV /2){
		if(fait_partie_bin(tot_touche,KEY_ATT) && (perso->temps_att < 0|| perso->temps_att > perso->vit_attack)){
			perso->temps_att=0;
			return 1;
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
	return 0;
	
}

/**
	\brief Une entité execute une action sur une autre
	\param SDL_Renderer Pointeur sur le renderer
	\param void Entite qui agit
	\param void Entite qui subit
	\param void Tableau des entité
	\param err_t Tableau des fonctions de destruction
	\param FILE Fichier dans lequel chercher
	\param char appel de fichier
*/
static 
void perso_action_agit(SDL_Renderer * ren,void * ent_courante, void * ent_subit, void * tab[NB_MAX_AFF],err_t (*tab_destr[NB_MAX_AFF])(void ** ), FILE * index, char * appel ){
	if(((entite_t*)ent_courante)->contact(ent_courante,ent_subit))
		((entite_t*)ent_subit)->action_subit((entite_t*)ent_subit, ((perso_t*)ent_courante)->degats);
}


/**
	\brief Une entité execute une action sur une autre
	\param void Entité qui subit
	\param int Dégâts
*/
static 
void perso_action_subit(void * ent_courante, int degats){
	if(((perso_t*)ent_courante)->temps_inv < 0 && degats > 0){
		((perso_t*)ent_courante)->vitesse_x=-GRAVITE/7 ;
		((perso_t*)ent_courante)->vie-=degats;
		((perso_t*)ent_courante)->temps_inv=0;
	}
	if(((perso_t*)ent_courante)->vie<=0 ){
		((perso_t*)ent_courante)->vie=0;
		((perso_t*)ent_courante)->existe=FAUX;
	}
}


/**
	\brief Deplacement d'un personnage
	\param void Entité à déplacer
	\param double le temps
	\return Retourne 1 si les coordonnées sont bonnes, 2 si elles mènent en dehors de la salle, ou 0 sinon.
*/
static 
int perso_deplacement(void * element,double temps ){
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
        if(!ent->en_l_air((entite_t*)ent))
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


/**
	\brief Vérifie quel coin du chunk est le plus proche du personnage passé en paramètre
	\param perso_t Personnage à vérifier
	\return Combinaison de deux lettres: H/B pour Haut/Bas et G/D pour Gauche/Droite
*/
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

/**
	\brief Change un personnage de salle pour l'amener dans l'endroit courant
	\param perso_t Personnage à déplacer
	\return Retourne OK à l'issu du déplacement
*/
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

/**
	\brief Baisse la vie du personnage de dégâts
	\param perso_t Personnage blessé
	\param int Dégâts infligés
*/
static
void perso_prendre_coup(perso_t * personnage, int degats){
	personnage->vie -= degats;
}

/**
	\brief Remplit les tableaux à partir de fichiers de génération pour la salle où le personnage se trouve
	\param SDL_Renderer Pointeur sur le renderer
	\param perso_t Pointeur sur personnage
	\param void Tableau d'entités
	\param err_t Tableaux de fonctions de destruction
	\param FILE Fichier
	\param FILE Fichier de génération
	\param char appel de fichier
	\return Retourne 0 si tout se passe bien, 1 en cas d'erreur
*/
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
			if(element ==NULL){
				printf("La génération d'un élément n'a pas fonctionné\n");
			}
			if(n==1){
				ajouter_tableaux(tab,tab_destr,element,((perso_t*)element)->detruire);
			}
			if(n==2){
				ajouter_tableaux(tab,tab_destr,element,((ennemi_t*)element)->detruire);
			}
			else if (n==3){
				ajouter_tableaux(tab,tab_destr,element,((attaque_t*)element)->detruire);
			}
			else 
				ajouter_tableaux(tab,tab_destr,element,((entite_t*)element)->detruire);
			
		}
	}
	fseek(file_gen,0,SEEK_SET);
	fseek(index,0,SEEK_SET);
	return 0;
}

/**
	\brief Changer le perosnnage de chunk
	\param SDL_Renderer Pointeur sur le renderer
	\param perso_t Pointeur sur personnage
	\param void Tableau d'entités
	\param err_t Tableaux de fonctions de destruction
	\param attaque_t Tableau des attaques
	\param FILE Fichier
	\param FILE Fichier de génération
	\param char appel de fichier
*/
static 
err_t perso_change_chunk( SDL_Renderer * ren, perso_t * perso,  void * tab[NB_MAX_AFF], err_t (*tab_destr[NB_MAX_AFF])(void ** ),attaque_t * tab_attaque[NB_MAX_ATT], FILE *index, FILE * file_gen, char * appel){
	vider_tableaux(tab, tab_destr);
	vider_attaque(tab_attaque);
	remplir_tableaux(ren,perso,tab,tab_destr,appel,index,file_gen);
}

/**
	\brief Vérifie si le personnage est en vie
	\param perso_t Personnage que l'on vérifie
	\return Retourne VRAI si le personnage a plus de 0 points de vie, FAUX sinon.
*/
static
booleen_t en_vie(perso_t * personnage){

    if(personnage->vie > 0)
        return VRAI;
    else
        return FAUX;

}

/**
	\brief Création d'un personnage
	\param char Nom du personnage
	\param char Description du personnage
	\param salle_t Salle
	\param chunk_t Chunk du personnage
	\param pos_t Position dans le chunk
	\param float Vitesse horizontale du personnage
	\param float Vitesse sur l'axe des ordonnées
	\param float Vitesse maximale sur l'axe des ordonnées
	\param float vitesse de saut
	\param int Largeur du personnage
	\param int Hauteur du personnage
	\param int Largeur de la hitbox
	\param int Hauteur du personnage
	\param int Décalage de la hitbox
	\param float Sprite secondaires
	\param float Vitesse d'attaque
	\param int Dégâts d'attaque
	\param char Nom de l'attaque
	\param int Nombre de textures
	\param SDL_Texture Sprite du personnage
	\return Retourne le personnage créé.
*/
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
					 float vit_attack, int degats, char * nom_attaque,
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
	personnage->vie_max=vie;
	personnage->vie = vie;
	personnage->vit_attack = vit_attack;
	personnage->degats = degats;
	personnage->temps_att=-1;
	personnage->temps_inv=-1;

	
	personnage->detruire= perso_detruire;
	personnage->prendre_coup=perso_prendre_coup;
	personnage->update_speed=input_update_speed;
	personnage->copie_partiel=perso_copie_partiel;
	personnage->depop=perso_depop;
	personnage->envie=en_vie;
	personnage->deplacer=perso_deplacement;
	personnage->nouvelle_attaque=perso_creer_ajouter_attaque;
	personnage->change_chunk=perso_change_chunk;
	personnage->change_salle=perso_change_salle;
	personnage->afficher_chunk=perso_afficher_dans_chunk;
	personnage->action_subit=perso_action_subit;
	personnage->action_agit=perso_action_agit;

	personnage->nom_attaque=malloc(sizeof(char)* strlen(nom_attaque)+1);
	if(!(personnage->nom_attaque)){
		printf("Le nom d'attaque n'a pas pu être alloué: %s\n",nom_attaque );
		personnage->detruire(&personnage);
		return NULL;
	}
	strcpy(personnage->nom_attaque,nom_attaque);
	personnage->nom_attaque[strlen(nom_attaque)]='\0';

	return(personnage);
}





