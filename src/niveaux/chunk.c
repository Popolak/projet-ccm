#include <stdlib.h>
#include "../../lib/niveaux/chunk.h"

/* CONSTANTES */

int cpt_chunk=0;

/* FONCTIONS */

/*  fonction chercher_porte:
    paramètres:
        chunk: pointeur sur chunk_t
        pos : position de la porte
    retourne un pointeur sur la porte a la position pos si elle existe, NULL sinon
*/

static porte_t * chercher_porte(const chunk_t * chunk, int pos){
    int i;
    for(i=0; i<chunk->nb_portes; i++){
        if (chunk->portes[i]->position==pos)
            return chunk->portes[i];
    }
    return NULL;
}

/*  fonction chunk_lire_partiel
    paramètres: 
        chunk: pointeur sur chunk_t
        x et y : valeurs a partir desquelles on veut lire le chunk
        w et h : width et height de ce qu'on veut lire
*/

static void chunk_lire_partiel(const chunk_t * chunk,int x,int y,int w,int h){
    int i,j;
    if (chunk!=NULL){
        unite_t *uniteAct=NULL;
        for (i=x; i<x+w;i++){
            for (j=y;j<y+h; j++){
                uniteAct=chunk->chunk[i][j];
                printf("%d ", uniteAct->lire(uniteAct));
            }
            printf("\n");
        }
    }
}

/*  fonction chunk_lire
    paramètre: 
        chunk: pointeur sur chunk_t
    Lit la totalité d'un chunk donné
*/

static void chunk_lire(const chunk_t * chunk){
    chunk->lire_partiel(chunk, 0,0,CHUNKH,CHUNKW);
}

/*  fonction chunk_detruire
    paramètre:
        chunk : pointeur sur pointeur sur chunk_t 
    retourne OK si la destruction s'est bien passée
*/

static err_t chunk_detruire(chunk_t ** chunk){
    int i,j;
    unite_t *uniteAct;
    if (*chunk){
        if((*chunk)->chunk){
            for (i=0;i<CHUNKH; i++){
                for(j=0;j<CHUNKW; j++){                               //Pour chaque lignes de chunk
                    uniteAct= (*chunk)->chunk[i][j];                 
                    uniteAct->detruire(&uniteAct);                    //On détruit chaque unité de cette ligne
                    uniteAct=NULL;
                }
                free((*chunk)->chunk[i]);                             //Puis on détruit la ligne
                (*chunk)->chunk[i]=NULL;
            }
            free((*chunk)->chunk);                                    //Enfin on détruit la  matrice
            (*chunk)->chunk=NULL;
        }
        for(i=0; i<(*chunk)->nb_portes; i++){                         //On détruit chaque porte
            (*chunk)->portes[i]->detruire(&((*chunk)->portes[i]));
            (*chunk)->portes[i]=NULL;
        }
        free((*chunk)->portes);                                       //On détruit le tableau de portes
        (*chunk)->portes=NULL;
        free(*chunk);                                                 //Et on détruit le chunk
        *chunk=NULL;
        cpt_chunk--;
    }
}

/*  fonction chunk_existe
    paramètre:
        chunk: pointeur sur chunk_t
    retourne VRAI si chunk existe , FAUX sinon
*/

extern booleen_t chunk_existe(chunk_t * chunk){
    if (!chunk)
        return FAUX;
    return VRAI;
}

/*  fonction chunk_creer
    paramètres:
        x et y : position du chunk dans la salle
        nb_portes : nombres de portes dans le chunk
        type : chaine donnant les positions de chaque porte
*/

extern chunk_t * chunk_creer(int x,int y, int nb_portes, char * type){
    unite_t * uniteAct=NULL;
    chunk_t * chunk=malloc(sizeof(chunk_t));
    int i,j;
    if (!chunk)
        return NULL;
    chunk->position.x=x;                        
    chunk->position.y=y;
    chunk->detruire=chunk_detruire;
    chunk->lire=chunk_lire;                             //Initialisation des méthodes
    chunk->lire_partiel=chunk_lire_partiel;
    chunk->chercher_porte=chercher_porte;
    chunk->chunk= malloc(sizeof(unite_t**)*CHUNKH);                     //
    if(chunk->chunk == NULL){                                           //
        chunk->detruire(&chunk);                                        // 
        return NULL;                                                    //
    }                                                                   //
    for (i=0;i<CHUNKH; i++){                                            //allocation de la matrice d'unité
        chunk->chunk[i]= malloc(sizeof(unite_t*)*CHUNKW);               //
        for(j=0; j<CHUNKW; j++){                                        //
            chunk->chunk[i][j]= unite_creer(MUR);                       //
        }                                                               //
    }                                                                   
    chunk->nb_portes=nb_portes;                                         
    chunk->portes=malloc(sizeof(porte_t*)*nb_portes);                   //allocation du tableau de pointeurs vers portes
    for(i=0; i<nb_portes; i++){
        chunk->portes[i]=porte_creer(NULL,type[i]-'0');
        switch(type[i]-'0'){                                                //En fonction de la position de la porte, on la place a un certain endroit
            case(0):
                chunk->chunk[0][0]->ecrire(&(chunk->chunk[0][0]),PORTE);
                break;
            case(1):
                chunk->chunk[0][CHUNKW-1]->ecrire(&(chunk->chunk[0][CHUNKW-1]),PORTE);
                break;
            case(2):
                chunk->chunk[CHUNKH-1][0]->ecrire(&(chunk->chunk[CHUNKH-1][0]),PORTE);
                break;
            case(3):
                chunk->chunk[CHUNKH-1][CHUNKW-1]->ecrire(&(chunk->chunk[CHUNKH-1][CHUNKW-1]),PORTE);
                break;
        }
    }
    cpt_chunk++;
    return chunk;
}

extern void chunk_afficher_ref(){
    printf("Nombre ref de chunks: %d\n", cpt_chunk);
}
