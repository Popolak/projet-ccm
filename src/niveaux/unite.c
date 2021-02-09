#include "unite.h"
#include "stdlib.h"

int cpt_unite=0;

static int unite_lire (const unite_t * unite){
    return unite->contenu;
}

static err_t unite_ecrire(unite_t ** unite, int contenu){
    if (*unite){
        (*unite)->contenu=contenu;
        return OK;
    }
}


static err_t unite_detruire(unite_t ** unite){
    if(*unite){
        free(*unite);
        *unite=NULL;
        cpt_unite--;
        return (OK);
    }
}

extern booleen_t unite_existe(unite_t * unite){
    if (unite == NULL)
        return FAUX;
    return VRAI;
}

extern unite_t * unite_creer(int contenu, int x, int y){
    unite_t * unite= malloc(sizeof(unite_t));
    if (!unite){
        return NULL;
    }
    unite->detruire=unite_detruire;
    unite->lire=unite_lire;
    unite->ecrire=unite_ecrire;
    unite->contenu=contenu;
    unite->x=x;
    unite->y=y;
    cpt_unite++;

    return unite;
}

extern void unite_afficher_ref(){
    printf("Nombre ref : %d", cpt_unite);
}