/* Par Matthis */

#include "stdio.h"
#include "SDL2/SDL.h"
#include "lib/entite/personnage.h"
#include "lib/affichage/room_rendering.h"
#include "lib/niveaux/niveau.h"
#include "lib/generation/element_generation.h"

int menu_accueil(){//menu d'accueil du jeu

    SDL_Event event;
    //création de toute les potentielles surfaces
    int WINW=1280, WINH=720;
    SDL_Window * win =NULL;
    SDL_Renderer * ren =NULL;
    SDL_Texture * affichage=NULL;
    SDL_Texture * menu[3];

    int continuer = 1;
    int i = 0;

    if(SDL_Init(SDL_INIT_VIDEO)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
    }
    if(SDL_CreateWindowAndRenderer(WINW, WINH,SDL_WINDOW_SHOWN,&win, &ren)<0){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur : %s", SDL_GetError());
        SDL_Quit();
    }

    SDL_Texture * Texture_menu=creer_texture_image(ren,"graphics/menu/menu.png");
    SDL_Texture * Texture_menu_jouer=creer_texture_image(ren,"graphics/menu/jouer.png");
    SDL_Texture * Texture_menu_quit=creer_texture_image(ren,"graphics/menu/quit.png");
        //SDL_WM_SetCaption("Roguelike", NULL);

    menu[0] = Texture_menu;
    menu[1] = Texture_menu_jouer;
    menu[2] = Texture_menu_quit;

    affichage = menu[0];
    int retour=0;
    while(continuer){

        //contrôle par clavier
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE){
                    retour =1;
                    continuer = 0;
                }
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_z) 
                    i=(i+1)%3;

                if(event.key.keysym.sym == SDLK_s){
                    i=(i-1)%3;
                    i=i<0 ? 2 : i;
                }
                if(event.key.keysym.sym == SDLK_SPACE){
                    if(affichage == menu[1]){
                        continuer = 0;
                        break;
                    }
                    else if(affichage == menu[2]){    
                        retour=1;
                        SDL_DestroyTexture(Texture_menu);
                        SDL_DestroyTexture(Texture_menu_jouer);
                        SDL_DestroyTexture(Texture_menu_quit);
                        SDL_DestroyTexture(affichage);
                        SDL_DestroyRenderer(ren);
                        SDL_DestroyWindow(win);
                        SDL_Quit();
                        exit(1); 
                    }
                }
                break;


            default: break;
        }
        //affichage et raffraichissement
        SDL_RenderClear(ren);
        affichage=menu[i];
        SDL_RenderCopy(ren, affichage, NULL, NULL);
        SDL_RenderPresent(ren);
    }
    SDL_DestroyTexture(Texture_menu);
    SDL_DestroyTexture(Texture_menu_jouer);
    SDL_DestroyTexture(Texture_menu_quit);
    SDL_DestroyTexture(affichage);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return retour;
}

int main(){

    if(menu_accueil())
        return 0;

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
    char appel[]="./";
    float sec,secAvant, secMaint;


    index=fopen("generation/index_entite.txt", "r");
    if(!index){
        printf("Pas d'index\n");
    }
    
    entite_gen =fopen("generation/generation_ent.txt","r");
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
    niv=niveau_creer("generation/niveau_gen.txt");
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
    SDL_Texture * bgTexture_des=creer_texture_image(ren,"graphics/texture/room_textures/fond haricot.png");
    SDL_Texture * bgTexture_air=creer_texture_image(ren,"graphics/texture/room_textures/dessus.png");
    SDL_Texture * murTexture=creer_texture_image(ren, "graphics/texture/room_textures/mur.png");
    SDL_Texture * pontTexture=creer_texture_image(ren, "graphics/texture/room_textures/pont.png");

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
    perso_control[roulette]->change_chunk(ren, perso_control[roulette], tableau_entite,tab_destr,tableau_attaque,index,entite_gen,appel);
    while(run){

        if(perso_control[roulette]->vie <=0){
            tot_key= tot_key | KEY_CHANGE_L;
        }

        if(KEY_CHANGE_L & tot_key || KEY_CHANGE_R & tot_key){
            i=roulette;
            do{
                roulette+= (KEY_CHANGE_L & tot_key ? -1 : 1);
                roulette = roulette % NB_PERSO;
                if(roulette < 0)
                    roulette = NB_PERSO-1;
            }while(perso_control[roulette]==NULL && roulette != i);
            
            
            
            if(roulette != i){
                perso_control[i]->copie_partiel(perso_control[roulette], perso_control[i]);
                perso_control[i]->depop(perso_control[i]);
            }
            if(perso_control[i]->vie <=0){
                perso_control[i]->detruire(&(perso_control[i]));
                perso_control[i]=NULL;
            }
            
        }

        if(perso_control[roulette] == NULL){
            break;
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

        render_chunk_unite(tableau_entite,tableau_attaque,ren,pontTexture,murTexture,perso_control[roulette]->salle ,perso_control[roulette]->chunk,WINW,WINH);
        if(KEY_HB & tot_key){
            perso_control[roulette]->hitbox(ren,(entite_t*)(perso_control[roulette]),WINH,WINW);
            hitbox_tableau(ren,(void**)tableau_entite,WINW,WINH);
            hitbox_tableau(ren,(void**)tableau_attaque,WINW,WINH);
        }
        perso_control[roulette]->afficher_chunk(ren,perso_control[roulette],WINH,WINW);
        SDL_RenderPresent(ren);
        
    }

    for(roulette=0; roulette<NB_PERSO;roulette++){
        if(perso_control[roulette]!=NULL)
            perso_control[roulette]->detruire(&(perso_control[roulette]));
    }
    
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