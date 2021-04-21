#include "../../lib/niveaux/unite.h"
#include "stdlib.h"

/**
* \file unite.c
* \brief Module unité
* \verion 1.0
* \date Avril 2021
*/

/* Constantes */

int cpt_unite=0;

/* FONCTIONS: */

/**
    \brief Lecture du contenu d'une unité 
    \param unite_t Pointeur sur l'unité à lire
    \return Retourne le contenu de l'unité
*/
static int unite_lire (const unite_t * unite){
    return unite->contenu;
}

/**
    \brief Edition d'une unité
    \param unite_t Pointeur sur pointeur de unite_t, entier contenu
    \param int Contenu à mettre dans l'unité
    \return Retourne OK si tout s'est bien passé 
*/
static err_t unite_ecrire(unite_t ** unite, int contenu){
    if (*unite){
        (*unite)->contenu=contenu;
        return OK;
    }
}

/**
    \brief Destruction d'une unité
    \param unite_t Pointeur sur pointeur sur l'unite a supprimer
    \return Retourne OK si tout s'est bien passé
*/
static err_t unite_detruire(unite_t ** unite){
    if(*unite){
        free(*unite);
        *unite=NULL;
        cpt_unite--;
        return (OK);
    }
}

/**
    \brief Verification d'existence d'une unité
    \param unite_t Pointeur sur l'unite a tester
    \return Retourne VRAI si l'unite existe, FAUX sinon
*/
extern booleen_t unite_existe(unite_t * unite){
    if (unite == NULL)
        return FAUX;
    return VRAI;
}

/**
    \brief Création d'une unité
    \param int Contenu de l'unite
    \return Retourne l'unité si elle est crée, NULL sinon
*/
extern unite_t * unite_creer(int contenu){
    unite_t * unite= malloc(sizeof(unite_t));
    if (!unite){
        return NULL;
    }
    unite->detruire=unite_detruire;
    unite->lire=unite_lire;
    unite->ecrire=unite_ecrire;
    unite->contenu=contenu;
    cpt_unite++;

    return unite;
}

/**
    \brief Affiche la référence de l'unité par printf
*/
extern void unite_afficher_ref(){
    printf("Nombre ref d'unite : %d\n", cpt_unite);
}