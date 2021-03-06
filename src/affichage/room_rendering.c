/* Par Matthis */

#include <stdlib.h>
#include <stdio.h>
#include "../../lib/affichage/room_rendering.h"
#include "../../lib/niveaux/niveau.h"

/**
* \file room_rendering.c
* \brief Module de rendu des environnements
* \author Matthis
* \verion 1.0
* \date Avril 2021
*/



/**
    \brief transforme une image en texture utilisable par SDL2
    paramètres :
        \param SDL_Renderer ren le pointeur sur renderer
        \param char nom_image le pointeur nom du fichier de l'image a utiliser
    retourne :
        \return Un pointeur sur SDL_Texture, la texture créée a partir de l'image en paramètres. En cas d'echec rien n'est retourné.
*/

extern
SDL_Texture * creer_texture_image( SDL_Renderer * ren,char * nom_image){
    SDL_Texture * texture;
    SDL_Surface * surface;
    surface= IMG_Load(nom_image);
    if(!surface){
        return NULL;
    }
    texture =SDL_CreateTextureFromSurface(ren,surface);
    if(!texture){
        free(surface);
        return NULL;
    }
    SDL_FreeSurface(surface);
    return texture;
}
    
/**
    \brief Crée les murs d'un chunk
    paramètres :
        \param SDL_Renderer  ren le pointeur sur renderer
        \param SDL_Texture texture_mur, la texture du mur créer au préalable
        \param salle_t salle, pointeur sur la salle où afficher
        \param chunk_t chunk, pointeur sur le chunk où afficher
        \param int WINW largeur de la fenêtre
        \param int WINH hauteur de la fenetre
    retourne:
        \return OK Si tout s'est bien passé
    render tous les murs d'un chunk 
*/
extern 
err_t render_mur_chunk(SDL_Renderer * ren, SDL_Texture * texture_mur, salle_t *salle,chunk_t * chunk, int WINW, int WINH){
    int i, j, taille=0, w_mur,h_mur,w_mur_modif,h_mur_modif,n;
    pos_t pos;
    const float ratioUtoP= 1.0 * WINW/CHUNKW;                       //Ratio entre une unité et un pixel (si une unité vaut deux pixel, une unité affiché = 2*2 pixels affiché)
    float ratioTaille;
    SDL_RendererFlip flip;
    SDL_Point centre;
    double angle;
    SDL_Rect src, dst, srcModif;
    chunk_t * chunk_dessous=NULL;
    chunk_dessous=salle->chercher_chunk(salle,chunk->position.x+1, chunk->position.y);
    src.x=0;
    src.y=0;
    SDL_QueryTexture(texture_mur,NULL,NULL,&(src.w), &(src.h));     
    srcModif=src;
    for(pos.x=0;pos.x<CHUNKH;){
        for(pos.y=0;pos.y<CHUNKW;){
            pos=prochain_mur(pos.x,pos.y,chunk);                    //On va chercher le prochain mur 
                                                                    //La lecture des murs se fait de gauche a droite puis de faut en bas
            if (pos.x==-1){                                         //S'il n'y en a pas, cela veut dire qu'il n'y a plus de mur a afficher
                return  OK;
            }

            if (pos.x>=CHUNKH-CHUNKH*ratioSol-1 && chunk_dessous==NULL){
                return OK;
            }
            
            attribut_mur(pos.x,pos.y,&w_mur,&h_mur,chunk);     //on récupère la hauteur et l'épaisseur du mur
            h_mur_modif=((pos.x+h_mur) > (CHUNKH - CHUNKH *ratioSol)) && chunk_dessous==NULL ? CHUNKH-CHUNKH*ratioSol - pos.x: h_mur ; 
            srcModif.h=src.w* h_mur_modif/TAILLE_MUR;                     //On adapte la portion de la texture a afficher par rapport a la proportion du mur 
            srcModif.w=src.w* w_mur/TAILLE_MUR;
            dst.y=pos.x*ratioUtoP;
            dst.x=pos.y*ratioUtoP;
            dst.w=w_mur*ratioUtoP;
            dst.h=h_mur_modif*ratioUtoP;

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

/**
    \brief Crée les ponts d'un chunk
    paramètres :
        \param SDL_Renderer ren le pointeur sur renderer
        \param SDL_Texture texture_pont, la texture du pont créer au préalable
        \param salle_t salle, pointeur sur la salle où afficher
        \param chunk_t chunk, pointeur sur le chunk où afficher
        \param int WINW largeur de la fenêtre
        \param int WINH hauteur de la fenetre
    retourne:
        \return OK Si tout s'est bien passé
    render tous les ponts d'un chunk 
*/
extern 
err_t render_pont_chunk(SDL_Renderer * ren, SDL_Texture * texture_pont, salle_t * salle,chunk_t * chunk, int WINW, int WINH){
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

/**
    \brief Crée le fond d'écran d'un chunk
    paramètres :
        \param SDL_Renderer ren le pointeur sur renderer
        \param salle_t salle, pointeur sur la salle dans laquelle dessiner
        \param chunk_t chunk, pointeur sur le chunk dans lequel dessiner
        \param SDL_Texture texture_sol, la texture du sol créée au préalable
        \param SDL_Texture texture_air, la texture de l'air créée au préalable
        \param int WINW largeur de la fenêtre
        \param int WINH hauteur de la fenetre
    retourne:
        \return OK Si tout s'est bien passé
    render tous les ponts d'un chunk 
*/
extern
err_t render_background(SDL_Renderer * ren,salle_t *salle, chunk_t * chunk,SDL_Texture * texture_sol, SDL_Texture * texture_air ,int  WINW,int WINH){
    SDL_Texture * a_afficher=NULL;
    if(salle->chercher_chunk(salle,chunk->position.x+1, chunk->position.y)){
        a_afficher=texture_air;
    }
    else{
        a_afficher=texture_sol;
    }
    SDL_RenderCopy(ren,a_afficher,NULL,NULL);
}

/**
    \brief Crée les entités d'un chunk
    paramètres :
        \param void tab un tableau de pointeurs sur entités
        \param attaque_t tab_attaque un tableau de pointeurs sur des structure 
        \param SDL_Renderer ren le pointeur sur renderer
        \param SDL_Texture texture_pont, la texture du pont créée au préalable
        \param SDL_Texture texture_mur, la texture du mur créée au préalable
        \param salle_t salle, pointeur sur la salle à afficher
        \param chunk_t chunk, pointeur sur le chunk à afficher
        \param int WINW largeur de la fenêtre
        \param int WINH hauteur de la fenetre
    retourne:
        \return OK Si tout s'est bien passé
    render tous les ponts d'un chunk 
*/
extern
err_t render_chunk_unite(void * tab[NB_MAX_AFF],
                         attaque_t *tab_attaque[NB_MAX_ATT], 
                         SDL_Renderer * ren,
                         SDL_Texture * texture_pont,
                         SDL_Texture * texture_mur,
                         salle_t * salle,
                         chunk_t * chunk,
                         int WINW, 
                         int WINH)
{
    render_mur_chunk(ren,texture_mur,salle,chunk,WINW,WINH);
    render_pont_chunk(ren,texture_pont,salle,chunk,WINW,WINH);
    afficher_tableau(tab,ren,WINW,WINH);
    afficher_attaques(tab_attaque,ren,WINW,WINH);
}