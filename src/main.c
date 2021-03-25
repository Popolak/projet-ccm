#include <stdio.h>
#include <stdlib.h>
#include "../lib/entite/personnage.h"
#include "../lib/entite/joueur.h"
#include "../lib/entite/personnage.h"
#include "../SDL/include/SDL.h"
#include "../lib/commun.h"
#include "../lib/niveaux/niveau.h"
#include "../lib/affichage/room_rendering.h"


void menu_accueil(){//menu d'accueil du jeu

	SDL_Event event;
	//création de tout les potentielles surfaces
	SDL_Window *win =NULL;
	SDL_Renderer *ren =NULL;
	SDL_Texture *texture_menu =NULL;
	SDL_Surface *affichage =NULL;
	SDL_Surface *menu[3];

	int continuer = 1;
	int i = 0;
	int WINW=1280, WINH=720;

	SDL_Init(SDL_INIT_VIDEO);

		SDL_CreateWindowAndRenderer(WINW, WINH,SDL_WINDOW_SHOWN,&win, &ren);
		SDL_WM_SetCaption("Roguelike", NULL);

	menu[0] = IMG_Load("../graphics/menu/menu.png");
	menu[1] = IMG_Load("../graphics/menu/jouer.png");
	menu[2] = IMG_Load("../graphics/menu/quit.png");

	affichage = menu[0];

	texture_menu = SDL_CreateTextureFromSurface(ecran,affichage);
	
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
					SDL_FreeSurface(affichage);
					SDL_FreeSurface(menu);
					SDL_DestroyRenderer(&ren);
					SDL_DestroyWindow(win);
					SDL_Quit(); 
					exit(1); 
				}
			default: break;
		}
		//affichage et raffraichissement
		texture_menu = SDL_CreateTextureFromSurface(ecran,affichage);
	}
	SDL_DestroyTexture(texture_menu);
	SDL_FreeSurface(affichage);
	SDL_FreeSurface(menu);
	SDL_DestroyRenderer(&ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}


int main(int argc, char* argv[]){
	int WINW=1280, WINH=720,x=CHUNKH-CHUNKH*ratioSol-30,y=TAILLE_MUR+40;
    float i=0.1;
    chunk_t * chunk;
    niveau_t * niv=NULL;
    salle_t * salle=NULL;
    perso_t *Tom=NULL;
    entite_t * entite_test=NULL;
    SDL_Window * win=NULL;
    SDL_Renderer * ren=NULL;
    pos_t pos={300,200}, pos2={100,200};
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
    SDL_Texture * bgTexture=creer_texture_image(ren,"../graphics/texture/room_textures/fond haricot.png");
    SDL_Texture * murTexture=creer_texture_image(ren, "../graphics/texture/room_textures/texture_mur.bmp");
    SDL_Texture * joueurTexture=creer_texture_image(ren,"../graphics/sprite/personnage_sprites/Tom neutre.png");
    SDL_Texture * entite_test_texture=creer_texture_image(ren,"../graphics/sprite/entite_sprites/test.png");
    Tom=perso_creer("Tom","tomate",30,salle,chunk,pos,0,0,400,700,60,80,40,60,0,0,1,&joueurTexture);
    entite_test=entite_creer("test","test",salle,chunk,pos2,200,200,400,40,40,30,30,1,&entite_test_texture);
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
    chunk->lire_partiel(chunk,CHUNKH-CHUNKH*ratioSol-1,100,CHUNKH*ratioSol,30);
    secAvant= 1.0*SDL_GetTicks()/1000;
    secDeg=secAvant;
    int tot_key=0;


    
    while(run){
        secMaint=1.0*SDL_GetTicks()/1000;
        sec=secMaint-secAvant;
        secDeg+=sec;
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
            SDL_DestroyTexture(joueurTexture);
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
    SDL_DestroyTexture(joueurTexture);
    SDL_DestroyTexture(entite_test_texture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}