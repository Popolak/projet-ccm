void (*lire)(struct entite_s * );
err_t (*afficher_chunk)(SDL_Renderer *ren,struct entite_s *,int ,int );
err_t (*afficher_fenetre)(SDL_Renderer * ,struct entite_s * , int , int , int , int , SDL_Texture * );
void (*deplacer)(struct entite_s * ent,double temps);
booleen_t (*en_l_air)(struct entite_s * ent);


chunk_t * chunk;
salle_t * salle;
char * nom, *description;
pos_t pos;
int dir,w,h,w_hitbox,h_hitbox, nbTextures;
SDL_Texture ** textures;
float vitesse_x,vitesse_y;
float vitesse_max_y;
booleen_t envie;