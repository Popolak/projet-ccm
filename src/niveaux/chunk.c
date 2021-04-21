#include <stdlib.h>
#include <ctype.h>
#include "../../lib/niveaux/chunk.h"
#include "../../lib/niveaux/salle.h"
#include "string.h"
/**
* \file chunk.c
* \brief Module chunk
* \verion 1.0
* \date Avril 2021
*/
/* CONSTANTES */

int cpt_chunk=0;

/* FONCTIONS */


/**
    \brief Vérification de coordonnées
    \param int Coordonnée d'abscisse
    \param int Coordonnée d'ordonnée
    \return VRAI si les coordonnées sont correctes, FAUX sinon
*/
extern
booleen_t coord_correcte(int x, int y){
    return (x>=0 && x<CHUNKH && y >=0 && y<CHUNKW);
}

/**
    \brief Cherche les coordonnées du mur le plus proche
    \param int Coordonnée d'abscisse
    \param int Coordonnée d'ordonnée
    \chunk_t Chunk où l'on recherche
    \return Retourne la position de l'élément si on le trouve, sinon pos.x = -1.
*/
extern
pos_t prochain_mur (int x, int y, chunk_t * chunk){
    int i=0,j=0;
    pos_t pos;
    for(i=0;x+i<CHUNKH;i+=TAILLE_MUR){
        for(j=0;y+j<CHUNKW;j++){
            if(chunk->chunk[x+i][y+j]->contenu==MUR){
                pos.x=x+i;
                pos.y=y+j;
                return pos; 
            }
        }
        y=0;
    }
    pos.x=-1;
    return pos;
}

/**
    \brief Cherche les coordonnées du pont le plus proche
    \param int Coordonnée d'abscisse
    \param int Coordonnée d'ordonnée
    \param chunk_t Chunk où l'on recherche
    \return Retourne la position de l'élément si on le trouve, sinon pos.x = -1.
*/
extern
pos_t prochain_pont (int x, int y, chunk_t * chunk){
    int i=0,j=0;
    pos_t pos;
    for(i=0;x+i<CHUNKH;i+=H_PONT){
        for(j=0;y+j<CHUNKW;j++){
            if(chunk->chunk[x+i][y+j]->contenu==PONT){
                pos.x=x+i;
                pos.y=y+j;
                return pos; 
            }
        }
        y=0;
    }
    pos.x=-1;
    return pos;
}

/**
    \brief Calcule la taille d'un mur
    \param int Coordonnée d'abscisse
    \param int Coordonnée d'ordonnée
    \param int Pointeur sur la largeur du mur sue l'on mesure, sera édité
    \param int Pointeur sur la hauteur du mur sue l'on mesure, sera édité
    \param chunk_t Chunk où l'on recherche
*/
extern
void attribut_mur(int x, int y, int* w, int* h,chunk_t * chunk){
    int i,j;
    chunk_t * chunk_dessous=NULL;
    for(i=0; i<TAILLE_MUR && x+i < CHUNKH  && chunk->chunk[x+i][y]->contenu == MUR; i++);
    for(j=0; j<TAILLE_MUR && y+j < CHUNKW && chunk->chunk[x][y+j]->contenu == MUR; j++);
    *h=i;
    *w=j;
}

/**
    \brief Calcule la taille d'un pont
    \param int Coordonnée d'abscisse
    \param int Coordonnée d'ordonnée
    \param int Pointeur sur la largeur du pont sue l'on mesure, sera édité
    \param int Pointeur sur la hauteur du pont sue l'on mesure, sera édité
    \param chunk_t Chunk où l'on recherche
*/
extern
void attribut_pont(int x, int y, int* w, chunk_t * chunk){
    int i,j;
    for(j=0; j<W_PONT && y+j < CHUNKW && chunk->chunk[x][y+j]->contenu == PONT; j++);
    *w=j;
}

