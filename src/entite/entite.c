#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../../lib/entite/entite.h"


extern 
int est_obstacle(int contenu,  int dir){
    return (contenu==MUR );
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
    SDL_Point centre;
    SDL_RendererFlip flip = entite->dir == DROITE ?  (SDL_RendererFlip)SDL_FLIP_NONE : (SDL_RendererFlip)SDL_FLIP_HORIZONTAL;
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
    centre.x=src.w/2;
    centre.y=src.h/2;
    if(x>=0 && y>=0){
        SDL_RenderCopyEx(ren,texture,&src,&dst,0,&centre,flip);
    }
    return OK;

}


pos_t mur_a_gauche(entite_t * ent){
    int i,j, add=0;
    pos_t pos_mur={-1,-1};
    for(i=0;i < ent->h/2-1 && i > -ent->h/2 && (int)ent->pos.x+i<CHUNKH;i=-(i+add%2),add++){
        for(j=1;(int)ent->pos.y+j>=1 && j>-ent->w/2  && (int)ent->pos.y+j <CHUNKW;j--){
            if(est_obstacle(ent->chunk->chunk[(int)ent->pos.x+i][(int)ent->pos.y+j-1]->contenu,GAUCHE)){
                pos_mur.x=(int)ent->pos.x+i;
                pos_mur.y=(int)ent->pos.y+j;
                break;
            }
        }
        if(pos_mur.x!=-1)
            break;
    }
    return pos_mur;
}


booleen_t est_dans_mur (entite_t * ent){

}

pos_t mur_a_droite(entite_t * ent){
    int i,j, add=0;
    pos_t pos_mur={-1,-1};
    for(i=0;i < ent->h/2-1 && i > -ent->h/2 && (int)ent->pos.x+i<CHUNKH;i=-(i+add%2),add++){
        for(j=1;(int)ent->pos.y+j<CHUNKW-1 && j<ent->w/2 && (int)ent->pos.y+j <CHUNKW;j++){
            if(est_obstacle(ent->chunk->chunk[(int)ent->pos.x+i][(int)ent->pos.y+j+1]->contenu,DROITE)){
                pos_mur.x=(int)ent->pos.x+i;
                pos_mur.y=(int)ent->pos.y+j+1;
                break;
            }
        }
        if(pos_mur.x!=-1)
            break;
    }
    return pos_mur;
}


pos_t mur_en_haut(entite_t * ent){
    int i,j, add=0;
    pos_t pos_mur={-1,-1};
    for(j=0;j < ent->w/2-1 && j > -ent->w/2 && (int)ent->pos.y+j<CHUNKW && ent->pos.y+j>0;j=-(j+add%2),add++){
        for(i=0;i>-ent->h/2 && (int)ent->pos.x+i > 0;i--){
            if(est_obstacle(ent->chunk->chunk[(int)ent->pos.x+i][(int)ent->pos.y+j]->contenu,HAUT)){
                pos_mur.x=(int)ent->pos.x+i+1;
                pos_mur.y=(int)ent->pos.y+j;
                break;
            }
        }
        if(pos_mur.x!=-1)
            break;
    }
    return pos_mur;
}


pos_t mur_en_bas(entite_t * ent){
    int i,j, add=0;
    pos_t pos_mur={-1,-1};
    for(j=0;j < ent->w/2-1 && j > -ent->w/2 && (int)ent->pos.y+j<CHUNKW && ent->pos.y+j>0;j=-(j+add%2),add++){
        for(i=0;i<ent->h/2 && (int)ent->pos.x+i < CHUNKH;i++){
            if(est_obstacle(ent->chunk->chunk[(int)ent->pos.x+i][(int)ent->pos.y+j]->contenu,HAUT)){
                pos_mur.x=(int)ent->pos.x+i+1;
                pos_mur.y=(int)ent->pos.y+j;
                break;
            }
        }
        if(pos_mur.x!=-1)
            break;
    }
    return pos_mur;
}

static 
booleen_t en_lair(entite_t * ent){
    pos_t pos_mur;
    pos_mur=mur_en_bas(ent);
    return pos_mur.x==-1;
}

void replacer(entite_t * ent, pos_t pos_mur, int direction){
    switch (direction){
        case (DROITE):
            ent->vitesse_y=0;
            ent->pos.y=pos_mur.y-ent->w/2;
            break;
        case (BAS):
            ent->vitesse_x=0;
            ent->pos.x=pos_mur.x-ent->h/2;
            break;
        case (GAUCHE):
            ent->vitesse_y=0;
            ent->pos.y=pos_mur.y+ent->w/2;
            break;
        case (HAUT):
            ent->vitesse_x=0;
            ent->pos.x=pos_mur.x+ent->h/2;
            break;
    }
}



