#include "../../lib/generation/element_generation.h"

extern 
void * creer_entite_chaine(SDL_Renderer *ren, int *n,const perso_t const * joueur , char * chaine,  FILE * index, char * appel){
    void * element;
    char type[20], *nom=NULL, *desc=NULL;
    int  w,h,w_hit,h_hit,offset, nbText;
    float v_y,secSprite;

    float vit_attack, vitesse_saut;
    int degats;
    int vie;

    pos_t pos;
    SDL_Texture ** textures;
    char str[300];
    if (!index){
        printf("Le fichier n'existe pas\n");
        return NULL;
    }
    fseek(index,0,SEEK_SET);
    if(feof(index)){
        return NULL;
    }
    sscanf(chaine, "%s %f %f",type, &(pos.x),&(pos.y));


    seek_entity_type(index,type);
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

    if(n==1){
        fscanf(index,"%d",&degats);
        fscanf(index,"%d", &vie);
        fscanf(index,"%f", &vit_attack);
        fscanf(index,"%f", &vitesse_saut);
    }

    fgetc(index);
    fgets(str,299,index);
    printf("%s\n", str);
    textures=creer_tableau_textures_chaine(ren,&nbText,str,appel);
    
    if(*n==1){
        element= perso_creer(nom,desc,vie,joueur->salle, joueur->chunk, pos,0,0,v_y,vitesse_saut,w,h,w_hit,h_hit,offset,secSprite,vit_attack,degats,nbText,textures);
    }
    else {
        element=entite_creer(nom,desc,joueur->salle, joueur->chunk, pos,0,0,v_y,secSprite,w,h,w_hit,h_hit,offset,nbText,textures);
    }
    free(nom);
    free(desc);
    return element;
    
}