#include "../../lib/niveaux/unite.h"
#include "stdlib.h"

/* Constantes */

int cpt_unite=0;

/* FONCTIONS: */

/*  fonctions: unite_lire 
    paramètres pointeur sur unite_t
    retourne le contenu de l'unité
*/

static int unite_lire (const unite_t * unite){
    return unite->contenu;
}

/*  fonction: unite_ecrire
    paramètres: pointeur sur pointeur de unite_t, entier contenu
    retourne OK si tout s'est bien passé 

*/

static err_t unite_ecrire(unite_t ** unite, int contenu){
    if (*unite){
        (*unite)->contenu=contenu;
        return OK;
    }
}

/*  fonction: unite_detruire
    paramètre: pointeur sur pointeur sur unite_t
    retourne OK si tout s'est bien passé
*/


static err_t unite_detruire(unite_t ** unite){
    if(*unite){
        free(*unite);
        *unite=NULL;
        cpt_unite--;
        return (OK);
    }
}

/*  fonction: unite_existe
    paramètre: pointeur sur unite_t
    retourne VRAI si l'unite existe, FAUX sinon
*/

extern booleen_t unite_existe(unite_t * unite){
    if (unite == NULL)
        return FAUX;
    return VRAI;
}

/*  fonction! unite_creer
    paramètres: contenu de l'unite
    retourne l'unité si elle est crée, NULL sinon
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

/*  fonction : unite_afficher_ref
    pas de paramètre
*/

extern void unite_afficher_ref(){
    printf("Nombre ref d'unite : %d\n", cpt_unite);
}