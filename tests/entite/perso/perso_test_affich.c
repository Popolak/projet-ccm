/* Par Matthis */

#include "stdio.h"
#include "SDL2/SDL.h"
#include "../../../lib/entite/entite.h"
#include "../../../lib/entite/personnage.h"
#include "../../../lib/affichage/room_rendering.h"
#include "../../../lib/niveaux/niveau.h"
#include "../../../lib/generation/element_generation.h"

int main(){
    void * tableau_entite[NB_MAX_AFF];
    err_t (*tab_destr[NB_MAX_AFF])(void ** );
    attaque_t * tableau_attaque[NB_MAX_ATT];


    initTabDaffich(tableau_entite);
    initTabDaffich((void**)tab_destr);
    initTabDaffich((void**)tableau_attaque);


    int WINW=1280, WINH=720,x=CHUNKH-CHUNKH*ratioSol-30,y=TAILLE_MUR+40, nbText,n, deplacer;
    chunk_t * chunk;
    niveau_t * niv=NULL;
    salle_t * salle=NULL;
    perso_t *Tom=NULL;
    entite_t * entite_test=NULL;
    SDL_Window * win=NULL;
    SDL_Renderer * ren=NULL;
    pos_t pos={300,200}, pos2={100,400};
    FILE * index= NULL, * entite_gen= NULL;
    float sec,secAvant, secMaint, secInvins=0.5;
    index=fopen("../../../generation/index_entite.txt", "r");
    if(!index){
        printf("Pas d'index\n");
    }
    
    entite_gen =fopen("../../../generation/generation_ent.txt","r");
    if(!entite_gen){
        printf("Pas de fichier de génération d'éléments\n");
    }
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
        return 1;
    }
    if(SDL_CreateWindowAndRenderer(WINW, WINH,SDL_WINDOW_SHOWN,&win, &ren)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    niv=niveau_creer("../../../generation/test_niv.txt");
    if(niv){
        printf("Génération du niveau effectuée\n");
    }
    else{
        printf("La génération de niveau n'a pas fonctionnée\n");
        return 1;
    }
    
    salle=niv->chercher_salle(niv,0,0);
    chunk=salle->chercher_chunk(salle,0,0);
    SDL_Event events;
    SDL_bool run=SDL_TRUE;
    SDL_Texture * bgTexture=creer_texture_image(ren,"../../../graphics/texture/room_textures/fond haricot.png");
    SDL_Texture * murTexture=creer_texture_image(ren, "../../../graphics/texture/room_textures/mur 2 essai.png");
    SDL_Texture * pontTexture=creer_texture_image(ren, "../../../graphics/texture/room_textures/pont.png");
    SDL_Texture ** joueurTextures=NULL, **entite_test_textures;

    joueurTextures=creer_tableau_textures_chaine(ren,&nbText,"\"../../../graphics/sprite/personnage_sprites/Tom immo.png\" \"../../../graphics/sprite/personnage_sprites/Tom neutre.png\" \"../../../graphics/sprite/personnage_sprites/Tom marche 1.png\" \"../../../graphics/sprite/personnage_sprites/Tom marche 2.png\" \"../../../graphics/sprite/personnage_sprites/Tom immo.png\" \"../../../graphics/sprite/personnage_sprites/Tom immo.png\" \"../../../graphics/sprite/personnage_sprites/Tom attaque.png\"","./");
    Tom=perso_creer("Tom","tomate",30,salle,chunk,pos,0,0,300,750,40,70,20,50,-5,0.2,0.4,0,"tom_attaque",nbText,joueurTextures);

    ajouter_tableaux(tableau_entite,tab_destr, creer_entite_chaine(ren,&n,Tom, " patate 300 200",index,"../../../"),(void*) entite_detruire );
    ajouter_tableaux(tableau_entite,tab_destr, creer_entite_chaine(ren,&n,Tom, " patate 200 400",index,"../../../"),(void*)entite_detruire );

    if(bgTexture==NULL || murTexture==NULL || joueurTextures[0]==NULL || joueurTextures[1]==NULL || joueurTextures[2]==NULL){
        printf("La création de la texture a échouée\n");
        SDL_DestroyTexture(murTexture);
        SDL_DestroyTexture(bgTexture);
        Tom->detruire(&Tom);
        vider_tableaux(tableau_entite,tab_destr);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win); 
        SDL_Quit();
        return ERR_DEB_MEMOIRE;
    }


    secAvant= 1.0*SDL_GetTicks()/1000;

    int tot_key=0;
    while(run){
        secMaint=1.0*SDL_GetTicks()/1000;
        sec=secMaint-secAvant;
        secAvant=secMaint;
        if(  Tom->update_speed(Tom,tot_key,sec)){
            Tom->nouvelle_attaque(ren,tableau_attaque,Tom,index,"../../../");
        }
        deplacer=Tom->deplacer(Tom,sec);
        if(deplacer==1){
            Tom->change_chunk(ren, Tom, tableau_entite,tab_destr,tableau_attaque,index,entite_gen,"../../../");
        }
        else if(deplacer==2){
            Tom->change_salle(Tom);
            Tom->change_chunk(ren,Tom,tableau_entite,tab_destr,tableau_attaque,index,entite_gen,"../../../");      
        }
        else{
            synchro_attaque(tableau_attaque,sec);
            synchro_tableau(tableau_entite,tab_destr,sec,NULL);
            tableau_contact(tableau_entite,tab_destr,Tom);
            contact_attaque_ennemis(tableau_attaque,tableau_entite,tab_destr);
        }
        
        
        while(SDL_PollEvent(&events)){
            switch (events.type)
            {
            case SDL_WINDOWEVENT:
                if (events.window.event == SDL_WINDOWEVENT_CLOSE)
                    run = SDL_FALSE;
                break;
            case SDL_MOUSEBUTTONDOWN :
                if (events.button.button == SDL_BUTTON_LEFT ){
                    tot_key = tot_key | KEY_ATT;
                }
                break;
            case SDL_MOUSEBUTTONUP :
                if (events.button.button == SDL_BUTTON_LEFT ){
                    tot_key = tot_key ^ KEY_ATT;
                }
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
                if(events.key.keysym.sym == SDLK_p){
                    tot_key = tot_key | KEY_HB;
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
                if(events.key.keysym.sym == SDLK_p){
                    tot_key = tot_key ^ KEY_HB;
                }
            }
        }

        SDL_RenderClear(ren);
        SDL_RenderCopy(ren,bgTexture,NULL,NULL);
        Tom->afficher_chunk(ren,(entite_t*)Tom,WINH,WINW);

        render_chunk_unite(tableau_entite,ren,pontTexture,murTexture,Tom->salle ,Tom->chunk,WINW,WINH);
        if(KEY_HB & tot_key){
            Tom->hitbox(ren,Tom,WINH,WINW);
            hitbox_tableau(ren,tableau_entite,WINW,WINH);
            hitbox_tableau(ren,tableau_attaque,WINW,WINH);
        }
        
        SDL_RenderPresent(ren);
        
    }
    Tom->detruire(&Tom);
    vider_tableaux(tableau_entite,tab_destr);
    vider_attaque(tableau_attaque);
    niv->detruire(&niv);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(murTexture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    fclose(index);
    fclose(entite_gen);
    return 0;
}