/**  
    \brief Recherche d'une porte à la position pos
    \param chunk_t pointeur sur chunk_t où l'on cherche
    \param int Position de la porte que l'on vérifie
    \return Retourne un pointeur sur la porte a la position pos si elle existe, NULL sinon
*/
static porte_t * chercher_porte(const chunk_t * chunk, int pos){
    int i;
    for(i=0; i<chunk->nb_portes; i++){
        if (chunk->portes[i]->position==pos)
            return chunk->portes[i];
    }
    return NULL;
}

/**
    \brief Affiche en terminal le contenu d'un chunk dans la limite des coordonées et tailels demandées
        \param chunk_t Pointeur sur chunk où l'on chercher
        \param int Abscisse de départ
        \param int Ordonnée de départ
        \param int Taille d'abscisse à lire
        \param int Taille d'ordonnée à lire
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

/**
    \brief Lit la totalité d'un chunk donné
    \param chunk_t Pointeur sur le chunk à lire
*/

static void chunk_lire(const chunk_t * chunk){
    chunk->lire_partiel(chunk, 0,0,CHUNKH,CHUNKW);
}

/**
    \brief Détruit un chunk et tout son contenu
    \param chunk_t Pointeur sur pointeur sur le chunk à détruire
    \return Retourne OK si la destruction s'est bien passée
*/
static err_t chunk_detruire(chunk_t ** chunk){
    int i,j;
    unite_t *uniteAct;
    if (*chunk){
        if((*chunk)->chunk){
            for (i=0;i<CHUNKH; i++){
                if((*chunk)->chunk[i]){
                    for(j=0;j<CHUNKW; j++){                               //Pour chaque lignes de chunk
                        if(uniteAct= (*chunk)->chunk[i][j]){                 
                            uniteAct->detruire(&uniteAct);                    //On détruit chaque unité de cette ligne
                            uniteAct=NULL;
                        }
                    }
                    free((*chunk)->chunk[i]);                             //Puis on détruit la ligne
                    (*chunk)->chunk[i]=NULL;
                }
            }
            free((*chunk)->chunk);                                    //Enfin on détruit la  matrice
            (*chunk)->chunk=NULL;
        }
        if((*chunk)->portes){
            for(i=0; i<(*chunk)->nb_portes; i++){                         //On détruit chaque porte
                (*chunk)->portes[i]->detruire(&((*chunk)->portes[i]));
                (*chunk)->portes[i]=NULL;
            }
            free((*chunk)->portes);                                       //On détruit le tableau de portes
            (*chunk)->portes=NULL;
        }
        free(*chunk);                                                 //Et on détruit le chunk
        *chunk=NULL;
        cpt_chunk--;
    }
}

/**
    \brief Vérifie si un chunk existe
    \param chunk_t Pointeur sur le chunk à vérifier
    \return Retourne VRAI si le chunk existe, FAUX sinon
*/
extern booleen_t chunk_existe(chunk_t * chunk){
    if (!chunk)
        return FAUX;
    return VRAI;
}

/**
    \brief Remplit la surface demandée avec le type de contenu donné
    \param chunk_t Pointeur sur le chunk à remplir
    \param int Abscisse de départ
    \param int Ordonnée de départ
    \param int Taille d'abscisse à remplir 
    \param int Taille d'ordonnée à remplir 
    \param int Contenu utilisé pour le remplissage
*/
static
err_t remplir_surface(chunk_t * chunk, int x, int y , int h ,int w, int contenu){
    int i,j;
    for(i=x;i<x+h;i++){
        for(j=y;j<y+w;j++){
            chunk->chunk[i][j]->contenu=contenu;
        }
    }
}

