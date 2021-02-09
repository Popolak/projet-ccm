#include <stdio.h>
#include <stdlib.h>
#include "chunk.h"
#include "salle.h"

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

/*extern salle_t * salle_creer_type0(){
    int i,j;
    salle_t * salle =malloc(sizeof(salle_t));
    if (!salle){
        printf("L'allocation de la salle n'a pas fonctionné\n");
        return NULL;
    } 
    salle->chunks=malloc(sizeof(chunk_t *));
    if (salle->chunks[0]=chunk_creer(0,0)){
        for (i=0;i<CHUNKW; i++){
            for(j=0;j<CHUNKH; j++){
                salle->chunks[0]->chunk[i][j]->ecrire(&(salle->chunks[0]->chunk[i][j]),VIDE);
            }   
        }
    }
    else{
        printf("La génération du chunk a échoué\n");
        return NULL;
    }
    salle->type=0;
    salle->nb_chunk=1;
    return salle;
}
*/

/*extern salle_t * salle_creer_type(char * type){
    int i,j;
    salle_t *salle= malloc(sizeof(salle_t));
    salle->nb_chunk=type[0];
    for(i=0; i<salle->nb_chunk; i++){
        salle->chunks[i]=chunk_creer();

    }
}*/


extern salle_t * salle_creer(char * type){
    salle_t * salle=NULL;
    
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