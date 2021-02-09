#include <stdio.h>
#include <stdlib.h>
#include "../../lib/niveaux/chunk.h"
#include "../../lib/niveaux/salle.h"

int cpt_salle=0;

static void salle_lire(const salle_t * salle){
    int i;
    for(i=0; i<salle->nb_chunk; i++){
        salle->chunks[i]->lire(salle->chunks[i]);
    }
}

static err_t salle_detruire(salle_t ** salle){
    int i;
    if(*salle){
        if((*salle)->chunks){
            for(i=0; i<(*salle)->nb_chunk; i++){
                (*salle)->chunks[i]->detruire(&((*salle)->chunks[i]));
            }
            free((*salle)->chunks);
        }
        free(*salle);
        cpt_salle--;
    }
    return OK;
}

extern booleen_t salle_existe (salle_t *salle){
    if(salle)
        return VRAI;
    return FAUX;
}

extern salle_t * salle_creer_type(char * type){
    int i,j,k, xchunk,ychunk,nbPortes;
    char * type_chunk;
    salle_t *salle= malloc(sizeof(salle_t));
    salle->x=type[0]-'0';
    salle->y=type[1]-'0';
    salle->nb_chunk=type[2]-'0';
    for(i=0,k=3; i<salle->nb_chunk; i++,k++){
        xchunk=type[k++]-'0';
        ychunk=type[k++]-'0';
        nbPortes=type[k++]-'0';
        type_chunk=malloc(sizeof(char)*nbPortes);
        for(j=0;j<nbPortes;j++,k++)
            type_chunk[j]=type[k];
        k--;
        salle->chunks[i]=chunk_creer(xchunk,ychunk,nbPortes,type_chunk);
        free(type_chunk);
        type_chunk=NULL;
    }
}


extern salle_t * salle_creer(char * type){
    salle_t * salle=NULL;
    salle_creer_type(type);
    if (salle){
        salle->detruire=salle_detruire;
        salle->lire=salle_lire;
        cpt_salle++;
        return salle;
    }
    else {
        printf("La creation de la salle a echoue\n");
        return NULL;
    }
}

extern void salle_afficher_ref(){
    printf("%d\n", cpt_salle);
}