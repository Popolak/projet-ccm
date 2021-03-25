#ifndef _COMMUN_H_
#define _COMMUN_H_

#include <stdio.h>

/*Contenus possible d'une unité*/
enum {MUR=0,VIDE=1,PORTE=2,JOUEUR=4};

#define ratioSol 1.0*1.1/12.9

typedef struct position
{
	float x;
	float y;
}
pos_t;

/* Definition du type booleen */
typedef enum booleen_s { FAUX , VRAI } booleen_t ;

/* Definition du type erreur */
typedef int err_t ; 

/* Erreurs */
#define OK 0

#define ERR_DEB_MEMOIRE -100 

#define ERR_LISTE_IND_ELEM  -200 

#define CAM_TROP_GRANDE 1

#define PAS_DALLOC 2

#endif

