#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "../../lib/entite/entite.h"

/*Fonctions*/

/*
    est_obstacle:
    paramètres:
        int contenu, contenu d'une unité d'un chunk
        int dir, direction d'une entité
    retourne 1 si le contenu est un obstacle a une entité compte tenu de la direction de l'entité 
*/
extern 
int est_obstacle(int contenu,  int dir){
    return (contenu==MUR );
}

/*
    afficher_dans_chunk:
    paramètres: 
        pointeur sur SDL_Renderer, le renderer
        pointeur sur entite_t, l'entite a afficher
        int WINW,int WINH, la hauteur et la largeur de la fenetre
    retourne OK si tout s'est bien passé, une erreur sinon

    Utilisé pour afficher les entités qui sont sur le chunk a afficher
*/


static err_t afficher_dans_chunk(SDL_Renderer *ren,entite_t *entite,int WINH,int WINW){
    SDL_Texture * a_afficher=NULL;
    if(entite->lastSprite >= 4*entite->secSprite ){
        entite->lastSprite=0;
    }
    if(entite->vitesse_y==0){
        entite->lastSprite=0;
        a_afficher=entite->textures[NEUTRE];
    }

    if(entite->vitesse_x < 0 ){
        if(entite->nbTextures <= POS_MOUV2)
            a_afficher= entite->textures[NEUTRE];
        else 
            a_afficher=entite->textures[SAUT];
    }
    else if(entite->vitesse_x > 0 ){
        if(entite->nbTextures <= SAUT)
            a_afficher= entite->textures[NEUTRE];
        else 
            a_afficher=entite->textures[TOMBE];
    }
    else {

        if(abs(entite->vitesse_y) && entite->nbTextures > NEUTRE && !entite->en_l_air(entite)){
            if(entite->secSprite > entite->lastSprite){
                a_afficher=entite->textures[POS_MOUV1];
                if(!a_afficher){
                    a_afficher=entite->textures[NEUTRE];
                }
            }
             
            else if (entite->lastSprite > 2 * entite->secSprite && entite->lastSprite < 3 * entite->secSprite && entite->nbTextures > POS_MOUV1){
                a_afficher=entite->textures[POS_MOUV2];
                if(!a_afficher)
                    a_afficher=entite->textures[NEUTRE];
            }
            else
                a_afficher=entite->textures[NEUTRE];
        }
    }
    


    entite->afficher_fenetre(ren,entite,entite->w*WINW/CHUNKW,entite->h*WINW/CHUNKW,entite->pos.x*WINW/CHUNKW, entite->pos.y*WINW/CHUNKW, a_afficher);
}


static 
void afficher_hitbox(SDL_Renderer * ren, entite_t * ent, int WINH, int WINW){
    SDL_Rect hitbox;
    hitbox.x=(ent->pos.y - ent->w_hitbox/2)* WINW/CHUNKW; 
    hitbox.y=(ent->pos.x - ent->h_hitbox/2)* WINW/CHUNKW; 
    hitbox.w=ent->w_hitbox*WINW/CHUNKW;
    hitbox.h=ent->h_hitbox*WINW/CHUNKW;
    SDL_SetRenderDrawColor(ren,255,0,0,255);
    SDL_RenderDrawRect(ren,&hitbox);
}

/*
    afficher dans fenetre
    paramètres:
        pointeur sur SDL_Renderer, le renderer
        pointeur sur entite_t, l'entite a afficher
        int w, h la hauteur et largeur (en pixel) de l'entité a afficher
        int x, y les les coordonées (en pixel) de l'entité a afficher 
        pointeur sur SDL_Texture, la texture a utiliser
    retourne OK si tout se passe bien
*/

static err_t afficher_dans_fenetre(SDL_Renderer * ren,entite_t * entite, int w, int h, int x, int y, SDL_Texture * texture){
    
      
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
        SDL_RenderCopyEx(ren,texture,&src,&dst,0,&centre,flip);     //En fonction de la direction de l'entité on retourne symétriquement ou pas la texture
    }
    return OK;

}

/*
    mur_*:
    paramètre:
        pointeur sur entite_t
    retourne la position du premier mur que la fonction croise, -1 s'il n'y en a pas

    Ces 4 fonctions vérifient si l'entité est en collision avec un mur de chaque coté (gauch droit haut bas)
*/


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

/*
    en_lair
    paramètre:
        pointeur sur entite_t
    retourne VRAI si l'entité ne touche pas d'obstacle en dessous d'elle, FAUX sinon
*/

static 
booleen_t en_lair(entite_t * ent){
    pos_t pos_mur;
    pos_mur=mur_en_bas(ent);
    return pos_mur.x==-1;
}

/*
    replacer
    paramètres:
        pointeur sur entite_t, l'entité a replacer hors d'un obstacle
        pos_t pos_mur, la position du mur
        int direction, la direction dans laquelle on doit replacer l'entité
    
*/

