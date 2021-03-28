#include "attribut_entite.h" 

err_t (*detruire_perso)(struct perso_s ** ent);                           //perso_t hérite des attribut et méthodes d'entite_t (sauf destruction)

void (*prendre_coup)(struct perso_s * , int );
void (*update_speed) (struct perso_s * ,  int ); // temporaire
err_t (*depop)(struct perso_s * );
err_t (*copie_partiel)(struct perso_s * , struct perso_s * );

float vit_attack, vitesse_saut;
int degats;
int vie;