/* Par Matthis */
#include <stdio.h>
#include <stdlib.h>

#include "../../lib/entite/attaque.h"

/**
* \file attaque.c
* \brief Module attaque
* \verion 1.0
* \date Avril 2021
*/


/**
    \brief Ajout d'une attaque dans le tableau
    \param attaque_t tableau des attaques
    \param void Attaque à ajouter
    \return Retourne 0 si l'ajout échoue
*/
extern 
int ajouter_attaque( attaque_t * tab[NB_MAX_ATT], attaque_t* ptr){
    int i;
    for(i=0;i<NB_MAX_ATT && tab[i]!=NULL; i++);
    if(i>=NB_MAX_ATT)
        return 0;
    tab[i]=ptr;
}

/**
    \brief Suppression de toutes les attaques du tableau et passage à NULL de celui-ci
    \param attaque_t tableau des attaques
*/
extern 
void enlever_attaque(attaque_t * tab[NB_MAX_ATT] ){
    int i;
    for(i=0;i<NB_MAX_ATT && tab[i]!=NULL;i++);
    if(i==NB_MAX_ATT)
        return;
    i--;
    (tab[i])->detruire(&(tab[i]));
    tab[i]=NULL;
}

/**
    \brief Vidage du tableau
    \param attaque_t tableau des attaques
*/
extern 
void vider_attaque(attaque_t * tab[NB_MAX_ATT] ){
    int i;
    while(tab[0]!=NULL){
        enlever_attaque(tab);
    }
}

/**
    \brief Affichage du tableau des attaques
    \param attaque_t tableau des attaques
    \param SDL_Renderer un point sur renderer
    \param int Largeur de la fenêtre
    \param int hauteur de la fenêtre
    \return retourne OK
*/
extern err_t afficher_attaques (attaque_t * tab_ent[NB_MAX_AFF], SDL_Renderer * ren, int WINW, int WINH){
    int i;
    for(i=0;i<NB_MAX_AFF && tab_ent[i]!=NULL; i++){
        ((entite_t*)tab_ent[i])->afficher_chunk(ren, (entite_t*)(tab_ent[i]), WINH, WINW);
    }
    return OK;
}

/**
    \brief Suppression d'une attaque du tableau
    \param attaque_t tableau des attaques
    \param void Attaque à enlever
*/
extern
void enlever_attaque_tableau(attaque_t * tab[NB_MAX_ATT],void *element ){
    int i,j;
    void * tempo=NULL;
    for(i=0;i<NB_MAX_ATT && tab[i]!=NULL && tab[i]!=element;i++);
    if(!(i>=NB_MAX_ATT || tab[i]==NULL)){
        for(j=0; j< NB_MAX_ATT && tab[j]!=NULL; j++);
            j--;
        echanger((void **)(&(tab[i])), (void **)(&(tab[j])));
        enlever_attaque(tab);
    }
}

/**
    \brief Actualisation du tableau des attaques
    \param attaque_t tableau des attaques
    \param double le temps
*/
extern 
void synchro_attaque(attaque_t * tab[NB_MAX_ATT],double temps){
    int i,j;
    for(i=0; i< NB_MAX_ATT && tab[i]!=NULL; i++){
        tab[i]->temps_vie+=temps;
        if(tab[i]->deplacer(tab[i],temps)==1){
            enlever_attaque_tableau(tab,tab[i]);
        }
    }
}

static 
int attaque_update_speed (void * perso, void * joueur,int tot_touche){
}

/**
    \brief Action de contact entre une entité et une attaque
    \param SDL_renderer pointeur sur le renderer
    \param attaque_t tableau des attaques
    \param void tableau des entités
    \param err_t Tableau de fonctions de destructions
    \param FILE index fichier de génération original des entités
    \param char appel de fichier
*/
extern
void contact_attaque_ennemis(SDL_Renderer* ren, 
                             attaque_t * tab_att[NB_MAX_ATT], 
                             void * tab[NB_MAX_AFF], 
                             err_t (*tab_destr[NB_MAX_AFF])(void ** ), 
                             FILE * index, char * appel){
    int i,j;
    for(i=0;i<NB_MAX_ATT && tab_att[i]!=NULL;i++){
        for(j=0;j<NB_MAX_AFF && tab[j]!=NULL;j++){
            if(((entite_t*)tab[j])->contact((entite_t*)tab[j],(entite_t*)tab_att[i])){
                ((entite_t*)tab[j])->action_subit(tab[j],tab_att[i]->degats);
            }
        }
    }
}


