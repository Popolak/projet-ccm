#ifndef _AFFICH_H_
#define _AFFICH_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "../commun.h"

/* Par Matthis */

#include "../niveaux/salle.h"


#define H_PONT_AFFICH 20
/* Structures*/
typedef struct camera_s{
    pos_t centre;
    int semiw,semih;            //Pas utile pour l'instant mais peut etre plus tard
    salle_t * salle;
    chunk_t * chunk;
}camera_t;

/* Fonctions */


/*
    creer_texture_image:
    paramètres :
        SDL_Renderer * ren le pointeur sur renderer
        char * nom_image, le nom du fichier de l'image a utiliser
    retourne :
        pointeur sur SDL_Texture, la texture créée a partir de l'image en paramètres
    rend le code plus clair
*/

extern SDL_Texture * creer_texture_image( SDL_Renderer * ren, char * nom_image);

/*
    render_mur_chunk
    paramètres :
        SDL_Renderer * ren le pointeur sur renderer
        SDL_Texture * texture_mur, la texture du mur créer au préalable
        camera_t * cam, pointeur sur la camera pour savoir quel chunk afficher
        int WINW et int WINH hight et width de la fenetre
    retourne:
        OK Si tout s'est bien passé
    render tous les murs d'un chunk 
*/
extern 
err_t render_mur_chunk(SDL_Renderer * ren, SDL_Texture * texture_mur,
                       salle_t *salle,chunk_t * chunk, int WINW, int WINH);

extern 
err_t render_pont_chunk(SDL_Renderer * ren, SDL_Texture * texture_pont,
                        salle_t * salle,chunk_t * chunk, int WINW, int WINH);

extern
err_t render_chunk_unite(void * tab[NB_MAX_AFF], 
                         attaque_t * tab_attaque[NB_MAX_ATT], 
                         SDL_Renderer * ren,
                         SDL_Texture * texture_pont,
                         SDL_Texture * texture_mur,
                         salle_t *salle,
                         chunk_t * chunk,
                         int WINW, 
                         int WINH);

extern
err_t render_background(SDL_Renderer * ren,salle_t *salle, chunk_t * chunk,SDL_Texture * texture_sol, SDL_Texture * texture_air ,int  WINW,int WINH);

#endif