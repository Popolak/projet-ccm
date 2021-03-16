#ifndef _ENTITE_H_
#define _ENTITE_H_


#include "../commun.h"
#include "../../SDL/include/SDL.h"
#include "../niveaux/salle.h"

typedef struct entite_s{
    err_t (*detruire)(struct entite_s ** );
    void (*lire)(struct entite_s * );
    err_t (*afficher_chunk)(SDL_Renderer *ren,struct entite_s *,int ,int );
    err_t (*afficher_fenetre)(SDL_Renderer * ,struct entite_s * , int , int , int , int , SDL_Texture * );
    int (*contact_obstacle)(struct entite_s * );
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
                        float vitesse_x, float vitesse_y);


extern 
booleen_t entite_existe(entite_t * ent);

extern 
int est_obstacle(int contenu,  int dir);

extern 
char * str_creer_copier( char * chaine_src);

extern 
booleen_t appartient_a_dir(int dir, int recherche);

#endif