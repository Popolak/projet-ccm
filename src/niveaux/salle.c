#include <stdlib.h>
#include "../../lib/niveaux/salle.h"

/* CONSTANTES */

int cpt_salle=0;

/* FONCTIONS */

/*  fonction chercher_chunk
    paramètres:
        salle: pointeur sur salle_t
        x et y : int, position du chunk dans la salle
    Retourne un pointeur sur le chunk aux positions x;y dans la salle
    Retrouve un chunk dans une salle a partir de ses coordonées
*/

static chunk_t * chercher_chunk(const salle_t * salle, int x, int y){
    int i;
    for(i=0;i<salle->nb_chunk; i++){
        if ((salle->chunks[i]->position.x==x) && (salle->chunks[i]->position.y==y))
            return salle->chunks[i];
    }
    return NULL;
}

/*  fonction salle_lire
    paramètre:
        salle: pointeur sur salle 
*/

static void salle_lire(const salle_t * salle){
    int i,j;
    for(i=0; i<salle->nb_chunk;i++){       //Affiche les chunks qui composent la salle et chaque portes correspondant au chunk
        printf("chunk %d;%d :\nnombre de porte(s): %d \n", salle->chunks[i]->position.x,salle->chunks[i]->position.y,salle->chunks[i]->nb_portes);
        for(j=0;j<salle->chunks[i]->nb_portes;j++){
            printf("Porte %d : position : %d\nPointeur : %p\n",j,salle->chunks[i]->portes[j]->position,salle->chunks[i]->portes[j]);
            printf("reliée à %p\n", salle->chunks[i]->portes[j]->porteDest);
        }
        printf("\n");
    }
    printf("\n");
}

/*  fonction salle_detruire
    paramètre: 
        salle: pointeur sur pointeur sur salle
    reoturne OK si tout s'est bien passé
*/

static err_t salle_detruire(salle_t ** salle){
    int i;
    if(*salle){
        if((*salle)->chunks){
            for(i=0; i<(*salle)->nb_chunk; i++){                //On détruit chaque chunk composant la salle
                (*salle)->chunks[i]->detruire(&((*salle)->chunks[i]));
                (*salle)->chunks[i]=NULL;
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

/*  fonction salle_existe
    paramètre:
        salle: pointeur sur salle_t
    Retourne VRAI si la salle existe FAUX sinon 
*/

extern booleen_t salle_existe (salle_t *salle){
    if(salle)
        return VRAI;
    return FAUX;
}

/*  fonction salle_creer_type
    paramètre:
        type: chaine de caractères (cf ./generation/creation_generation_explication.txt)
    Retourne un pointeur sur salle_t si la salle est crée, NULL sinon 
*/

extern salle_t * salle_creer_type(char *type){
    int i=0,j=0,k=0, xchunk=0,ychunk=0,nbPortes=0;
    char * type_chunk=NULL;
    salle_t *salle= malloc(sizeof(salle_t));
    if(!salle){
        printf("Echec de la creation de la salle\n");
        return NULL;
    }                                                                   //Exemple : 012001001223
    salle->position.x=type[0]-'0';                                      //0 12001001223   x pour la salle
    salle->position.y=type[1]-'0';                                      //0 1 2001001223  y pour la salle
    salle->nb_chunk=type[2]-'0';                                        //01 2 001001223  nombre de chunks dans la salle
    salle->chunks=malloc(sizeof(chunk_t)*salle->nb_chunk);
    for(i=0,k=3; i<salle->nb_chunk; i++){
        xchunk=type[k++]-'0';                                           //012 0 01001223  x pour le premier chunk | 0120010 0 1223 x pour le deuxième
        ychunk=type[k++]-'0';                                           //0120 0 1001223  y pour le premier chunk | 01200100 1 223 y pour le deuxième 
        nbPortes=type[k++]-'0';                                         //01200 1 001223  nombre de portes dans le premier chunk | 012001201 2 23 
        type_chunk=malloc(sizeof(char)*nbPortes);
        for(j=0;j<nbPortes;j++,k++)                                     //A partir de chaque porte, on créer la type pour la chunk
            type_chunk[j]=type[k];                                      //0 pour le premier | 23 pour le deuxième
        salle->chunks[i]=chunk_creer(xchunk,ychunk,nbPortes,type_chunk);
        free(type_chunk);
        type_chunk=NULL;
    }
    return salle;
}

/* fonction salle_creer
    paramètre: 
        type: chaine de caractères (cf ./generation/creation_generation_explication.txt)
    Retourne un pointeur sur salle_t si créer, NULL sinon 
    salle_creer_type sert a initialiser seulement les attributs la ou cette fonction initialise les méthodes
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
        printf("La creation de la salle a echoue\n");
        return NULL;
    }
}

extern void salle_afficher_ref(){
    printf("Nombre de ref de salle : %d\n", cpt_salle);
}