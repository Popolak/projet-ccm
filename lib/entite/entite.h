#ifndef _ENTITE_H_
#define _ENTITE_H_


#include "../commun.h"
#include "../../SDL/include/SDL.h"
#include "../niveaux/salle.h"

/* Constantes */

#define GRAVITE CHUNKH*7   //Valeur de la gravité (changera surement encore quelques fois)

#define DECEL 500	//Décélération des éntités lorsqu'elles sont en l'air 

#define COEFF_DECEL_SOL 5

/* Structures */

typedef struct entite_s{
    err_t (*detruire)(struct entite_s ** );
    #include "attribut_entite.h"
}entite_t;


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

extern 
int est_obstacle(int contenu,  int dir);

extern 
char * str_creer_copier( char * chaine_src);

extern 
booleen_t appartient_a_dir(int dir, int recherche);

extern
pos_t pos_proche_mur(entite_t * ent, pos_t pos_mur);

#endif