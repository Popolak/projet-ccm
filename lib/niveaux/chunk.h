/* Par Matthis */

typedef struct chunk_s chunk_t;

#ifndef _CHUNK_H_
#define _CHUNK_H_


#include "./unite.h"
#include "./porte.h"
#include "../entite/personnage.h"

#define CHUNKH 360   //Hauteur et épaisseur des chunks en unité (ratio ~16/9)
#define CHUNKW 640
#define TAILLE_MUR 20
#define TAILLE_PORTE 80
#define W_PONT 130
#define H_PONT 2

enum {RIEN,HAUT,DROITE,BAS=4,GAUCHE=8,COIN_NE=16,COIN_SE=32,COIN_SO=64,COIN_NO=128};

typedef struct chunk_s{
    void (*lire)(const struct chunk_s * );
    err_t (*detruire)(struct chunk_s ** );                                  //méthodes
    void (*lire_partiel)(const struct chunk_s * ,int ,int ,int ,int );
    porte_t * (*chercher_porte)(const struct chunk_s *, int);
    err_t (*remplir)(struct chunk_s * , int );
    err_t (*remplir_surface)(struct chunk_s *, int , int  , int  ,int , int );
    
    pos_t position;     //Position du chunk dans la salle
    unite_t *** chunk;  //matrice d'unités
    porte_t ** portes;  //Tableau de portes
    int nb_portes;
} chunk_t;

extern pos_t prochain_mur (int x, int y, chunk_t * chunk);
extern pos_t prochain_pont (int x, int y, chunk_t * chunk);

extern
void attribut_mur(int x, int y, int* w, int* h,chunk_t * chunk);
extern void attribut_pont(int x, int y, int* w, chunk_t * chunk);

extern booleen_t chunk_existe(chunk_t * chunk);
extern chunk_t * chunk_creer(salle_t *salle,int x,int y, int nb_portes, char * type);
extern void chunk_afficher_ref();

extern booleen_t coord_correcte(int x, int y);


extern 
err_t creer_pont_chaine(chunk_t * chunk,char * type);

#endif