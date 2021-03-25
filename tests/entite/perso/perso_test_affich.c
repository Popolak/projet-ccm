#include "stdio.h"
#include "SDL2/SDL.h"
#include "../../../lib/entite/personnage.h"
#include "../../../lib/affichage/room_rendering.h"
#include "../../../lib/niveaux/niveau.h"

int main(){
    int WINW=1280, WINH=720,x=CHUNKH-CHUNKH*ratioSol-30,y=TAILLE_MUR+40;
    chunk_t * chunk;
    niveau_t * niv=NULL;
    salle_t * salle=NULL;
    perso_t *Tom=NULL;
    entite_t * entite_test=NULL;
    SDL_Window * win=NULL;
    SDL_Renderer * ren=NULL;
    pos_t pos={300,200}, pos2={100,400};
    float sec,secAvant, secMaint, secInvins=0.5,secDeg=0;
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
        return 1;
    }
    if(SDL_CreateWindowAndRenderer(WINW, WINH,SDL_WINDOW_SHOWN,&win, &ren)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    niv=niveau_creer("/home/matthis/Documents/Roguelike/src/niveaux/generation/test_niv.txt");
    if(niv){
        printf("Génération du niveau effectuée\n");
    }
    else{
        printf("La génération de niveau n'a pas fonctionnée\n");
        return 1;
    }
    salle=niv->chercher_salle(niv,0,0);
    chunk=salle->chercher_chunk(salle,0,0);
    niv->lire(niv);
    SDL_Event events;
    SDL_bool run=SDL_TRUE;
    SDL_Texture * bgTexture=creer_texture_image(ren,"../../../graphics/texture/room_textures/fond haricot.png");
    SDL_Texture * murTexture=creer_texture_image(ren, "../../../graphics/texture/room_textures/texture_mur.bmp");
    SDL_Texture * joueurTextures[POS_MOUV2+1];
    joueurTextures[NEUTRE]=creer_texture_image(ren,"../../../graphics/sprite/personnage_sprites/Tom vrai neutre.png");
    joueurTextures[POS_MOUV1]=creer_texture_image(ren,"../../../graphics/sprite/personnage_sprites/Tom marche 1.png");
    joueurTextures[POS_MOUV2]=creer_texture_image(ren,"../../../graphics/sprite/personnage_sprites/Tom marche 2.png");
    SDL_Texture * entite_test_texture=creer_texture_image(ren,"../../../graphics/sprite/entite_sprites/test.png");

    Tom=perso_creer("Tom","tomate",30,salle,chunk,pos,0,0,100,700,60,80,30,60,0.3,0,0,2,joueurTextures);
    entite_test=entite_creer("test","test",salle,chunk,pos2,200,-100,400,0.1,40,40,30,30,1,&entite_test_texture);
    if(bgTexture==NULL || murTexture==NULL || joueurTextures[0]==NULL || joueurTextures[1]==NULL || joueurTextures[2]==NULL){
        printf("La création de la texture a échouée\n");
        SDL_DestroyTexture(murTexture);
        SDL_DestroyTexture(bgTexture);
        SDL_DestroyTexture(joueurTextures[0]);
        SDL_DestroyTexture(joueurTextures[1]);
        SDL_DestroyTexture(joueurTextures[2]);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win); 
        SDL_Quit();
        return ERR_DEB_MEMOIRE;
    }
    chunk->lire_partiel(chunk,CHUNKH-CHUNKH*ratioSol-1,100,CHUNKH*ratioSol,30);
    secAvant= 1.0*SDL_GetTicks()/1000;
    secDeg=secAvant;
    int tot_key=0;

    
    while(run){
        secMaint=1.0*SDL_GetTicks()/1000;
        sec=secMaint-secAvant;
        secDeg+=sec;
        Tom->lastSprite+=sec;
        secAvant=secMaint;
        if(sec>0.001){
            Tom->deplacer((entite_t *)Tom,sec);
            entite_test->deplacer(entite_test,sec);
        }
        if(Tom->contact((entite_t*) Tom, entite_test) && secDeg > secInvins){
            Tom->prendre_coup(Tom,10);
            printf("Tom prend un coup %d\n", Tom->vie);
            if(Tom->vie<=0)
                run=SDL_FALSE;
            secDeg=0;
        }
        while(SDL_PollEvent(&events)){
            switch (events.type)
            {
            case SDL_WINDOWEVENT:
                if (events.window.event == SDL_WINDOWEVENT_CLOSE)
                    run = SDL_FALSE;
                break;
            case SDL_KEYDOWN: // Un événement de type touche enfoncée est effectué

                if (events.key.keysym.sym == SDLK_q){
                    tot_key = tot_key | KEY_LEFT;
                }
                if (events.key.keysym.sym == SDLK_d){ 
                    tot_key= tot_key | KEY_RIGHT;
                }
                if (events.key.keysym.sym == SDLK_SPACE){ 
                    tot_key = tot_key | KEY_JUMP;
                }

                break;
            case SDL_KEYUP:// Un événement de type touche relâchée

                if (events.key.keysym.sym == SDLK_q){ 
                    tot_key = tot_key ^ KEY_LEFT;
                }
                if (events.key.keysym.sym == SDLK_d){ 
                    tot_key= tot_key ^ KEY_RIGHT;
                }
                if (events.key.keysym.sym == SDLK_SPACE){ 
                    tot_key= tot_key ^ KEY_JUMP;
                }
            }
        }
        
        Tom->update_speed(Tom, tot_key);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren,bgTexture,NULL,NULL);
        Tom->afficher_chunk(ren,(entite_t*)Tom,WINH,WINW);
        entite_test->afficher_chunk(ren, entite_test,WINH,WINW);

        Tom->hitbox(ren,Tom,WINH,WINW);
        entite_test->hitbox(ren, entite_test,WINH,WINW);
        if(render_mur_chunk(ren,murTexture,Tom->chunk,WINW,WINH )==ERR_DEB_MEMOIRE){
            Tom->detruire(&Tom);
            entite_test->detruire(&entite_test);
            niv->detruire(&niv);
            SDL_DestroyTexture(bgTexture);
            SDL_DestroyTexture(murTexture);
            SDL_DestroyTexture(joueurTextures[0]);
            SDL_DestroyTexture(joueurTextures[1]);
            SDL_DestroyTexture(joueurTextures[2]);
            SDL_DestroyTexture(entite_test_texture);
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            SDL_Quit();
        }
        SDL_RenderPresent(ren);
    }
    Tom->detruire(&Tom);
    entite_test->detruire(&entite_test);
    niv->detruire(&niv);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(murTexture);
    SDL_DestroyTexture(joueurTextures[0]);
    SDL_DestroyTexture(joueurTextures[1]);
    SDL_DestroyTexture(joueurTextures[2]);
    SDL_DestroyTexture(entite_test_texture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}