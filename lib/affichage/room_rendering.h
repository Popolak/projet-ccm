#ifndef _AFFICH_H_
#define _AFFICH_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "../commun.h"

#include "../niveaux/salle.h"

/* Structures*/
typedef struct camera_s{
    pos_t centre;
    int semiw,semih;            //Pas utile pour l'instant mais peut etre plus tard
    salle_t * salle;
    chunk_t * chunk;
}camera_t;

/* Fonctions */

extern SDL_Texture * creer_texture_image( SDL_Renderer * ren, char * nom_image);
extern err_t render_mur_chunk(SDL_Renderer * ren, SDL_Texture * texture_mur, camera_t * cam, int WINW, int WINH);




#endif