static 
void entite_deplacement(entite_t * ent,double temps){
    pos_t pos_mur,pos_mur_verif, pos_mur_double_verif,ent_pos_verif;
    chunk_t * chunk;
    booleen_t deja=FAUX;
    int w,h;
    float vitesse_tempo;
    (ent->pos.y)+= (ent->vitesse_y)*temps;

    pos_mur=mur_a_gauche(ent);
    if(pos_mur.x!=-1){
        replacer(ent,pos_mur,GAUCHE);
    }
    
    pos_mur=mur_a_droite(ent);
    if(pos_mur.x!=-1){
        replacer(ent,pos_mur,DROITE);
    }

    (ent->pos.x)+= (ent->vitesse_x)*temps;
    pos_mur=mur_en_bas(ent);
    if(pos_mur.x!=-1){
        replacer(ent,pos_mur,BAS);
    }

    pos_mur=mur_en_haut(ent);
    if(pos_mur.x!=-1 ){
        replacer(ent,pos_mur,HAUT);
    }

    
    if(ent->en_l_air(ent)){
        ent->vitesse_x+=GRAVITE*temps;
    }

    if(ent->vitesse_y > 0)
        ent->dir=DROITE;
    else if(ent->vitesse_y < 0)
        ent->dir=GAUCHE;

    
    
    if(ent->pos.y>=CHUNKW){
        if((chunk=ent->salle->chercher_chunk(ent->salle,ent->chunk->position.x, ent->chunk->position.y+1))!=NULL){
            ent->chunk=chunk;
            ent->pos.y=ent->pos.y-CHUNKW;
        }
        else{
            printf("Erreur de segmentation, pas de chunk a droite\n");
            exit(1);
        }
    }
    if(ent->pos.y<0){
        if((chunk=ent->salle->chercher_chunk(ent->salle,ent->chunk->position.x, ent->chunk->position.y-1))!=NULL){
            ent->chunk=chunk;
            ent->pos.y=CHUNKW+ent->pos.y;
        }
        else{
            printf("Erreur de segmentation, pas de chunk a gauche\n");
            exit(1);
        }
    }
    if(ent->pos.x>=CHUNKH){
        if((chunk=ent->salle->chercher_chunk(ent->salle,ent->chunk->position.x+1, ent->chunk->position.y))!=NULL){
            ent->chunk=chunk;
            ent->pos.x=ent->pos.x-CHUNKH;
        }
        else{
            printf("Erreur de segmentation, pas de chunk en bas\n");
            exit(1);
        }
    }
    if(ent->pos.x<0){
        if((chunk=ent->salle->chercher_chunk(ent->salle,ent->chunk->position.x-1, ent->chunk->position.y))!=NULL){
            ent->chunk=chunk;
            ent->pos.x=CHUNKH+ent->pos.y;
        }
        else{
            printf("Erreur de segmentation,pas de chunk en haut\n");
            exit(1);
        }
    }

    if(ent->vitesse_y){
        if(!ent->en_l_air(ent))
            ent->vitesse_y= ent->vitesse_y -  DECEL*COEFF_DECEL_SOL* temps * (ent->vitesse_y>0 ? 1 : -1)  ;

        else
            ent->vitesse_y= ent->vitesse_y -  DECEL*temps * (ent->vitesse_y>0 ? 1 : -1)  ;

        if(ent->vitesse_y > 0 && ent->dir == GAUCHE || ent->vitesse_y < 0 && ent->dir == DROITE)
            ent->vitesse_y=0;
    }
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
                        salle_t * salle,
                        chunk_t * chunk,
                        pos_t pos,
                        float vitesse_x, float vitesse_y, float vitesse_max_y,
                        int w, int h, 
                        int w_hitbox, int h_hitbox, 
                        int nbTextures,
                        SDL_Texture ** textures)
{
    entite_t * entite=NULL;
    if (!(entite=malloc(sizeof(entite_t)))){
        printf("L'allocation de l'entite %s n'a pas pu etre effeectuee\n", nom);
        return  NULL;
    }

    entite->envie=VRAI;
    entite->pos=pos;
    entite->w=w;
    entite->h=h;
    entite->w_hitbox=w_hitbox;
    entite->h_hitbox=h_hitbox;
    entite->nbTextures=nbTextures;
    entite->vitesse_x=vitesse_x;
    entite->vitesse_y=vitesse_y;
    entite->vitesse_max_y=vitesse_max_y;
    entite->chunk=chunk;
    entite->salle=salle;
    entite->textures=textures;
    entite->dir= vitesse_y >= 0 ? DROITE : GAUCHE;

    entite->detruire=entite_detruire;
    entite->lire=entite_lire;
    entite->afficher_chunk=afficher_dans_chunk;
    entite->afficher_fenetre=afficher_dans_fenetre;
    entite->deplacer=entite_deplacement;
    entite->en_l_air=en_lair;

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