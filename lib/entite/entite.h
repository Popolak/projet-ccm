#ifndef _ENTITE_H_
#define _ENTITE_H_


#include "../commun.h"
#include "../../SDL/include/SDL.h"
#include "../niveaux/salle.h"

/* Constantes */

#define GRAVITE CHUNKH*7   //Valeur de la gravité (changera surement encore quelques fois)

#define DECEL 500	//Décélération des éntités lorsqu'elles sont en l'air 

#define COEFF_DECEL_SOL 5

enum {NEUTRE,POS_MOUV1,POS_MOUV2,SAUT,TOMBE,ATTAQUE1,APT};

/* Structures */

typedef struct entite_s{
    err_t (*detruire)(struct entite_s ** );
    #include "attribut_entite.h"
}entite_t;


/* Fonctions */

extern 
entite_t * entite_creer(char * nom, 
                        char *description,
                        salle_t * salle,
                        chunk_t * chunk,
                        pos_t pos,
                        float vitesse_x, float vitesse_y, float vitesse_max_y,
                        int w, int h, 
                        int w_hitbox, int h_hitbox, 
                        int nbTextures,
                        SDL_Texture ** textures);


extern 
booleen_t entite_existe(entite_t * ent);


/*
    est_obstacle:
    paramètres:
        int contenu, contenu d'une unité d'un chunk
        int dir, direction d'une entité
    retourne 1 si le contenu est un obstacle a une entité compte tenu de la direction de l'entité 
*/
extern 
int est_obstacle(int contenu,  int dir);


/*
    str_creer_copier
    paramètre:
        chaine de caractères, chaine source a copier
    retourne un pointeur sur char différent de chaine_src mais avec la même chaine de caractères, NULL si ça s'est mal passé 
*/
extern 
char * str_creer_copier( char * chaine_src);


#endif