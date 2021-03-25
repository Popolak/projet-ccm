#include "attribut_entite.h"                            //perso_t hérite des attribut et méthodes d'entite_t (sauf destruction)

void (*prendre_coup)(struct perso_s * , int );
void (*update_speed) (struct perso_s * perso,  int tot_touche); // temporaire
err_t (*depop)(struct perso_s * perso);
err_t (*copie_partiel)(struct perso_s * persoDst, struct perso_s * persoSrc);

float vit_attack, vitesse_saut;
int degats;
int vie;