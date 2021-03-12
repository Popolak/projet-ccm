#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../../lib/entite/entite.h"


extern 
est_obstacle(int contenu){
    return (contenu==MUR);
}

static 
void entite_deplacement(entite_t * ent){
    
}

static 
int mur_a_gauche(entite_t * ent){
    int i,j;
    for(i=-ent->h/2; ent->pos.x + i <0;i++);
    for(;i < ent->h/2 && ent->pos.x+i<CHUNKH;i++){
        for(j=-ent->w/2; ent->pos.y + j <0;j++);
        for(;j>=0;j++){
            if(est_obstacle(ent->chunk->chunk[i][j]->contenu)){
                return GAUCHE;
            }
        }
    }
}

static 
int mur_a_droite(entite_t * ent){
    int i,j;
    for(i=-ent->h/2; ent->pos.x + i <0;i++);
    for(;i < ent->h/2 && ent->pos.x+i<CHUNKH;i++){
        for(j=ent->w/2; ent->pos.y + j >=CHUNKW;j--);
        for(;j>=0;j--){
            if(est_obstacle(ent->chunk->chunk[i][j]->contenu)){
                return DROITE;
            }
        }
    }
}

static 
int mur_en_haut(entite_t * ent){
    int i,j;
    for(j=-ent->w/2; ent->pos.y + j <0;j++);
    for(;j < ent->w/2 && ent->pos.y+j<CHUNKW;j++){
        for(i=-ent->h/2; ent->pos.x + i <0;i++);
        for(;i>=0;i++){
            if(est_obstacle(ent->chunk->chunk[i][j]->contenu)){
                return HAUT;
            }
        }
    }
}

static 
int mur_en_bas(entite_t * ent){
    int i,j;
    for(j=-ent->w/2; ent->pos.y + j <0;j++);
    for(;j < ent->w/2 && ent->pos.y+j<CHUNKW;j++){
        for(i=ent->h/2; ent->pos.x + i >=CHUNKH;i--);
        for(;i>=0;i--){
            if(est_obstacle(ent->chunk->chunk[i][j]->contenu)){
                return GAUCHE;
            }
        }
    }
}

static 
int en_contact_obstacle(entite_t * ent){
    int tot=RIEN;
    tot+=mur_a_gauche(ent);
    tot+=mur_a_droite(ent);
    tot+=mur_en_haut(ent);
    tot+=mur_en_bas(ent);
    return tot;
    
}

static
booleen_t en_contact(entite_t * ent_courante, entite_t * ent_a_verif){
    if (ent_courante->w_hitbox == 0 && ent_courante->w_hitbox == 0 )
        return FAUX;
    if(ent_courante->pos.y + ent_courante->w_hitbox/2 >= ent_a_verif->pos.y - ent_a_verif->w_hitbox/2)
        return VRAI;
    if(ent_courante->pos.y - ent_courante->w_hitbox/2 >= ent_a_verif->pos.y + ent_a_verif->w_hitbox/2)
        return VRAI;
    if(ent_courante->pos.x + ent_courante->h_hitbox/2 >= ent_a_verif->pos.x - ent_a_verif->h_hitbox/2)
        return VRAI;
    if(ent_courante->pos.x - ent_courante->h_hitbox/2 >= ent_a_verif->pos.x + ent_a_verif->h_hitbox/2)
        return VRAI;
    return FAUX;
}




extern 
err_t str_creer_copier(char * chaine_dest, char * chaine_src){
    chaine_dest=NULL;
    chaine_dest=malloc(sizeof(char)*strlen(chaine_src)+1);
    if(!chaine_dest){
        printf("L'allocation de l'entite n'a pas pu etre effectuee\n");
        return PAS_DALLOC;
    }
    strcpy(chaine_dest,chaine_src);
    chaine_dest[strlen(chaine_src)]='\0';
    return OK;
}

static 
void entite_lire(entite_t * ent){
    printf ("%s :\n%s", ent->nom, ent->description);
}

static 
err_t entite_detruire(entite_t ** ent){
    if(*ent){
        if((*ent)->nom){
            free((*ent)->nom);
            (*ent)->nom=NULL;
        }
        if((*ent)->description){
            free((*ent)->description);
            (*ent)->description=NULL;
        }
    }
    free(*ent);
    *ent=NULL;
}


extern 
booleen_t entite_existe(entite_t * ent){
    if (ent==NULL)
        return FAUX;
    return VRAI;
}

extern 
entite_t * entite_creer(char * nom, 
                        char *description,
                        pos_t pos,
                        int w, int h, 
                        int w_hitbox, int h_hitbox, 
                        int nbTextures,
                        SDL_Texture ** textures,
                        chunk_t * chunk,
                        salle_t * salle,
                        int vitesse_x,int vitesse_y)
{
    entite_t * entite=NULL;
    if (!(entite=malloc(sizeof(entite_t)))){
        printf("L'allocation de l'entite n'a pas pu etre effeectuee\n");
        return  NULL;
    }

    entite->en_vie=VRAI;
    entite->pos=pos;
    entite->w=w;
    entite->h=h;
    entite->w_hitbox=w_hitbox;
    entite->h_hitbox=h_hitbox;
    entite->nbTextures=nbTextures;
    entite->vitesse_x=vitesse_x;
    entite->vitesse_y=vitesse_y;
    entite->chunk=chunk;
    entite->salle=salle;
    entite->textures=textures;


    entite->detruire=entite_detruire;
    entite->lire=entite_lire;

    if(str_creer_copier(entite->nom,nom)==PAS_DALLOC){
        entite->detruire(&entite);
        return PAS_DALLOC;
    }
    if(str_creer_copier(entite->description, description)==PAS_DALLOC){
        entite->detruire(&entite);
        return PAS_DALLOC;
    }


}