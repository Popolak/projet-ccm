#include "attribut_entite.h"
void (*prendre_coup)(struct perso_s * , int );
void (*animer)(struct perso_s * const);
void (*update_speed) (struct perso_s * perso, int touche);
float vit_attack, vitesse_saut;
int degats;
int vie;