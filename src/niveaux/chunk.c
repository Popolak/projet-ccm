#include <stdlib.h>
#include "chunk.h"

int cpt_chunk=0;


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

static void chunk_lire(const chunk_t * chunk){
    chunk->lire_partiel(chunk, 0,0,CHUNKW,CHUNKH);
}

static err_t chunk_detruire(chunk_t ** chunk){
    int i,j;
    unite_t *uniteAct;
    if (*chunk){
        if((*chunk)->chunk){
            for (i=0;i<CHUNKW; i++){
                for(j=0;j<CHUNKH; j++){
                    uniteAct= (*chunk)->chunk[i][j];
                    uniteAct->detruire(&uniteAct);
                    uniteAct=NULL;
                }
                free((*chunk)->chunk[i]);
                (*chunk)->chunk[i]=NULL;
            }
            free((*chunk)->chunk);
            (*chunk)->chunk=NULL;
        }
        for(i=0; i<(*chunk)->nb_portes; i++){
            (*chunk)->portes[i]->detruire(&((*chunk)->portes[i]));
            (*chunk)->portes[i]=NULL;
        }
        free((*chunk)->portes);
        (*chunk)->portes=NULL;
        free(*chunk);
        *chunk=NULL;
        cpt_chunk--;
    }
}

extern booleen_t chunk_existe(chunk_t * chunk){
    if (!chunk)
        return FAUX;
    return VRAI;
}

extern chunk_t * chunk_creer(int x,int y, int nb_portes, porte_t ** portes){
    unite_t * uniteAct=NULL;
    chunk_t * chunk=malloc(sizeof(chunk_t));
    int i,j;
    if (!chunk)
        return NULL;
    chunk->x=x;
    chunk->y=y;
    chunk->detruire=chunk_detruire;
    chunk->lire=chunk_lire;
    chunk->lire_partiel=chunk_lire_partiel;
    chunk->chunk= malloc(sizeof(unite_t**)*CHUNKW);
    if(chunk->chunk == NULL){
        chunk->detruire(&chunk);    
        return NULL;
    }
    for (i=0;i<CHUNKW; i++){
        chunk->chunk[i]= malloc(sizeof(unite_t*)*CHUNKH);
        for(j=0; j<CHUNKH; j++){
            chunk->chunk[i][j]= unite_creer(MUR,i,j);
        }
    }
    chunk->nb_portes=nb_portes;
    chunk->portes=malloc(sizeof(porte_t*)*nb_portes);
    for(i=0; i<nb_portes; i++){
        chunk->portes[i]=porte_creer(chunk,portes[i]->porteDest,portes[i]->position);
    }
    cpt_chunk++;
    return chunk;
}

extern void chunk_afficher_ref(){
    printf("Nombre ref : %d", cpt_chunk);
}
