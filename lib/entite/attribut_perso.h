/* Par Matthis */
#include "attribut_entite.h" 
                          //perso_t hérite des attribut et méthodes d'entite_t (sauf destruction)

booleen_t (*envie)(struct perso_s * personnage);
void (*prendre_coup)(struct perso_s * , int );
void (*update_speed) (struct perso_s * ,  int ); // temporaire
err_t (*depop)(struct perso_s * );
err_t (*copie_partiel)(struct perso_s * , struct perso_s * );
err_t (*change_chunk)(SDL_Renderer * , struct perso_s * ,  void **, void (**)(void ** ), FILE *, FILE * , char *  );

float vit_attack, vitesse_saut;
int degats;
int vie;