void replacer(entite_t * ent, pos_t pos_mur, int direction){
    switch (direction){
        case (DROITE):
            ent->vitesse_y=0;
            ent->pos.y=pos_mur.y-ent->w/2;             //Si direction==DROITE, on replace l'entité vers la gauche du mur
            break;
        case (BAS):
            ent->vitesse_x=0;
            ent->pos.x=pos_mur.x-ent->h/2;             //Si direction==BAS, on replace l'entité vers le haut du mur etc...
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

/*
    entitre_deplacement:
    paramètres:
        entite_t * ent, l'entité a déplacer
        double temps, le temps en secondes depuis le dernier déplacement
    
    déplace l'entité en fonction de ses vitesses et du chunk
*/


static 
void entite_deplacement(entite_t * ent,double temps){
    pos_t pos_mur;
    chunk_t * chunk;
    booleen_t deja=FAUX;
    int w,h;
    float vitesse_tempo;
    (ent->pos.y)+= (ent->vitesse_y)*temps;                  //On actualise la position horizontale grace a vitesse_y
                                                                
    pos_mur=mur_a_gauche(ent);                              //Si on se retrouve dans un mur, on se replace
    if(pos_mur.x!=-1){
        replacer(ent,pos_mur,GAUCHE);
    }
    
    pos_mur=mur_a_droite(ent);                              //Pareil
    if(pos_mur.x!=-1){
        replacer(ent,pos_mur,DROITE);
    }

    (ent->pos.x)+= (ent->vitesse_x)*temps;                  //Puis on actualise la position verticale
    pos_mur=mur_en_bas(ent);
    if(pos_mur.x!=-1){
        replacer(ent,pos_mur,BAS);                          //Si on se retrouve dans un mur, on se replace
    }

    pos_mur=mur_en_haut(ent);
    if(pos_mur.x!=-1 ){
        replacer(ent,pos_mur,HAUT);                         //Pareil
    }

    
    if(ent->en_l_air(ent)){                                 //Si on est en l'air, on tombe (La vitesse maximale est atteinte après 2 secondes de chute avec v0=0)
        ent->vitesse_x= (ent->vitesse_x + GRAVITE*temps > GRAVITE*2) ? GRAVITE*2:ent->vitesse_x + GRAVITE*temps;
    }

    if(ent->vitesse_y > 0)                          //On adapte la direction de l'entité en fonction de sa vitesse
        ent->dir=DROITE;                            //Si elle est nulle, on ne change rien 
    else if(ent->vitesse_y < 0)
        ent->dir=GAUCHE;

    
    
    if(ent->pos.y>=CHUNKW){                             //Si le personnage se retrouve trop a droite, il sort du chunk 
        if((chunk=ent->salle->chercher_chunk(ent->salle,ent->chunk->position.x, ent->chunk->position.y+1))!=NULL){ //alors on vérifie s'il y en a un a droite
            ent->chunk=chunk;                           //Le chunk a droite devient le nouveau chunk de l'entité
            ent->pos.y=ent->pos.y-CHUNKW;               //Sa position change en fonction
        }
        else{
            printf("Erreur de segmentation, pas de chunk a droite\n");
            exit(1);
        }
    }               

                                            //Meme systeme ensuite
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

    //Si la vitesse de l'entité n'est pas actualisée (soit par un input de l'utilisateur, soit par l'algorithme des ennemis)
    //Alors on la change grace a la décélération

    if(ent->vitesse_y){
        if(!ent->en_l_air(ent))
            ent->vitesse_y= ent->vitesse_y -  DECEL*COEFF_DECEL_SOL* temps * (ent->vitesse_y>0 ? 1 : -1)  ; //Sur le sol, la décélération est COEFF_DECEL_SOL fois plus grande

        else
            ent->vitesse_y= ent->vitesse_y -  DECEL*temps * (ent->vitesse_y>0 ? 1 : -1)  ;                  //Que dans l'air

        if(ent->vitesse_y > 0 && ent->dir == GAUCHE || ent->vitesse_y < 0 && ent->dir == DROITE)
            ent->vitesse_y=0;
    }
}


/*
    en_contact
    paramètres:
        2 pointeur sur entite_t
    Retourne VRAI si les deux entités sont en collision de hitbox, FAUX sinon
*/
static
booleen_t en_contact(entite_t * ent_courante, entite_t * ent_a_verif){
    if(ent_a_verif->chunk !=  ent_courante->chunk)
        return FAUX;
    if (ent_courante->w_hitbox == 0 && ent_courante->w_hitbox == 0 )
        return FAUX;
    if(abs(ent_courante->pos.y - ent_a_verif->pos.y) <= (ent_courante->w_hitbox + ent_a_verif->w_hitbox)/2 && abs(ent_courante->pos.x - ent_a_verif->pos.x) <= (ent_courante->h_hitbox + ent_a_verif->h_hitbox)/2)
        return VRAI;
    return FAUX;
}

/*
    str_creer_copier
    paramètre:
        chaine de caractères, chaine source a copier
    retourne un pointeur sur char différent de chaine_src mais avec la même chaine de caractères, NULL si ça s'est mal passé 
*/

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

/*
    entite_lire
    paramètre:
        pointeur sur entite_t
    affiche le nom et la description de l'entité
*/
static 
void entite_lire(entite_t * ent){
    printf ("%s :\n%s", ent->nom, ent->description);
}

/*
    entite_detruire 
    paramètre:
        pointeur sur pointeur sur entite_t
    retourne OK si tout s'est bien passé

    libère l'entité et toutes les allocations reliées a celle ci
*/

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
    return OK;
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
                        float secSprite,
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
    entite->secSprite=secSprite;
    entite->chunk=chunk;
    entite->salle=salle;
    entite->textures=textures;
    entite->dir= vitesse_y >= 0 ? DROITE : GAUCHE;
    entite->lastSprite=0;

    entite->detruire=entite_detruire;
    entite->lire=entite_lire;
    entite->afficher_chunk=afficher_dans_chunk;
    entite->afficher_fenetre=afficher_dans_fenetre;
    entite->deplacer=entite_deplacement;
    entite->en_l_air=en_lair;
    entite->contact=en_contact;
    entite->hitbox=afficher_hitbox;


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