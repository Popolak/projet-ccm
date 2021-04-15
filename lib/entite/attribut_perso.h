/* Par Matthis */
#include "attribut_entite.h" 
                          //perso_t hérite des attribut et méthodes d'entite_t (sauf destruction)

booleen_t (*envie)(struct perso_s * personnage);
void (*update_speed) (struct perso_s * ,  int ); // temporaire
err_t (*depop)(struct perso_s * );
err_t (*copie_partiel)(struct perso_s * , struct perso_s * );

float vit_attack, vitesse_saut;
int degats;
int vie;