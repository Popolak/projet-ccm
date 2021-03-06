#include <stdlib.h>
#include <ctype.h>
#include "../../lib/niveaux/salle.h"

/**
* \file salle.c
* \brief Module salle
* \verion 1.0
* \date Avril 2021
*/

/* CONSTANTES */

int cpt_salle=0;

/* FONCTIONS */
/*

extern 
int chunk_entoure(salle_t * salle, chunk_t*  chunk){
    int tot=RIEN;
    if (chercher_chunk(salle,chunk->position.x-1,chunk->position.y)){
        tot+=HAUT;
    }
    if (chercher_chunk(salle,chunk->position.x,chunk->position.y+1)){
        tot+=DROITE;
    }
    if (chercher_chunk(salle,chunk->position.x+1,chunk->position.y)){
        tot+=BAS;
    }
    if (chercher_chunk(salle,chunk->position.x,chunk->position.y-1)){
        tot+=GAUCHE;
    }
    if (chercher_chunk(salle,chunk->position.x-1,chunk->position.y) && chercher_chunk(salle,chunk->position.x,chunk->position.y+1) && chercher_chunk(salle,chunk->position.x-1,chunk->position.y+1)){
        tot+=COIN_NE;
    }
    if (chercher_chunk(salle,chunk->position.x+1,chunk->position.y) && chercher_chunk(salle,chunk->position.x,chunk->position.y+1) && chercher_chunk(salle,chunk->position.x+1,chunk->position.y+1)){
        tot+=COIN_SE;
    }
    if (chercher_chunk(salle,chunk->position.x+1,chunk->position.y) && chercher_chunk(salle,chunk->position.x,chunk->position.y-1) && chercher_chunk(salle,chunk->position.x+1,chunk->position.y-1)){
        tot+=COIN_SO;
    }
    if (chercher_chunk(salle,chunk->position.x-1,chunk->position.y) && chercher_chunk(salle,chunk->position.x,chunk->position.y-1) && chercher_chunk(salle,chunk->position.x-1,chunk->position.y-1)){
        tot+=COIN_NO;
    }
    return tot;
}
*/

/**
    \brief Vérification du type
    \param char Position de la porte
    \return Retourne VRAI si le type est correcte, FAUX sinon
*/
extern 
booleen_t type_correct(char * type){
    
    int i;
    for(i=0;type[i]!='\n' && type[i];i++){
        if(!isdigit(type[i]))
         return FAUX;
    }
    return VRAI;
}

/**
    \brief Cherche le chunk aux coordonnées et dans la salle donnée.
    \param salle_t Pointeur sur la salle dans laquelle on regarde
    \param int Coordonnée d'abscisses où chercher dans la salle
    \param int Coordonnée d'ordonnées où chercher dans la salle
    \return Retourne le chunk si on en trouve un, NULL sinon
*/
static chunk_t * chercher_chunk(const salle_t * salle, int x, int y){
    int i;
    for(i=0;i<salle->nb_chunk; i++){
        if ((salle->chunks[i]->position.x==x) && (salle->chunks[i]->position.y==y))
            return salle->chunks[i];
    }
    return NULL;
}

/**
    \brief Calcule la largeur de la maximale de la salle
    \param salle_t Salle à mesurer
    \return Retourne la largeur de la salle
*/
static int chercher_max_x_chunk(salle_t * salle){
    int i,xmax=0;
    for(i=0;i<salle->nb_chunk;i++){
        xmax = xmax > salle->chunks[i]->position.x ? xmax : salle->chunks[i]->position.x;
    }
    return xmax;
}

/**
    \brief Calcule la hauteur de la maximale de la salle
    \param salle_t Salle à mesurer
    \return Retourne la hauteur de la salle
*/
static int chercher_max_y_chunk(salle_t * salle){
    int i,ymax=0;
    for(i=0;i<salle->nb_chunk;i++){
        ymax = ymax > salle->chunks[i]->position.y ? ymax : salle->chunks[i]->position.y;
    }
    return ymax;
}


/**
    \brief Lecture de la salle
    \param salle_t Salle a lire
*/
static void salle_lire(const salle_t * salle){
    int i,j;
    printf("Salle %.0f;%.0f : \n",salle->position.x, salle->position.y);
    for(i=0; i<salle->nb_chunk;i++){       //Affiche les chunks qui composent la salle et chaque portes correspondant au chunk
        printf("chunk %.0f;%.0f :\nnombre de porte(s): %d \n", salle->chunks[i]->position.x,salle->chunks[i]->position.y,salle->chunks[i]->nb_portes);
        for(j=0;j<salle->chunks[i]->nb_portes;j++){
            printf("Porte %d : position : %d\nPointeur : %p\n",j,salle->chunks[i]->portes[j]->position,salle->chunks[i]->portes[j]);
            printf("reliée à %p\n", salle->chunks[i]->portes[j]->porteDest);
        }
        printf("\n");
    }
    printf("\n");
}

