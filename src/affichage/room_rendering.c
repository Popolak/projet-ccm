#include <stdlib.h>
#include <stdio.h>
#include "../../lib/affichage/room_rendering.h"
#include "../../lib/niveaux/salle.h"


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
extern 
err_t render_mur(SDL_Renderer * ren,SDL_Texture * texture_mur, camera_t * cam, int WINH, int WINW){
    int i=0,j=0,ecartx=0,ecarty=0 ,Tw,Th;
    float ratioPtoU=1.0*WINW/(cam->semiw*2);
    if(ratioPtoU<1){
        return CAM_TROP_GRANDE;
    }
    SDL_Rect src, dst;
    chunk_t * chunk;
    src.x=0;
    src.y=0;
    dst.h=TAILLE_MUR;
    dst.w=TAILLE_MUR;
    SDL_QueryTexture(texture_mur,NULL,NULL,&Tw,&Th);
    if(cam->centre.x - cam->semih < 0){

        /*if((chunk = chunk_audDessus(cam->salle, cam->chunk)) == NULL){
            dst.y= (cam->semiw- cam->centre.y)*ratioPtoU;

            /*for(i=CHUNKH-TAILLE_MUR;i>CHUNKH- (cam->semih-cam->centre.x);i-=TAILLE_MUR){
                for(j= ((cam->centre.y - cam->semiw < 0) ? 0 : cam->centre.y - cam->semiw) ;j<((cam->semiw+cam->centre.y) > CHUNKW ? CHUNKW : (cam->semiw+cam->centre.y));j+=TAILLE_MUR){
                    
                }
            }
        }
        //
    }
    ecartx=calcul_ecart_x(cam,0);

    if(ecartx<0){
        dst.x=0;
    }

    ecarty=calcul_ecart_y(cam,0);

    if(ecarty<0){
        dst.y=0;
    }
}  */


extern 
err_t render_mur_chunk(SDL_Renderer * ren, SDL_Texture * texture_mur, camera_t * cam, int WINW, int WINH){
    int i, j, taille=0, w_mur,h_mur,n;
    pos_t pos;
    const float ratioPtoU= 1.0 * WINW/CHUNKW;
    float ratioTaille;
    SDL_Rect src, dst, srcModif;
    src.x=0;
    src.y=0;
    dst.w= TAILLE_MUR * ratioPtoU;
    dst.h= TAILLE_MUR * ratioPtoU;
    SDL_QueryTexture(texture_mur,NULL,NULL,&(src.w), &(src.h));
    srcModif=src;
    for(pos.x=0;pos.x<CHUNKH;){
        for(pos.y=0;pos.y<CHUNKW;){
            pos=prochain_mur(pos.x,pos.y,cam->chunk);
            if (pos.x==-1){
                return  OK;
            }

            if (pos.x>=CHUNKH-CHUNKH*ratioSol){
                return OK;
            }
            
            attribut_mur(pos.x,pos.y,&w_mur,&h_mur,cam->chunk);

            srcModif.h=src.w* h_mur/TAILLE_MUR;
            srcModif.w=src.w* w_mur/TAILLE_MUR;
            dst.y=pos.x*ratioPtoU;
            dst.x=pos.y*ratioPtoU;
            dst.w=w_mur*ratioPtoU;
            dst.h=h_mur*ratioPtoU;
            SDL_RenderCopy(ren,texture_mur,&srcModif,&dst);
            pos.y+=w_mur;
        }
        pos.x+=h_mur;
    }
    return OK;
}

