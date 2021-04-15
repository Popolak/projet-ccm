/* Par Matthis */
void (*lire)(struct entite_s * );
err_t (*afficher_chunk)(SDL_Renderer *,struct entite_s *  ,int ,int );
err_t (*afficher_fenetre)(SDL_Renderer * ,struct entite_s * , int , int , int , int , SDL_Texture * );
void (*hitbox)(SDL_Renderer * , struct entite_s * , int , int );
int (*deplacer)(struct entite_s * ,double );
booleen_t (*en_l_air)(struct entite_s * );
booleen_t (*contact)(struct entite_s * , struct entite_s * );
booleen_t (*contact_porte)(struct entite_s * );
err_t (*detruire_textures)(struct entite_s **);


chunk_t * chunk;
salle_t * salle;
char * nom, *description;
pos_t pos;
int dir,w,h,w_hitbox,h_hitbox, nbTextures;
booleen_t gravite;
SDL_Texture ** textures;
float vitesse_x,vitesse_y;
float vitesse_max_y;
float secSprite, lastSprite;
int offset_hitbox;