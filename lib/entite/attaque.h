/* Par Matthis */

typedef struct attaque_s attaque_t;

#ifndef _ATTAQUE_H_
#define _ATTAQUE_H_
#include "entite.h"
#include "personnage.h"

typedef struct attaque_s{
    err_t (*detruire_ent)(struct entite_s ** );
    #include "attribut_attaque.h"
    err_t (*detruire)(struct attaque_s ** );
}attaque_t;


extern 
entite_t * attaque_creer(char * nom, 
                        char *description,
                        salle_t * salle,
                        chunk_t * chunk,
                        pos_t pos,
                        float vitesse_x, float vitesse_y, float vitesse_max_y,
                        float secSprite,
                        int w, int h, 
                        int w_hitbox, int h_hitbox, int offset_hitbox,
                        booleen_t gravite,
                        int degats,float duree_vie, 
                        entite_t * entite_liee,
                        int nbTextures,
                        SDL_Texture ** textures);

extern 
void synchro_attaque(attaque_t * tab[NB_MAX_ATT],double temps);

extern
void enlever_attaque_tableau(attaque_t * tab[NB_MAX_ATT],void *element );

extern 
void vider_attaque(attaque_t * tab[NB_MAX_ATT] );

extern 
void enlever_attaque(attaque_t * tab[NB_MAX_ATT] );

extern 
int ajouter_attaque( attaque_t * tab[NB_MAX_ATT], attaque_t* ptr);


extern err_t afficher_attaques (attaque_t * tab_ent[NB_MAX_AFF], SDL_Renderer * ren, int WINW, int WINH);

extern
void contact_attaque_ennemis(SDL_Renderer* ren, 
                             attaque_t * tab_att[NB_MAX_ATT], 
                             void * tab[NB_MAX_AFF], 
                             err_t (*tab_destr[NB_MAX_AFF])(void ** ), 
                             FILE * index, char * appel);

                


#endif