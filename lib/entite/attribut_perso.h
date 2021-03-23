#include "attribut_entite.h"                            //perso_t hérite des attribut et méthodes d'entite_t (sauf destruction)

void (*prendre_coup)(struct perso_s * , int );
void (*animer)(struct perso_s * const);
void (*update_speed) (struct perso_s * perso,  int tot_touche, float temps); // temporaire

float vit_attack, vitesse_saut;
int degats;
int vie;