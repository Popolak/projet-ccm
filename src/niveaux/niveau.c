#include <stdlib.h>
#include "../../lib/niveaux/niveau.h"
#include <string.h>

/* CONSTANTES */

int cpt_niveau=0; 

/* FONCTIONS */

/* fonction chercher_salle
    paramètres:
        niveau: pointeur sur niveau
        x et y : position de la salle a rechercher dans le niveau
    retourne un pointeur sur salle_t si la salle est trouvée, NULL sinon 
*/

static salle_t * chercher_salle(const niveau_t * niveau, int x, int y){
    int i;
    for(i=0; i<niveau->nbSalle; i++){
        if (niveau->salles[i]->position.x==x && niveau->salles[i]->position.y==y)
            return niveau->salles[i];
    }
    return NULL;
}

/*  fonction relier_porte
    paramètres:
        fichier: pointeur sur FILE 
        niveau: pointeur sur niveau
*/

extern void relier_portes(FILE * fichier,niveau_t * niveau){    //Pour le contenu du fichier cf ./generation/creation_generation_explication.txt
    char str[256];
    porte_t * porte1, *porte2;
    salle_t * salle;
    chunk_t * chunk;
    while(fgets(str,255,fichier)){                                          //Pour chaque ligne contenant deux portes
        salle=niveau->chercher_salle(niveau,str[0]-'0',str[1]-'0');         /// 
        chunk=salle->chercher_chunk(salle,str[2]-'0', str[3]-'0');          ///On cherche la première porte
        porte1=chunk->chercher_porte(chunk,str[4]-'0');                     ///
        salle=niveau->chercher_salle(niveau,str[5]-'0',str[6]-'0');      //
        chunk=salle->chercher_chunk(salle,str[7]-'0', str[8]-'0');       //On cherche la deuxième porte
        porte2=chunk->chercher_porte(chunk,str[9]-'0');                  //

        porte1->porteDest=porte2;                               //Et on relie ces deux portes
        porte2->porteDest=porte1;                               //
    }
}

/* fonction niveau_lire
    paramètre:
        niveau: pointeur sur niveau
*/

static void niveau_lire(const niveau_t * niveau){
    int i;
    for(i=0;i<niveau->nbSalle;i++){                 //On appelle la fonction d'affichage de la salle pour chaque salle
        printf("\nSalle %d:\n",i);
        niveau->salles[i]->lire(niveau->salles[i]);
    }
}

/*  fonction niveau_detruire
    paramètre:
        niveau: pointeur sur pointeur sur niveau_t
    retourne OK si tout s'est bien passé
*/

static err_t niveau_detruire(niveau_t ** niveau){
    int i;
    if(*niveau){
        for(i=0; i<(*niveau)->nbSalle; i++){                            //Pour chaque salle
            (*niveau)->salles[i]->detruire(&((*niveau)->salles[i]));    //on détruit la salle
            (*niveau)->salles[i]=NULL;
        }
        free((*niveau)->salles);                                        //On détruit le tableau de salle
        (*niveau)->salles=NULL;
        free(*niveau);                                                  //On détruit le niveau
        *niveau=NULL;
    }
    cpt_niveau--;
    return OK;
}

/* fonction niveau_existe
    paramètre:
        niveau: pointeur sur niveau_t
    retourne VRAI si niveau existe, FAUX sinon
*/

extern booleen_t niveau_existe(niveau_t * niveau){
    if(niveau)
        return VRAI;
    return FAUX;
}

/* fonction niveau_creer
    paramètre: 
        nom_fichier: chaine de caractères contenant le nom du fichier de génération du niveau
    retourne un pointeur sur niveau_t si tout fonctionne bien, NULL sinon 
*/

extern niveau_t * niveau_creer(char * nom_fichier){
    int nbSalle,i;                              //nombre de salle dans le niveau et irérateur
    char type_salle[256],str[101];              //Variable pour récupérer les lignes du fichier
    niveau_t * niveau=NULL;                     
    FILE * fichier=fopen(nom_fichier,"r");      //On ouvre le fichier de génération pour le lire
    if(!fichier){                            
        printf("Aucun fichier de ce nom\n");
        return NULL;
    }
    fgets(str,100,fichier);                             //Pour le contenu du fichier cf ./generation/creation_generation_explication.txt
    nbSalle=atoi(str);                                  //On récupère le nombre de salle pour allouer plus tard
    niveau=malloc(sizeof(niveau_t)); 
    if(!niveau){
        printf("L'allocation du niveau a échouée\n");
        return NULL;
    }
    niveau->nbSalle=nbSalle;    
    niveau->detruire=niveau_detruire;                   //
    niveau->lire=niveau_lire;                           // initialisation des méthodes
    niveau->chercher_salle=chercher_salle;              //
    niveau->salles=malloc(sizeof(salle_t*)*nbSalle);  
    for(i=0;i<nbSalle;i++){                             //On créer nbSalle salle dans le niveau
        fgets(type_salle,255,fichier);
        niveau->salles[i]=salle_creer(type_salle);
    }
    relier_portes(fichier,niveau);                      //Puis on relie les portes entre elles
    fclose(fichier);
    cpt_niveau++;
    return niveau;
}

extern void niveau_afficher_ref(){
    printf("Nombre de ref de niveau :%d\n",cpt_niveau);
}