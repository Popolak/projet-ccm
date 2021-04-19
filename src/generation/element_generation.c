/* Par Matthis */

#include "../../lib/generation/element_generation.h"

extern 
void * creer_entite_chaine(SDL_Renderer *ren, int *n,const perso_t const * joueur, char * chaine,  FILE * index, char * appel){
    void * element;
    char type[40], *nom=NULL, *desc=NULL, * nom_attaque=NULL;
    int  w,h,w_hit,h_hit,offset, nbText, liee;
    float v_y,secSprite, duree_vie, v_i_x,v_i_y;

    float vit_attack, vitesse_saut;
    int degats, range;
    int vie;

    //ennemis
    int defense;
    int attaque;
    int portee;


    entite_t *entite_liee=NULL;

    pos_t pos;
    SDL_Texture ** textures;
    char str[700];
    if (!index){
        printf("Le fichier n'existe pas\n");
        return NULL;
    }
    fseek(index,0,SEEK_SET);
    if(feof(index)){
        return NULL;
    }
    sscanf(chaine, "%s %f %f %f %f",type, &(pos.x),&(pos.y),&v_i_x,&v_i_y);
    if(pos.x == -1 || pos.y == -1){
        if(joueur){
            pos.x=joueur->pos.x;
            pos.y=joueur->pos.y;
        }
        else{
            pos.x=CHUNKH/2;
            pos.y=CHUNKW/2;
        }
    }
    if(seek_entity_type(index,type) == -1){
        return NULL;
    }
    fscanf(index,"%d",n);
    nom=entre_guillemet(index);
    desc=entre_guillemet(index);
    fscanf(index,"%d",&w);
    fscanf(index,"%d",&h);
    fscanf(index,"%d",&w_hit);
    fscanf(index,"%d",&h_hit);
    fscanf(index,"%d",&offset);
    fscanf(index,"%f",&v_y);
    fscanf(index,"%f",&secSprite);
    if(*n==1 || *n==2){
        fscanf(index,"%d",&degats);
        fscanf(index,"%d", &vie);
        fscanf(index,"%f", &vit_attack);
        nom_attaque= entre_guillemet(index);
        fscanf(index,"%f", &vitesse_saut);
    }
    if(*n==2){
        fscanf(index,"%d", &range);
    }

    if(*n==3){
        fscanf(index,"%d",&degats);
        fscanf(index,"%f",&duree_vie);
        fscanf(index,"%d",&liee);
        v_y=v_y* (joueur->dir==DROITE ? 1 : -1);
        if(liee==1){
            entite_liee=(entite_t*)joueur;
            duree_vie= ((perso_t*)(joueur))->vit_attack/2;
        }
        else{
            ((perso_t*)joueur)->temps_att= ((perso_t*)(joueur))->vit_attack/4;
        }
    }

    fgetc(index);
    fgets(str,699,index);
    textures=creer_tableau_textures_chaine(ren,&nbText,str,appel);
    if(*n==1){
        element= perso_creer(nom,desc,vie,joueur->salle, joueur->chunk, pos,v_i_x,v_i_y,v_y,vitesse_saut,w,h,w_hit,h_hit,offset,secSprite,vit_attack,degats,nom_attaque,nbText,textures);
    }
    else if(*n==2){
        element= ennemi_creer(nom,desc,vie,joueur->salle, joueur->chunk, pos,v_i_x,v_i_y,v_y,vitesse_saut,w,h,w_hit,h_hit,offset,secSprite,vit_attack,degats,nom_attaque,range,nbText,textures);
    }
    else if(*n==3){
        element = attaque_creer(nom,desc,joueur->salle,joueur->chunk,pos,v_i_x,v_y,v_y,secSprite,w,h,w_hit,h_hit,offset,0,degats,duree_vie,entite_liee,nbText,textures);
    }
    else {
        element=entite_creer(nom,desc,joueur->salle, joueur->chunk, pos,v_i_x,v_i_y,v_y,secSprite,w,h,w_hit,h_hit,offset,nbText,textures);
    }
    free(nom);
    free(desc);
    free(nom_attaque);
    return element;
    
}