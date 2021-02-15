#ifndef _COMMUN_H_
#define _COMMUN_H_

#include <stdio.h>

#include "contenu.h"

typedef struct position
{
	int x;
	int y;
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

#endif