/**
    \brief Création de pont
    \param chunk_t Pointeur sur le chunk où créer
    \param char Pointeur sur la position des ponts
    \return Retourne OK après la création
*/
extern 
err_t creer_pont_chaine(chunk_t * chunk,char * type){
    int i, x,y,pos, depart_x, depart_y, taille_pont;
    char str[3];
    int relie, total_w, total_h;
    str[0]=type[0];
    str[1]=type[1];
    str[2]=type[2];
    relie=atoi(str);
    
    total_w=CHUNKW-2*TAILLE_MUR;
    depart_y=TAILLE_MUR;
    if(relie & DROITE){
        total_w+=TAILLE_MUR;
    }
    if(relie & GAUCHE){
        total_w+=TAILLE_MUR;
        depart_y=0;
    }
    taille_pont=total_w/5;
    for(i=3;type[i];i++){
        pos=type[i]-'a';
        if(pos < 5 && pos >= 0){
            x=TAILLE_MUR+TAILLE_PORTE;
        }
        if(pos >= 5 && pos < 10){
            x= (CHUNKH-CHUNKH*ratioSol + TAILLE_PORTE+TAILLE_MUR)/2;
            pos-=5;
        }
        else if(pos>= 10){
            x=CHUNKH-CHUNKH*ratioSol;
            pos-=10;
        }
        y=depart_y+ taille_pont*pos;
        chunk->remplir_surface(chunk,x,y,H_PONT, taille_pont ,PONT);
    }
    return OK;
}


/**
    \brief Remplissage d'un chunk
    \param chunk_t Pointeur sur le chunk à remplir
    \param int Taille du chunk
*/
static err_t chunk_remplir(chunk_t * chunk, int chunk_cote){
    int i;
    for(i=0;i<chunk->nb_portes;i++){
        switch (chunk->portes[i]->position){
            case (HG):
                chunk->remplir_surface(chunk,TAILLE_MUR,0,TAILLE_PORTE,TAILLE_MUR,VIDE);
                chunk->remplir_surface(chunk,TAILLE_MUR,0,TAILLE_PORTE,5,PORTE);
                break;
            case (BG):
                chunk->remplir_surface(chunk,CHUNKH-(ratioSol*CHUNKH)-TAILLE_PORTE,0,TAILLE_PORTE,TAILLE_MUR,VIDE);
                chunk->remplir_surface(chunk,CHUNKH-(ratioSol*CHUNKH)-TAILLE_PORTE,0,TAILLE_PORTE,5,PORTE);
                
                break;
            case (HD):
                chunk->remplir_surface(chunk,TAILLE_MUR,CHUNKW-TAILLE_MUR,TAILLE_PORTE,TAILLE_MUR,VIDE);
                chunk->remplir_surface(chunk,TAILLE_MUR,CHUNKW-6,TAILLE_PORTE,5,PORTE);
                break;
            case (BD):
                chunk->remplir_surface(chunk,CHUNKH-ratioSol*CHUNKH-TAILLE_PORTE,CHUNKW-TAILLE_MUR,TAILLE_PORTE,TAILLE_MUR,VIDE);
                chunk->remplir_surface(chunk,CHUNKH-ratioSol*CHUNKH-TAILLE_PORTE,CHUNKW-6,TAILLE_PORTE,5,PORTE);
                break;
        }
    }
    i=COIN_NO;
    while(chunk_cote>0){
        for(;i>chunk_cote;i/=2);
        chunk_cote-=i;
        switch(i){
            case(HAUT):
                chunk->remplir_surface(chunk,0,TAILLE_MUR,TAILLE_MUR,CHUNKW-2*TAILLE_MUR,VIDE); break;
            case(DROITE):
                chunk->remplir_surface(chunk,TAILLE_MUR,CHUNKW-TAILLE_MUR,CHUNKH-TAILLE_MUR-ratioSol*CHUNKH,TAILLE_MUR,VIDE);break;
            case(BAS):
                chunk->remplir_surface(chunk,CHUNKH-ratioSol*CHUNKH,TAILLE_MUR,CHUNKH*ratioSol+1,CHUNKW-2*TAILLE_MUR,VIDE);break;
            case(GAUCHE):
                chunk->remplir_surface(chunk,TAILLE_MUR,0,CHUNKH-TAILLE_MUR-ratioSol*CHUNKH,TAILLE_MUR,VIDE);break;
            case(COIN_NE):
                chunk->remplir_surface(chunk,0,CHUNKW-TAILLE_MUR,TAILLE_MUR,TAILLE_MUR,VIDE);break;
            case(COIN_SE):
                chunk->remplir_surface(chunk,CHUNKH-ratioSol*CHUNKH,CHUNKW-TAILLE_MUR,CHUNKH*ratioSol+1,TAILLE_MUR,VIDE);break;
            case(COIN_SO):
                chunk->remplir_surface(chunk,CHUNKH-ratioSol*CHUNKH,0,ratioSol*CHUNKH+1,TAILLE_MUR,VIDE);break;
            case(COIN_NO):
                chunk->remplir_surface(chunk,0,0,TAILLE_MUR,TAILLE_MUR,VIDE);break;
        }
        i/=2;
    }
    remplir_surface(chunk,TAILLE_MUR,TAILLE_MUR,CHUNKH-TAILLE_MUR-CHUNKH*ratioSol,CHUNKW-2*TAILLE_MUR,VIDE);
}

