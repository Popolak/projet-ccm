#include <stdlib.h>
#include <ctype.h>
#include "../../lib/niveaux/chunk.h"

/* CONSTANTES */

int cpt_chunk=0;

/* FONCTIONS */

extern
pos_t prochain_mur (int x, int y, chunk_t * chunk){
    int i=0,j=0;
    pos_t pos;
    for(i=0;x+i<CHUNKH-CHUNKH*ratioSol;i+=TAILLE_MUR){
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

extern
void attribut_mur(int x, int y, int* w, int* h, chunk_t * chunk){
    int i,j;
    for(i=0; i<TAILLE_MUR && x+i < CHUNKH-CHUNKH*ratioSol && chunk->chunk[x+i][y]->contenu == MUR; i++);
    for(j=0; j<TAILLE_MUR && y+j < CHUNKW && chunk->chunk[x][y+j]->contenu == MUR; j++);
    *h=i;
    *w=j;
}

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

static
err_t remplir_surface(chunk_t * chunk, int x, int y , int h ,int w, int contenu){
    int i,j;
    for(i=x;i<x+h;i++){
        for(j=y;j<y+w;j++){
            chunk->chunk[i][j]->contenu=contenu;
        }
    }
}


static err_t chunk_remplir(chunk_t * chunk, int chunk_cote){
    int i;
    for(i=0;i<chunk->nb_portes;i++){
        switch (chunk->portes[i]->position){
            case (HG):
                chunk->remplir_surface(chunk,TAILLE_MUR,0,TAILLE_PORTE,TAILLE_MUR,VIDE);
                chunk->chunk[TAILLE_MUR+TAILLE_PORTE-1][0]->contenu=PORTE;
                break;
            case (BG):
                chunk->remplir_surface(chunk,CHUNKH-(ratioSol*CHUNKH)-TAILLE_PORTE,0,TAILLE_PORTE,TAILLE_MUR,VIDE);
                chunk->chunk[CHUNKH-(int)(ratioSol*CHUNKH)-1][0]->contenu=PORTE;
                break;
            case (HD):
                chunk->remplir_surface(chunk,TAILLE_MUR,CHUNKW-TAILLE_MUR,TAILLE_PORTE,TAILLE_MUR,VIDE);
                chunk->chunk[TAILLE_MUR+TAILLE_PORTE-1][CHUNKW-1]->contenu=PORTE;
                break;
            case (BD):
                chunk->remplir_surface(chunk,CHUNKH-ratioSol*CHUNKH-TAILLE_PORTE,CHUNKW-TAILLE_MUR,TAILLE_PORTE,TAILLE_MUR,VIDE);
                chunk->chunk[CHUNKH-(int)(ratioSol*CHUNKH)-1][CHUNKW-1]->contenu=PORTE;
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
                chunk->remplir_surface(chunk,CHUNKH-ratioSol*CHUNKH,TAILLE_MUR,CHUNKH*ratioSol,CHUNKW-2*TAILLE_MUR,VIDE);break;
            case(GAUCHE):
                chunk->remplir_surface(chunk,TAILLE_MUR,0,CHUNKH-TAILLE_MUR-ratioSol*CHUNKH,TAILLE_MUR,VIDE);break;
            case(COIN_NE):
                chunk->remplir_surface(chunk,0,CHUNKW-TAILLE_MUR,TAILLE_MUR,TAILLE_MUR,VIDE);break;
            case(COIN_SE):
                chunk->remplir_surface(chunk,CHUNKH-ratioSol*CHUNKH,CHUNKW-TAILLE_MUR,CHUNKH*ratioSol,TAILLE_MUR,VIDE);break;
            case(COIN_SO):
                chunk->remplir_surface(chunk,CHUNKH-ratioSol*CHUNKH,0,ratioSol*CHUNKH,TAILLE_MUR,VIDE);break;
            case(COIN_NO):
                chunk->remplir_surface(chunk,0,0,TAILLE_MUR,TAILLE_MUR,VIDE);break;
        }
        i/=2;
    }
    remplir_surface(chunk,TAILLE_MUR,TAILLE_MUR,CHUNKH-TAILLE_MUR-CHUNKH*ratioSol,CHUNKW-2*TAILLE_MUR,VIDE);
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
    int i,j,k,cote=0;
    char chaine_cote[3];
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
        if( !isdigit(type[i]) || !(chunk->portes[i]=porte_creer(NULL,type[i]-'0'))){
            chunk->detruire(&chunk);
            printf("L'allocation de la porte %d a échouée: %c\n", i, type[i]);
            return NULL;
        }
    }
    cote= atoi(&type[i]);
    chunk->remplir(chunk,cote);
    cpt_chunk++;
    return chunk;
}

extern void chunk_afficher_ref(){
    printf("Nombre ref de chunks: %d\n", cpt_chunk);
}
