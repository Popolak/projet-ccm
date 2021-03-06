#include "stdio.h"
#include "SDL2/SDL.h"
#include "../lib/entite/personnage.h"
#include "../lib/affichage/room_rendering.h"
#include "../lib/niveaux/niveau.h"
#include "../lib/generation/element_generation.h"

void menu_accueil(){//menu d'accueil du jeu

    SDL_Event event;
    //création de tout les potentielles surfaces
    int WINW=1280, WINH=720
    SDL_Window * win =NULL;
    SDL_Renderer * ren =NULL;
    SDL_Texture * affichage=NULL;
    SDL_Texture * menu[3];

    int continuer = 1;
    int i = 0;

    if(SDL_Init(SDL_INIT_VIDEO)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
        return 1;
    }
    if(SDL_CreateWindowAndRenderer(WINW, WINH,SDL_WINDOW_SHOWN,&win, &ren)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture * Texture_menu=creer_texture_image(ren,"../graphics/menu/menu.png");
    SDL_Texture * Texture_menu_jouer=creer_texture_image(ren,"../graphics/menu/jouer.png");
    SDL_Texture * Texture_menu_quit=creer_texture_image(ren,"../graphics/menu/quit.png");
        //SDL_WM_SetCaption("Roguelike", NULL);

    menu[0] = Texture_menu;
    menu[1] = Texture_menu_jouer;
    menu[2] = Texture_menu_quit;

    affichage = menu[0];

    SDL_Rect texture_rect;
    texture_rect.x = 0;  //the x coordinate
    texture_rect.y = 0; // the y coordinate
    texture_rect.w = WINW; //the width of the texture
    texture_rect.h = WINH;
    
    while(continuer){

        //contrôle par clavier
        SDL_WaitEvent(&event);
        switch(event.key.keysym.sym){
            case SDLK_s: 
                affichage = menu[++i];break;
            case SDLK_z:
                affichage = menu[--i];break;
            case SDLK_SPACE:
                if(affichage == menu[1]){
                    continuer = 0;
                    break;
                }
                else if(affichage == menu[2]){    
                    SDL_DestroyTexture(texture_menu);
                    SDL_DestroyTexture(Texture_menu_jouer);
                    SDL_DestroyTexture(Texture_menu_quit);
                    SDL_DestroyTexture(affichage);
                    SDL_DestroyTexture(menu);
                    SDL_DestroyRenderer(ren);
                    SDL_DestroyWindow(win);
                    SDL_Quit();
                    exit(1); 
                }
            default: break;
        }
        //affichage et raffraichissement
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, affichage, NULL, &texture_rect);
        SDL_RenderPresent(ren);
    }
    SDL_DestroyTexture(texture_menu);
    SDL_DestroyTexture(Texture_menu_jouer);
    SDL_DestroyTexture(Texture_menu_quit);
    SDL_DestroyTexture(affichage);
    SDL_DestroyTexture(menu);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
int main(){
    void * tableau_entite[NB_MAX_AFF];
    err_t (*tab_destr[NB_MAX_AFF])(void ** );
    attaque_t * tableau_attaque[NB_MAX_ATT];


    initTabDaffich(tableau_entite);
    initTabDaffich((void**)tab_destr);
    initTabDaffich((void**)tableau_attaque);


    int WINW=1280, WINH=720, nbText,n,i,roulette=0, deplacer;
    pos_t pos={CHUNKH/2, CHUNKW/2};
    perso_t * perso_control [NB_PERSO];
    chunk_t * chunk;
    niveau_t * niv=NULL;
    salle_t * salle=NULL;
    perso_t  *init=NULL;
    SDL_Window * win=NULL;
    SDL_Renderer * ren=NULL;
    FILE * index= NULL, * entite_gen= NULL;
    char appel[]="../";
    float sec,secAvant, secMaint;
    index=fopen("../generation/index_entite.txt", "r");
    if(!index){
        printf("Pas d'index\n");
    }
    
    entite_gen =fopen("../generation/generation_ent.txt","r");
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
    niv=niveau_creer("../generation/niveau_gen.txt");
    if(niv){
        printf("Génération du niveau effectuée\n");
    }
    else{
        printf("La génération de niveau n'a pas fonctionnée\n");
        return 1;
    }
    
    salle=niv->chercher_salle(niv,0,3);
    chunk=salle->chercher_chunk(salle,0,0);
    init=perso_creer("","",0,salle,chunk,pos,0,0,0,0,0,0,0,0,0,0,0,0,"",0,NULL);
    SDL_Event events;
    SDL_bool run=SDL_TRUE;
    SDL_Texture * bgTexture_des=creer_texture_image(ren,"../graphics/texture/room_textures/fond haricot.png");
    SDL_Texture * bgTexture_air=creer_texture_image(ren,"../graphics/texture/room_textures/dessus.png");
    SDL_Texture * murTexture=creer_texture_image(ren, "../graphics/texture/room_textures/mur.png");
    SDL_Texture * pontTexture=creer_texture_image(ren, "../graphics/texture/room_textures/pont.png");

    perso_control[0]=creer_entite_chaine(ren,&n,init,"tom -1 -1 0 0",index,appel);
    init->detruire(&init);
    perso_control[1]=creer_entite_chaine(ren,&n,NULL,"kurt 0 0 0 0",index,appel);
    
    
    if(bgTexture_des==NULL || murTexture==NULL || bgTexture_air==NULL || pontTexture==NULL){
        printf("La création de la texture a échouée\n");
        SDL_DestroyTexture(murTexture);
        SDL_DestroyTexture(bgTexture_des);
        SDL_DestroyTexture(bgTexture_air);
        SDL_DestroyTexture(pontTexture);
        perso_control[0]->detruire(&(perso_control[0]));
        perso_control[1]->detruire(&(perso_control[1]));
        vider_tableaux(tableau_entite,tab_destr);
        vider_attaque(tableau_attaque);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win); 
        SDL_Quit();
        return ERR_DEB_MEMOIRE;
    }

    secAvant= 1.0*SDL_GetTicks()/1000;
    int tot_key=0;
    while(run){

        if(KEY_CHANGE_L & tot_key || KEY_CHANGE_R & tot_key){
            i=roulette%NB_PERSO;
            do{
                roulette+= (KEY_CHANGE_L & tot_key ? -1 : 1);
                roulette%=NB_PERSO;
                if(roulette < 0)
                    roulette = NB_PERSO-1;
            }while(perso_control[roulette%NB_PERSO]==NULL && roulette%NB_PERSO != i);
            if (roulette%NB_PERSO==i && perso_control[i]==NULL)
                run=SDL_FALSE;
            else{
                perso_control[i]->copie_partiel(perso_control[roulette%NB_PERSO], perso_control[i]);
                perso_control[i]->depop(perso_control[i]);
            }

            if(perso_control[i]->vie <=0){
                perso_control[i]->detruire(&(perso_control[i]));
            }
            
        }


        tot_key=tot_key & (KEY_HB+KEY_LEFT+KEY_RIGHT+KEY_JUMP+KEY_ATT);
        secMaint=1.0*SDL_GetTicks()/1000;
        sec=secMaint-secAvant;
        secAvant=secMaint;
        if(sec < 0.05){
            if(  perso_control[roulette]->update_speed(perso_control[roulette],NULL,tot_key)){
                perso_control[roulette]->nouvelle_attaque(ren,tableau_attaque,NULL,perso_control[roulette],index,appel);
            }
            deplacer=perso_control[roulette]->deplacer(perso_control[roulette],sec);
            if(deplacer==1){
                perso_control[roulette]->change_chunk(ren, perso_control[roulette], tableau_entite,tab_destr,tableau_attaque,index,entite_gen,appel);
            }
            else if(deplacer==2){
                perso_control[roulette]->change_salle(perso_control[roulette]);
                perso_control[roulette]->change_chunk(ren,perso_control[roulette],tableau_entite,tab_destr,tableau_attaque,index,entite_gen,appel);      
            }
            else{
                synchro_attaque(tableau_attaque,sec);
                update_ennemis_input(tableau_entite,perso_control[roulette]);
                tableau_agit(ren,tableau_entite,tab_destr,perso_control[roulette],index,appel);
                synchro_tableau(tableau_entite,tab_destr,sec,NULL);
                contact_attaque_ennemis(ren,tableau_attaque,tableau_entite,tab_destr,index,appel);
            }
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
                if(events.key.keysym.sym == SDLK_e){
                    tot_key = tot_key | KEY_CHANGE_R;
                }
                if(events.key.keysym.sym == SDLK_a){
                    tot_key = tot_key | KEY_CHANGE_L;
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
        render_background(ren,perso_control[roulette]->salle,perso_control[roulette]->chunk,bgTexture_des,bgTexture_air,WINW,WINH);
        perso_control[roulette]->afficher_chunk(ren,(entite_t*)perso_control[roulette],WINH,WINW);

        render_chunk_unite(tableau_entite,tableau_attaque,ren,pontTexture,murTexture,perso_control[roulette]->salle ,perso_control[roulette]->chunk,WINW,WINH);
        if(KEY_HB & tot_key){
            perso_control[roulette]->hitbox(ren,perso_control[roulette],WINH,WINW);
            hitbox_tableau(ren,tableau_entite,WINW,WINH);
            hitbox_tableau(ren,tableau_attaque,WINW,WINH);
        }
        
        SDL_RenderPresent(ren);
        
    }
    perso_control[roulette]->detruire(&perso_control[roulette]);
    vider_tableaux(tableau_entite,tab_destr);
    vider_attaque(tableau_attaque);
    niv->detruire(&niv);
    SDL_DestroyTexture(bgTexture_des);
    SDL_DestroyTexture(bgTexture_air);
    SDL_DestroyTexture(murTexture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    fclose(index);
    fclose(entite_gen);
    return 0;
}