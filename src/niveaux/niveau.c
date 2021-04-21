#include <stdlib.h>
#include <string.h>
#include "../../lib/niveaux/niveau.h"
/**
* \file niveau.c
* \brief Module niveau
* \verion 1.0
* \date Avril 2021
*/
/* CONSTANTES */

int cpt_niveau=0; 

/* FONCTIONS */

/**
    \brief Recherche d'une salle
    \param niveau_t Pointeur sur le niveau
    \param int Position horizontale de la salle dans le niveau
    \param int Position verticale de la salle dans le niveau
    \return Retourne un pointeur sur salle_t si la salle est trouvée, NULL sinon
*/
static salle_t * chercher_salle(const niveau_t * niveau, int x, int y){
    int i;
    for(i=0; i<niveau->nbSalle; i++){
        if (niveau->salles[i]->position.x==x && niveau->salles[i]->position.y==y)
            return niveau->salles[i];
    }
    return NULL;
}

/**
    \brief Relier les portesdu niveau entres elles
    \param FILE Pointeur sur FILE 
    \param niveau_t Niveau où relier les portes
    \return Retourne OK si l'opération réussi, sinon un code erreur.
*/
extern err_t relier_portes(FILE * fichier,niveau_t * niveau){    //Pour le contenu du fichier cf ./generation/generation_explication.txt
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

        if(!porte1 || !porte2){
            printf("L'une des portes n'existe pas: %s\n", str);
            niveau->detruire(&niveau);
            return ERR_DEB_MEMOIRE;
        }
        porte1->porteDest=porte2;                               //Et on relie ces deux portes
        porte2->porteDest=porte1;                               //
    }
    return OK;
}

/** 
    \brief Affichage de toutes les salles du niveau
    \param niveau_t Pointeur sur le niveau à afficher
*/

static void niveau_lire(const niveau_t * niveau){
    int i;
    for(i=0;i<niveau->nbSalle;i++){                 //On appelle la fonction d'affichage de la salle pour chaque salle
        niveau->salles[i]->lire(niveau->salles[i]);
    }
}

/** 
    \brief Destruction de toutes les salles du niveau
    \param niveau_t Pointeur sur le niveau à afficher
    \return Retourne OK si la destruction réussi
*/
static err_t niveau_detruire(niveau_t ** niveau){
    int i;
    if(*niveau){
        if((*niveau)->salles){
            for(i=0; i<(*niveau)->nbSalle; i++){                            //Pour chaque salle
                if((*niveau)->salles[i]){
                    (*niveau)->salles[i]->detruire(&((*niveau)->salles[i]));    //on détruit la salle
                    (*niveau)->salles[i]=NULL;
                }
            }
            free((*niveau)->salles);                                        //On détruit le tableau de salle
            (*niveau)->salles=NULL;
        }
        free(*niveau);                                                      //On détruit le niveau
        *niveau=NULL;
    }
    cpt_niveau--;
    return OK;
}

/** 
    \brief Création des ponts d'un niveau
    \param FILE Fichier de création du niveau
    \param niveau_t Pointeur sur le niveau à afficher
*/
extern 
err_t creer_pont(FILE * fichier,niveau_t* niv){
    int sx,sy,cx,cy;
    char str[30];
    salle_t * salle;
    chunk_t * chunk;
    fseek(fichier,0,SEEK_SET);
    while(!feof(fichier)){
        salle=NULL;
        chunk=NULL;
        sx=fgetc(fichier)-'0';
        sy=fgetc(fichier)-'0';
        cx=fgetc(fichier)-'0';
        cy=fgetc(fichier)-'0';
        salle= niv->chercher_salle(niv,sx,sy);
        if(salle){
            chunk=salle->chercher_chunk(salle,cx,cy);
        }
        if(chunk){
            fgets(str,29,fichier);
            creer_pont_chaine(chunk,str);
        }
    }
}

/**
    \brief Vérification d'existence du niveau passé en paramètre
    \param niveau_t Pointeur sur le niveau à tester
    \return Retourne VRAI si niveau existe, FAUX sinon
*/
extern booleen_t niveau_existe(niveau_t * niveau){
    if(niveau)
        return VRAI;
    return FAUX;
}

/**
    \brief Création du niveau
    \param char Nom_fichier: chaine de caractères contenant le nom du fichier de génération du niveau
    \return Retourne un pointeur sur niveau_t si tout fonctionne bien, NULL sinon 
*/
extern niveau_t * niveau_creer(char * nom_fichier){
    int nbSalle,i;                              //nombre de salle dans le niveau et irérateur
    char type_salle[256],str[256];              //Variable pour récupérer les lignes du fichier
    niveau_t * niveau=NULL;                     
    FILE * fichier=fopen(nom_fichier,"r");      //On ouvre le fichier de génération pour le lire
    if(!fichier){                            
        printf("Aucun fichier de ce nom\n");
        return NULL;
    }
    fgets(str,255,fichier);                             //Pour le contenu du fichier cf ./generation/generation_explication.txt
    nbSalle=atoi(str);                                  //On récupère le nombre de salle pour allouer plus tard
    niveau=malloc(sizeof(niveau_t)); 
    if(!niveau){
        fclose(fichier);
        printf("L'allocation du niveau a échouée\n");
        return NULL;
    }
    niveau->nbSalle=nbSalle;    
    niveau->detruire=niveau_detruire;                   //
    niveau->lire=niveau_lire;                           // initialisation des méthodes
    niveau->chercher_salle=chercher_salle;              //
    if(!(niveau->salles=malloc(sizeof(salle_t*)*nbSalle))){
        fclose(fichier);
        printf("L'allocation du tableau de salle a échouée\n");
        niveau->detruire(&niveau);
        return NULL;
    }  
    for(i=0;i<nbSalle;i++){                             //On créer nbSalle salle dans le niveau
        fgets(type_salle,255,fichier);
        if(!(niveau->salles[i]=salle_creer(type_salle))){
            fclose(fichier);
            printf("La création de la salle %d a échouée \n", i);
            niveau->detruire(&niveau);
            return NULL;
        }
    }  
    if(!feof(fichier)){
        if(relier_portes(fichier,niveau)!=OK){              //Puis on relie les portes entre elles
            fclose(fichier);
            printf("Il y a eu un problème pour lier les portes\n");
            niveau_detruire(&niveau);
            return NULL;
        }
    }               
    fclose(fichier);
    cpt_niveau++;
    return niveau;
}

/**
    \brief Affiche la référence du niveau par printf
*/
extern void niveau_afficher_ref(){
    printf("Nombre de ref de niveau :%d\n",cpt_niveau);
}