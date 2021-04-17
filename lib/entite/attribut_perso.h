/* Par Matthis */
#include "attribut_entite.h" 
                          //perso_t hérite des attribut et méthodes d'entite_t (sauf destruction)

booleen_t (*envie)(struct perso_s * personnage);
void (*prendre_coup)(struct perso_s * , int );
int (*update_speed) (struct perso_s * ,  int ,double);
err_t (*depop)(struct perso_s * );
err_t (*copie_partiel)(struct perso_s * , struct perso_s * );
err_t (*change_chunk)(SDL_Renderer * , struct perso_s * ,  void **, err_t (**)(void ** ),attaque_t **, FILE *, FILE * , char *  );
err_t (*change_salle)(struct perso_s * );
int (*nouvelle_attaque)(SDL_Renderer *,attaque_t **, struct perso_s * , FILE * , char * );

float vit_attack, vitesse_saut, temps_att;
int degats;
int vie;
float temps_inv;
char * nom_attaque;