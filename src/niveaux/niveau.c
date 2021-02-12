#include <stdlib.h>
#include "../../lib/niveaux/niveau.h"
#include <string.h>

int cpt_niveau=0; 

static salle_t * chercher_salle(const niveau_t * niveau, int x, int y){
    int i;
    for(i=0; i<niveau->nbSalle; i++){
        if (niveau->salles[i]->position.x==x && niveau->salles[i]->position.y==y)
            return niveau->salles[i];
    }
    return NULL;
}

extern void relier_portes(FILE * fichier,niveau_t * niveau){
    char str[256];
    porte_t * porte1, *porte2;
    salle_t * salle;
    chunk_t * chunk;
    while(fgets(str,255,fichier)){
        salle=niveau->chercher_salle(niveau,str[0]-'0',str[1]-'0');
        chunk=salle->chercher_chunk(salle,str[2]-'0', str[3]-'0');
        porte1=chunk->chercher_porte(chunk,str[4]-'0');
        salle=niveau->chercher_salle(niveau,str[5]-'0',str[6]-'0');
        chunk=salle->chercher_chunk(salle,str[7]-'0', str[8]-'0');
        porte2=chunk->chercher_porte(chunk,str[9]-'0');

        porte1->porteDest=porte2;
        porte2->porteDest=porte1;
    }
}

static void niveau_lire(const niveau_t * niveau){
    int i;
    for(i=0;i<niveau->nbSalle;i++){
        printf("\nSalle %d:\n",i);
        niveau->salles[i]->lire(niveau->salles[i]);
    }
}

static err_t niveau_detruire(niveau_t ** niveau){
    int i;
    if(*niveau){
        for(i=0; i<(*niveau)->nbSalle; i++){
            (*niveau)->salles[i]->detruire(&((*niveau)->salles[i]));
            (*niveau)->salles[i]=NULL;
        }
        free((*niveau)->salles);
        (*niveau)->salles=NULL;
        free(*niveau);
        *niveau=NULL;
    }
    cpt_niveau--;
    return OK;
}

extern booleen_t niveau_existe(niveau_t * niveau){
    if(niveau)
        return VRAI;
    return FAUX;
}

extern niveau_t * niveau_creer(char * nom_fichier){
    int nbSalle,i;
    char type_salle[256],str[101];
    niveau_t * niveau=NULL;
    FILE * fichier=fopen(nom_fichier,"r");
    if(!fichier){
        printf("Aucun fichier de ce nom\n");
        return NULL;
    }
    fgets(str,100,fichier);
    nbSalle=atoi(str);
    niveau=malloc(sizeof(niveau_t));
    niveau->nbSalle=nbSalle;
    niveau->detruire=niveau_detruire;
    niveau->lire=niveau_lire;
    niveau->chercher_salle=chercher_salle;
    niveau->salles=malloc(sizeof(salle_t*)*nbSalle);  
    for(i=0;i<nbSalle;i++){
        fgets(type_salle,255,fichier);
        niveau->salles[i]=salle_creer(type_salle);
    }
    relier_portes(fichier,niveau);
    fclose(fichier);
    cpt_niveau++;
    return niveau;
}

extern void niveau_afficher_ref(){
    printf("Nombre de ref de niveau :%d\n",cpt_niveau);
}