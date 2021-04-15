/* Par Matthis */

typedef struct entite_s entite_t;

#ifndef _ENTITE_H_
#define _ENTITE_H_


#include "../commun.h"
#include "../../SDL/include/SDL.h"
#include "../niveaux/salle.h"

/* Constantes */

#define GRAVITE CHUNKH*7   //Valeur de la gravité (changera surement encore quelques fois)

#define DECEL 500	//Décélération des éntités lorsqu'elles sont en l'air 

#define COEFF_DECEL_SOL 5

#define IMMO 0
#define NEUTRE 1
#define POS_MOUV1 2
#define POS_MOUV2 3
#define SAUT 4
#define TOMBE 5
#define ATTAQUE1 6
#define APT 7

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
                        float secSprite,
                        int w, int h, 
                        int w_hitbox, int h_hitbox,int offset_hitbox,
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
int est_mur(int contenu);


extern int est_pont(int contenu);

/*
    str_creer_copier
    paramètre:
        chaine de caractères, chaine source a copier
    retourne un pointeur sur char différent de chaine_src mais avec la même chaine de caractères, NULL si ça s'est mal passé 
*/
extern 
char * str_creer_copier( char * chaine_src);

extern 
SDL_Texture ** creer_tableau_textures_chaine(SDL_Renderer *ren, int *n,char * chaine, char * appel);

extern 
SDL_Texture ** creer_tableau_textures_manuel(SDL_Renderer * ren, int *n,...);

extern 
long seek_entity_type(FILE * index,char *type);

extern 
char * entre_guillemet(FILE * file);


extern 
entite_t * creer_entite_chaine(SDL_Renderer *ren, const entite_t const * joueur , char * chaine,  FILE * index, char * appel);

extern 
void initTabDaffich(void ** tab);

extern err_t afficher_tableau (void * tab_ent[NB_MAX_AFF], SDL_Renderer * ren, int WINW, int WINH);

extern 
int ajouter_tableaux( void * tab[NB_MAX_AFF], void (*tab_destr[NB_MAX_AFF])(void ** ), void * ptr, void (*fonction_dest)(void **));

extern 
void enlever_tableaux(void * tab[NB_MAX_AFF] , void (*tab_destr[NB_MAX_AFF])(void ** ));

extern 
void vider_tableaux(void * tab[NB_MAX_AFF] , void (*tab_destr[NB_MAX_AFF])(void ** ));

extern 
void synchro_tableau(void * tab[NB_MAX_AFF], void (*tab_destr[NB_MAX_AFF])(void ** ),double temps);

extern 
err_t entite_detruire(entite_t ** ent);

extern
pos_t pont_en_bas(entite_t * ent );

extern
pos_t mur_en_bas(entite_t * ent);

extern
pos_t mur_en_haut(entite_t * ent);

extern
pos_t mur_a_droite(entite_t * ent);

extern
pos_t mur_a_gauche(entite_t * ent);

extern
void replacer(entite_t * ent, pos_t pos_mur, int direction);

extern 
void hitbox_tableau(SDL_Renderer * ren, void * tab[NB_MAX_AFF], int WINW, int WINH);

#endif