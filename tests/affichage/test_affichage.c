#include <stdio.h>
#include "../../lib/affichage/room_rendering.h"
#include "../../lib/niveaux/niveau.h"

int main(){
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" );
    int WINW=1280, WINH=720,x=CHUNKH-CHUNKH*1.0*6/72-40,y=TAILLE_MUR+30;
    camera_t cam;
    niveau_t * niv=NULL;
    salle_t * salle=NULL;
    SDL_Window * win=NULL;
    SDL_Renderer * ren=NULL;
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
        return 1;
    }
    if(SDL_CreateWindowAndRenderer(WINW, WINH,SDL_WINDOW_SHOWN,&win, &ren)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    niv=niveau_creer("../../src/niveaux/generation/test_niv.txt");
    if(niv){
        printf("Génération du niveau effectuée\n");
    }
    else{
        printf("La génération de niveau n'a pas fonctionnée\n");
        return 1;
    }
    salle=niv->chercher_salle(niv,0,0);
    cam.chunk=salle->chercher_chunk(salle,0,0);
    niv->lire(niv);

    SDL_Event events;
    SDL_bool run=SDL_TRUE;
    SDL_Texture * bgTexture=creer_texture_image(ren,"../../graphics/texture/room_textures/fond haricot2.png");
    SDL_Texture * murTexture=creer_texture_image(ren, "../../graphics/texture/room_textures/texture_mur.bmp");
    SDL_Texture * joueurTexture=creer_texture_image(ren,"../../graphics/sprite/personnage_sprites/Tom neutre.png");
    if(bgTexture==NULL || murTexture==NULL || joueurTexture==NULL){
        printf("La création de la texture a échouée\n");
        SDL_DestroyTexture(murTexture);
        SDL_DestroyTexture(bgTexture);
        SDL_DestroyTexture(joueurTexture);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return ERR_DEB_MEMOIRE;
    }
    while(run){
        while(SDL_PollEvent(&events)){
            switch (events.type)
            {
            case SDL_WINDOWEVENT: 
                if (events.window.event == SDL_WINDOWEVENT_CLOSE) 
                    run = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                if(events.key.keysym.sym==SDLK_LSHIFT){
                    cam.chunk=salle->chercher_chunk(salle,0,1);
                }
                break;
            case SDL_KEYUP:
                if(events.key.keysym.sym==SDLK_LSHIFT){
                    cam.chunk=salle->chercher_chunk(salle,0,0);
                }
            }
        }
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren,bgTexture,NULL,NULL);
        
        if(render_mur_chunk(ren,murTexture,&cam,WINW,WINH )==ERR_DEB_MEMOIRE){
            niv->detruire(&niv);
            SDL_DestroyTexture(bgTexture);
            SDL_DestroyTexture(murTexture);
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            SDL_Quit();
        }
        SDL_RenderPresent(ren);
    }
    niv->detruire(&niv);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(murTexture);
    SDL_DestroyTexture(joueurTexture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}