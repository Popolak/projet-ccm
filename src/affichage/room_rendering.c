/* Par Matthis */

#include <stdlib.h>
#include <stdio.h>
#include "../../lib/affichage/room_rendering.h"
#include "../../lib/niveaux/salle.h"

/* Fonctions */


/*
    creer_texture_image:
    paramètres :
        SDL_Renderer * ren le pointeur sur renderer
        char * nom_image, le nom du fichier de l'image a utiliser
    retourne :
        pointeur sur SDL_Texture, la texture créée a partir de l'image en paramètres
    rend le code plus clair
*/

extern
SDL_Texture * creer_texture_image( SDL_Renderer * ren,char * nom_image){
    SDL_Texture * texture;
    SDL_Surface * surface;
    surface= IMG_Load(nom_image);
    if(!surface){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s\n", SDL_GetError());
        return NULL;
    }
    texture =SDL_CreateTextureFromSurface(ren,surface);
    if(!texture){
        free(surface);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s\n", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(surface);
    return texture;
}


/*
    render_mur_chunk
    paramètres :
        SDL_Renderer * ren le pointeur sur renderer
        SDL_Texture * texture_mur, la texture du mur créer au préalable
        camera_t * cam, pointeur sur la camera pour savoir quel chunk afficher
        int WINW et int WINH hight et width de la fenetre
    retourne:
        OK Si tout s'est bien passé
    render tous les murs d'un chunk 
*/
extern 
err_t render_mur_chunk(SDL_Renderer * ren, SDL_Texture * texture_mur, chunk_t * chunk, int WINW, int WINH){
    int i, j, taille=0, w_mur,h_mur,n;
    pos_t pos;
    const float ratioUtoP= 1.0 * WINW/CHUNKW;                       //Ratio entre une unité et un pixel (si une unité vaut deux pixel, une unité affiché = 2*2 pixels affiché)
    float ratioTaille;
    SDL_RendererFlip flip;
    SDL_Point centre;
    double angle;
    SDL_Rect src, dst, srcModif;
    src.x=0;
    src.y=0;
    SDL_QueryTexture(texture_mur,NULL,NULL,&(src.w), &(src.h));     
    srcModif=src;
    for(pos.x=0;pos.x<CHUNKH;){
        for(pos.y=0;pos.y<CHUNKW;){
            pos=prochain_mur(pos.x,pos.y,chunk);               //On va chercher le prochain mur 
                                                                    //La lecture des murs se fait de gauche a droite puis de faut en bas
            if (pos.x==-1){                                         //S'il n'y en a pas, cela veut dire qu'il n'y a plus de mur a afficher
                return  OK;
            }

            if (pos.x>=CHUNKH-CHUNKH*ratioSol){
                return OK;
            }
            
            attribut_mur(pos.x,pos.y,&w_mur,&h_mur,chunk);     //on récupère la hauteur et l'épaisseur du mur

            srcModif.h=src.w* h_mur/TAILLE_MUR;                     //On adapte la portion de la texture a afficher par rapport a la proportion du mur 
            srcModif.w=src.w* w_mur/TAILLE_MUR;
            dst.y=pos.x*ratioUtoP;
            dst.x=pos.y*ratioUtoP;
            dst.w=w_mur*ratioUtoP;
            dst.h=h_mur*ratioUtoP;

            centre.x= dst.w/2;
            centre.y= dst.h/2;
            if(dst.y==0){
                angle=90;
                flip= SDL_FLIP_NONE;
            }
            else if(dst.x > WINW/2){
                angle=0;
                flip = SDL_FLIP_HORIZONTAL;
            }
            else {
                angle=0;
                flip= SDL_FLIP_NONE;
            }

            SDL_RenderCopyEx(ren,texture_mur,&srcModif,&dst,angle,&centre,flip);         //On affiche
            pos.y+=w_mur;                                           
        }
        pos.x+=h_mur;
    }
    return OK;
}

extern 
err_t render_pont_chunk(SDL_Renderer * ren, SDL_Texture * texture_pont, chunk_t * chunk, int WINW, int WINH){
    int i,j,w_pont;
    const float ratioUtoP= 1.0 * WINW/CHUNKW;  
    SDL_Rect src, dst, srcModif;
    src.x=0;
    src.y=0;
    pos_t pos;
    SDL_Point centre;
    SDL_QueryTexture(texture_pont,NULL,NULL,&(src.w), &(src.h));
    srcModif=src;
    for(pos.x=0;pos.x<CHUNKH;){
        for(pos.y=0;pos.y<CHUNKW;){
            pos=prochain_pont(pos.x,pos.y, chunk);
            if(pos.x==-1)
                return OK;
            attribut_pont(pos.x,pos.y,&w_pont,chunk);

            srcModif.w=src.w* w_pont/W_PONT;
            dst.y=pos.x*ratioUtoP -  (5.0/6)*H_PONT_AFFICH*ratioUtoP;
            dst.x=pos.y*ratioUtoP;
            dst.w=w_pont*ratioUtoP/2;
            dst.h=H_PONT_AFFICH*ratioUtoP;

            centre.x= dst.w/2;
            centre.y= dst.h/2;

            SDL_RenderCopyEx(ren,texture_pont,&srcModif,&dst,0,&centre,SDL_FLIP_NONE);         //On affiche

            dst.x+=w_pont*ratioUtoP/2 ;
            SDL_RenderCopyEx(ren,texture_pont,&srcModif,&dst,0,&centre,SDL_FLIP_HORIZONTAL);
            pos.y+=w_pont;
        }
        pos.x+=H_PONT;
    }
    return OK;
}

extern
err_t render_chunk_unite(void * tab[NB_MAX_AFF], 
                         SDL_Renderer * ren,
                         SDL_Texture * texture_pont,
                         SDL_Texture * texture_mur,
                         chunk_t * chunk,
                         int WINW, 
                         int WINH)
{
    render_mur_chunk(ren,texture_mur,chunk,WINW,WINH);
    render_pont_chunk(ren,texture_pont,chunk,WINW,WINH);
    afficher_tableau(tab,ren,WINW,WINH);
}