/**
    \brief Destruction d'une salle
    \param salle_t Pointeur sur pointeur sur la salle a détruire
    \return Retourne OK après la destruction
*/
static err_t salle_detruire(salle_t ** salle){
    int i;
    if(*salle){
        if((*salle)->chunks){
            for(i=0; i<(*salle)->nb_chunk; i++){                //On détruit chaque chunk composant la salle
                if((*salle)->chunks[i]){
                    (*salle)->chunks[i]->detruire(&((*salle)->chunks[i]));
                    (*salle)->chunks[i]=NULL;
                }
            }
            free((*salle)->chunks);             //On détruit le tableau de chunks
            (*salle)->chunks=NULL;
        }
        free(*salle);                           //Puis on détruit la salle 
        *salle=NULL;
        cpt_salle--;
    }
    return OK;
}

/**
    \brief Vérification d'existence d'une salle
    \salle_t Pointeur sur la salle à tester
    \return Retourne VRAI si la salle existe, FAUX sinon 
*/
extern booleen_t salle_existe (salle_t *salle){
    if(salle)
        return VRAI;
    return FAUX;
}

/**
    \brief Création d'une salle a partir d'une référence
    \param char Chaine de caractères (cf ./generation/generation_explication.txt)
    \return Retourne un pointeur sur salle_t si la salle est crée, NULL sinon 
*/
extern salle_t * salle_creer_type(char *type){
    int i=0,j=0,k=0, xchunk=0,ychunk=0,nbPortes=0, tot=0, taille;
    char * type_chunk=NULL;
    salle_t *salle= malloc(sizeof(salle_t));
    if(!salle){
        printf("L'allocation de la salle a échouée\n");
        return NULL;
    }                                                                   
                                                                        //Exemple : 012001001223
    salle->position.x=type[0]-'0';                                      //0 12001001223   x pour la salle
    salle->position.y=type[1]-'0';                                      //0 1 2001001223  y pour la salle                            
    salle->nb_chunk=type[2]-'0';                                        //01 2 001001223  nombre de chunks dans la salle
    if(!(salle->chunks=malloc(sizeof(chunk_t)*salle->nb_chunk))){
        printf("L'allocation pour le tableau de chunk a échouée, nombre de chunk : %d\n", salle->nb_chunk);
        salle_detruire(&salle);
        return NULL;
    }
    for(i=0,k=3, taille=0; i<salle->nb_chunk; i++){
        xchunk=type[k++]-'0';                                           //012 0 01001223  x pour le premier chunk | 0120010 0 1223 x pour le deuxième
        ychunk=type[k++]-'0';                                           //0120 0 1001223  y pour le premier chunk | 01200100 1 223 y pour le deuxième 
        nbPortes=type[k++]-'0';                                         //01200 1 001223  nombre de portes dans le premier chunk | 012001201 2 23
        for(taille=0;type[k+taille]!=' ' && type[k+taille]!='\n' && type[k+taille]!='\0'; taille++);
        if(!(type_chunk=malloc(sizeof(char)*(taille+1)))){
            printf("L'allocation de la chaine de caractères pour le type du chunk %d %d a échouée\n", xchunk,ychunk);
            salle_detruire(&salle);
            return NULL;
        }
        for(j=0;j<taille;j++,k++){                                     //On créer la type pour le chunk
            type_chunk[j]=type[k];                                              //0 pour le premier | 23 pour le deuxième
        }
        type_chunk[taille]='\0';
        if(!(salle->chunks[i]=chunk_creer(salle,xchunk,ychunk,nbPortes,type_chunk))){
            printf("La création du chunk %d %d a échouée\n", xchunk,ychunk);
            salle_detruire(&salle);
            return NULL;
        }
        free(type_chunk);
        type_chunk=NULL;
        k++;
    }
    return salle;
}

/**
    \brief Création d'une salle
    \param char Chaine de caractères (cf ./generation/generation_explication.txt)
    \return Retourne un pointeur sur la salle créée, NULL sinon 
*/
extern salle_t * salle_creer(char * type){
    salle_t * salle=NULL;
    salle=salle_creer_type(type);
    if (salle){
        salle->detruire=salle_detruire;
        salle->lire=salle_lire;
        salle->chercher_chunk=chercher_chunk;
        cpt_salle++;
        return salle;
    }
    else {
        return NULL;
    }
}

/**
    \brief Affiche la référence de la salle par printf
*/
extern void salle_afficher_ref(){
    printf("Nombre de ref de salle : %d\n", cpt_salle);
}