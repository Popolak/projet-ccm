#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../../lib/entite/entite.h"


extern 
int est_obstacle(int contenu,  int dir){
    return (contenu==MUR);
}

extern 
booleen_t appartient_a_dir(int dir, int recherche){
    int i=COIN_NO;
    for(;i>dir;i/=2);
    for(;i>0;i/=2){
        if(dir>=i){
            dir-=i;
            if(i==recherche)
                return VRAI;
        }
    }
    return FAUX;
}

static err_t afficher_dans_chunk(SDL_Renderer *ren,entite_t *entite,int WINH,int WINW){
    entite->afficher_fenetre(ren,entite,entite->w*WINW/CHUNKW,entite->h*WINW/CHUNKW,entite->pos.x*WINW/CHUNKW, entite->pos.y*WINW/CHUNKW, entite->textures[0]);
}

static err_t afficher_dans_fenetre(SDL_Renderer * ren,entite_t * entite, int w, int h, int x, int y, SDL_Texture * texture){
    float ratioTaille;
    SDL_Rect src,dst;
    if(!texture){
        printf("Pas de textures donnée pour l'entité : %s\n",entite->nom);
        return PAS_DALLOC;
    }
    src.x=0;
    src.y=0;
    dst.h=h;
    dst.w=w;
    dst.x=y-w/2;
    dst.y=x-h/2;
    SDL_QueryTexture(texture,NULL,NULL,&(src.w),&(src.h));
    if(x>=0 && y>=0){
        SDL_RenderCopy(ren,texture,&src,&dst);
    }
    return OK;
    

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
        if(ent->pos.y+j>1)
            j--;
        for(;j<0;j++){
            if(est_obstacle(ent->chunk->chunk[(int)ent->pos.x+i][(int)ent->pos.y+j]->contenu,GAUCHE)){
                return GAUCHE;
            }
        }
    }
    return 0;
}

static 
int mur_a_droite(entite_t * ent){
    int i,j;
    for(i=-ent->h/2; ent->pos.x + i <0;i++);
    for(;i < ent->h/2 && ent->pos.x+i<CHUNKH;i++){
        for(j=ent->w/2; ent->pos.y + j >=CHUNKW;j--);
        if(ent->pos.y+j<CHUNKW-1)
            j++;
        for(;j>0;j--){
            if(est_obstacle(ent->chunk->chunk[(int)ent->pos.x+i][(int)ent->pos.y+j]->contenu,DROITE)){
                return DROITE;
            }
        }
    }
    return 0;
}

static 
int mur_en_haut(entite_t * ent){
    int i,j;
    for(j=-ent->w/2; ent->pos.y + j <0;j++);
    for(;j < ent->w/2 && ent->pos.y+j<CHUNKW;j++){
        for(i=-ent->h/2; ent->pos.x + i <0;i++);
        if(ent->pos.x+i>1)
            i--;
        for(;i>0;i++){
            if(est_obstacle(ent->chunk->chunk[(int)ent->pos.x+i][(int)ent->pos.y+j]->contenu,HAUT)){
                return HAUT;
            }
        }
    }
    return 0;
}

static 
int mur_en_bas(entite_t * ent){
    int i,j;
    for(j=-ent->w/2; ent->pos.y + j <0;j++);
    for(;j < ent->w/2 && ent->pos.y+j<CHUNKW;j++){
        for(i=ent->h/2; ent->pos.x + i >=CHUNKH;i--);
        if(ent->pos.x+i<CHUNKH-1)
            i++;
        for(;i>0;i--){
            if(est_obstacle(ent->chunk->chunk[(int)ent->pos.x+i][(int)ent->pos.y+j]->contenu,BAS)){
                return BAS;
            }
        }
    }
    return 0;
}

static 
int en_contact_obstacle(entite_t * ent){
    int tot=RIEN;
    tot+=mur_a_gauche(ent);
    tot+=mur_a_droite(ent);
    tot+=mur_en_bas(ent);
    tot+=mur_en_haut(ent);
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
char * str_creer_copier( char * chaine_src){
    char *chaine_dest=NULL;

    chaine_dest=malloc(sizeof(char)*(strlen(chaine_src)+1));
    if(!chaine_dest){
        printf("L'allocation de l'entite n'a pas pu etre effectuee\n");
        return NULL;
    }
    strcpy(chaine_dest,chaine_src);
    chaine_dest[strlen(chaine_src)]='\0';
    return chaine_dest;
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
                        float vitesse_x,float vitesse_y)
{
    entite_t * entite=NULL;
    if (!(entite=malloc(sizeof(entite_t)))){
        printf("L'allocation de l'entite %s n'a pas pu etre effeectuee\n", nom);
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
    entite->afficher_chunk=afficher_dans_chunk;
    entite->afficher_fenetre=afficher_dans_fenetre;
    entite->contact_obstacle=en_contact_obstacle;

    if((entite->nom = str_creer_copier(nom))==NULL){
        printf("Le nom %s n'a pas pu etre attribué\n",nom);
        entite->detruire(&entite);
        return NULL;
    }
    if((entite->description=str_creer_copier( description))==NULL){
        printf("La description de  %s n'a pas pu etre attribué\n",description);
        entite->detruire(&entite);
        return NULL;
    }
    return entite;
}