#include "stdio.h"
#include "SDL2/SDL.h"
#include "../../../lib/entite/personnage.h"
#include "../../../lib/affichage/room_rendering.h"
#include "../../../lib/niveaux/niveau.h"

int main(){
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" );
    int WINW=1280, WINH=720,x=CHUNKH-CHUNKH*ratioSol-30,y=TAILLE_MUR+40;
    float i=0.1;
    camera_t cam;
    niveau_t * niv=NULL;
    salle_t * salle=NULL;
    perso_t *Tom=NULL;
    SDL_Window * win=NULL;
    SDL_Renderer * ren=NULL;
    pos_t pos={CHUNKH-CHUNKH*ratioSol-40,TAILLE_MUR+80};
    float sec,secAvant, secMaint;
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
        return 1;
    }
    if(SDL_CreateWindowAndRenderer(WINW, WINH,SDL_WINDOW_SHOWN,&win, &ren)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    niv=niveau_creer("../../../src/niveaux/generation/test_niv.txt");
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
    SDL_Texture * bgTexture=creer_texture_image(ren,"../../../graphics/texture/room_textures/fond haricot.png");
    SDL_Texture * murTexture=creer_texture_image(ren, "../../../graphics/texture/room_textures/texture_mur.bmp");
    SDL_Texture * joueurTexture=creer_texture_image(ren,"../../../graphics/sprite/personnage_sprites/Tom neutre.png");
    Tom=perso_creer("Tom","tomate",20,pos,60,80,60,80,1,&joueurTexture,0,0,700,200,0,0,cam.chunk,cam.salle);
    Tom->pos.x=CHUNKH-CHUNKH*ratioSol-Tom->h/2;
    Tom->pos.x=200; //
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
    cam.chunk->lire_partiel(cam.chunk,CHUNKH-CHUNKH*ratioSol-1,100,CHUNKH*ratioSol,30);
    secAvant= 1.0*SDL_GetTicks()/1000;
    int q=SDLK_q,d=SDLK_d, tot=0;


    printf("%f %f\n",Tom->pos.x, Tom->pos.y);
    while(run){
        secMaint=1.0*SDL_GetTicks()/1000;
        sec=secMaint-secAvant;
        secAvant=secMaint;
        Tom->deplacer((entite_t *)Tom,sec);
        while(SDL_PollEvent(&events)){
            switch (events.type)
            {
            case SDL_WINDOWEVENT:
                if (events.window.event == SDL_WINDOWEVENT_CLOSE)
                    run = SDL_FALSE;
                break;
            case SDL_KEYDOWN: // Un événement de type touche enfoncée est effectué
                SDL_Log("+key");  // Affiche un message +key (pour dire qu'on appuie sur une touche du clavier)

                if (events.key.keysym.sym == SDLK_q){ // Regarde si le scancode W est enfoncé (Z sous un azerty)
                    SDL_Log("Q\n"); // Affiche un message
                    tot=q;
                }
                if (events.key.keysym.sym == SDLK_d){ // Regarde si le scancode W est enfoncé (Z sous un azerty)
                    SDL_Log("D\n"); // Affiche un message
                    tot=d;
                }
                if (events.key.keysym.sym == SDLK_SPACE){ // Regarde si le scancode W est enfoncé (Z sous un azerty)
                    SDL_Log("D\n"); // Affiche un message
                    tot=SDLK_SPACE;
                }

                break;
            case SDL_KEYUP:// Un événement de type touche relâchée
                SDL_Log("-key");
                break;
            }
        }
        Tom->update_speed(Tom, tot);
        //printf("%f\n", Tom->acceleration_y);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren,bgTexture,NULL,NULL);
        Tom->afficher_chunk(ren,(entite_t*)Tom,WINH,WINW);
        
        if(render_mur_chunk(ren,murTexture,&cam,WINW,WINH )==ERR_DEB_MEMOIRE){
            Tom->detruire(&Tom);
            niv->detruire(&niv);
            SDL_DestroyTexture(bgTexture);
            SDL_DestroyTexture(murTexture);
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            SDL_Quit();
        }
        SDL_RenderPresent(ren);
    }
    Tom->detruire(&Tom);
    niv->detruire(&niv);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(murTexture);
    SDL_DestroyTexture(joueurTexture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}