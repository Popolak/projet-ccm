#ifndef _ENTITE_H_
#define _ENTITE_H_


#include "../commun.h"
#include "SDL2/SDL.h"
#include "../niveaux/salle.h"

typedef struct entite_s{
    #include "attribut_entite.h"
}entite_t;


extern 
entite_t * entite_creer(char * nom, 
                        char *description,
                        pos_t pos,
                        int w, int h, 
                        int w_hitbox, int h_hitbox, 
                        int nbTextures,
                        SDL_Texture ** textures,
                        chunk_t * chunk,
                        salle_t * salle,
                        int vitesse_x, int vitesse_y);


extern 
booleen_t entite_existe(entite_t * ent);


#endif