/**
    \brief Fonction de création de chunk
    \param int Position d'abscisse du chunk dans la salle
    \param int Position d'ordonnée du chunk dans la salle
    \param int Nombres de portes dans le chunk
    \param char chaine donnant les positions de chaque porte
    \return Le chunk créé
*/
extern chunk_t * chunk_creer(salle_t *salle,int x,int y, int nb_portes, char * type){
    unite_t * uniteAct=NULL;
    chunk_t * chunk=malloc(sizeof(chunk_t));
    int i,j,k,cote=0;
    char chaine_pont[20];
    if (!chunk)
        return NULL;
    if(x <0 || y<0){
        printf("Position incorecte: %d;%d\n",x,y);
        free(chunk);
        return NULL;
    }
    chunk->position.x=x;                        
    chunk->position.y=y;
    chunk->detruire=chunk_detruire;
    chunk->lire=chunk_lire;                                             //Initialisation des méthodes
    chunk->lire_partiel=chunk_lire_partiel;
    chunk->chercher_porte=chercher_porte;
    chunk->remplir=chunk_remplir;
    chunk->remplir_surface=remplir_surface;

    chunk->chunk= malloc(sizeof(unite_t**)*CHUNKH);                     //
    if(chunk->chunk == NULL){                                           //
        printf("L'allocation du tableau de tableau d'unité a échouée\n");
        chunk->detruire(&chunk);                                        // 
        return NULL;                                                    //
    }                                                                   //
    for (i=0;i<CHUNKH; i++){                                            //allocation de la matrice d'unité
        if(!(chunk->chunk[i]= malloc(sizeof(unite_t*)*CHUNKW))){        //
            printf("L'allocation pour le tableau %d d'unités a échouée\n", i);
            chunk->detruire(&chunk);                                    //
            return NULL;                                                //
        }                                                               //
        for(j=0; j<CHUNKW; j++){                                        //
            if(!(chunk->chunk[i][j]= unite_creer(MUR))){                //
                printf("La création de l'unité a échouée\n");
                chunk->detruire(&chunk);                                //
                return NULL;                                            //
            }                                                           //
        }                                                               //
    }                                                                   
    chunk->nb_portes=nb_portes;                                         
    if (!(chunk->portes=malloc(sizeof(porte_t*)*nb_portes))){                   //allocation du tableau de pointeurs vers portes
        chunk->detruire(&chunk);
        printf("L'allocation des portes a échouée\n");
        return NULL;
    }
    for(i=0; i<nb_portes; i++){
        if( !isdigit(type[i]) || !(chunk->portes[i]=porte_creer(NULL,type[i]-'0',salle,chunk))){
            chunk->detruire(&chunk);
            printf("L'allocation de la porte %d a échouée: %c\n", i, type[i]);
            return NULL;
        }
    }
    cote= atoi(&type[i]);
    chunk->remplir(chunk,cote);
    strcpy(chaine_pont,&type[i]);
    creer_pont_chaine(chunk,chaine_pont);
    cpt_chunk++;

    return chunk;
}

/**
    \brief Affiche dans un printf le nombre de référence du chunk
*/
extern void chunk_afficher_ref(){
    printf("Nombre ref de chunks: %d\n", cpt_chunk);
}