/**
    \brief Deplacement d'une entité
    \param void entité selectionnée
    \param double le temps
*/
static 
int attaque_deplacement(void * element,double temps ){
    pos_t pos_mur;
    chunk_t * chunk;
    booleen_t deja=FAUX;
    int w,h;
    float vitesse_tempo;
	entite_t * ent = (entite_t *) element;
    attaque_t * attaque = (attaque_t*)element;
    ent->lastSprite+=temps;
    attaque->temps_vie+=temps;
    if(attaque->temps_vie > attaque->duree_vie){
        attaque->existe=FAUX;
        return 1;
    }

    if(attaque->entite_liee != NULL){
        attaque->pos.x= attaque->entite_liee->pos.x;
        attaque->pos.y= attaque->entite_liee->pos.y;
        attaque->dir=attaque->entite_liee->dir;
        return 0;
    }

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

    if(!coord_correcte(ent->pos.x, ent->pos.y)){
        return 1;
    }   
    (ent->pos.y)+= (ent->vitesse_y)*temps;                  //On actualise la position horizontale grace a vitesse_y  
    if(coord_correcte(ent->pos.x ,ent->pos.y) && dans_mur(ent) && attaque->entite_liee==NULL){  
        return 1;
    }                                     
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
    if(coord_correcte(ent->pos.x ,ent->pos.y) && dans_mur(ent) && attaque->entite_liee==NULL){
        return 1;
    }  
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
        if(ent->vitesse_y > 0 && ent->dir == GAUCHE || ent->vitesse_y < 0 && ent->dir == DROITE)
            ent->vitesse_y=0;
    }
	
    return 0;
    

    
}

/**
    \brief Attaque
    \param SDL_renderer pointeur vers le renderer
    \param void attaque provoquée
    \param void entitée ciblée
    \param void tableau des entités
    \param err_t Tableau de fonctions de destructions
    \param FILE index fichier de génération original des entités
    \param char appel de fichier
*/
static 
void attaque_action_agit(SDL_Renderer * ren,void * attaque, void * element, void * tab[NB_MAX_AFF],err_t (*tab_destr[NB_MAX_AFF])(void ** ), FILE * index, char * appel ){
    if(((entite_t*)attaque)->contact(((entite_t*)attaque),((entite_t*)element))){
        ((entite_t*)element)->action_subit(element,((attaque_t*)attaque)->degats);
    }
}

/**
    \brief Suppression d'une attaque
    \param attaque_t L'attaque à supprimer
    \return Retourne OK si l'attaque est supprimée ou si elle n'existe pas. ERR_DEB_MEMOIRE en cas d'échec
*/
static
err_t attaque_detruire(attaque_t **  attaque){
    entite_t * ent;
    if(!(*attaque))
        return OK ;
    ent=realloc(*attaque,sizeof(entite_t));
    if(ent==NULL){
        printf("La destruction de l'attaque a échoué : %s\n", (*attaque)->nom);
        return ERR_DEB_MEMOIRE;
    }
    ent->detruire(&ent);
    return OK;
}

/**
    \brief fonction de création d'une attaque
    \param char nom de l'attaque
    \param char description de l'attaque
    \param salle_t salle de la carte où se trouve l'entité
    \param chunk_t chunk de la carte où se trouve l'entité
    \param pos_t position dans le chunk
    \param float vitesse de l'entité sur l'axe x
    \param float vitesse de l'entité sur l'axe y
    \param float vitesse maximale sur l'axe y d'un saut de l'entité,
    \param float sprite secondaire pour les animations
    \param int largeur de l'entité,
    \param int hauteur de l'entité,
    \param int largeur de la hitbox de l'entité
    \param int hauteur de la hitbox de l'entité
    \param int décalage de la hitbox par rapport à la position de l'entité
    \param booleen_t soumission ou non de l'entité à la gravité
    \param int dégâts infligés par l'entité si elle frappe
    \param float durée de vie de l'entité
    \param entite_t entité propriétaire de l'attaque
    \param int nombre de textures employées par l'entité
    \param SDL_Texture sprite de l'entité
    \return L'entité fraichement crée
*/
extern 
entite_t * attaque_creer(char * nom, 
                        char *description,
                        salle_t * salle,
                        chunk_t * chunk,
                        pos_t pos,
                        float vitesse_x, float vitesse_y, float vitesse_max_y,
                        float secSprite,
                        int w, int h, 
                        int w_hitbox, int h_hitbox, int offset_hitbox,
                        booleen_t gravite,
                        int degats,float duree_vie, 
                        entite_t * entite_liee,
                        int nbTextures,
                        SDL_Texture ** textures)
{
    attaque_t * attaque=NULL,* new_attaque= NULL;
	if ((attaque=(attaque_t*)entite_creer(nom,description,salle,chunk,pos,vitesse_x,vitesse_y,vitesse_max_y,secSprite,w,h,w_hitbox,h_hitbox,offset_hitbox,nbTextures,textures))==NULL){
		return NULL;
	}
	new_attaque=realloc(attaque,sizeof(attaque_t));
	if(new_attaque==NULL){
		printf("La réallocation du personnage %s n'a pas pu etre effectuée\n", nom);
		attaque->detruire_ent((entite_t **)(&attaque));
		return NULL;
	}
    attaque=new_attaque;
    attaque->degats=degats;
    attaque->gravite=gravite;
    attaque->duree_vie=duree_vie;
    attaque->temps_vie=0;
    attaque->entite_liee=entite_liee;

    attaque->update_speed=attaque_update_speed;
    attaque->action_agit=attaque_action_agit;
    attaque->detruire=attaque_detruire;
    attaque->deplacer=attaque_deplacement;
}