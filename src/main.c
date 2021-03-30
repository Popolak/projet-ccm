#include <stdio.h>
#include <stdlib.h>
#include "../lib/entite/personnage.h"
#include "../lib/entite/joueur.h"
#include "../lib/entite/personnage.h"
#include "../SDL/include/SDL.h"
#include "../lib/commun.h"
#include "../lib/niveaux/niveau.h"
#include "../lib/affichage/room_rendering.h"

/*
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
}*/

/*
int main(int argc, char* argv[]){

	//menu_accueil(); // le programme continue uniquement s'il passe le menu

	int WINW=1280, WINH=720,x=CHUNKH-CHUNKH*ratioSol-30,y=TAILLE_MUR+40, nbText;
    chunk_t * chunk;
    niveau_t * niv=NULL;
    salle_t * salle=NULL;
    perso_t *Tom=NULL;
    perso_t *Carot=NULL;
    perso_t *Viande=NULL;
    perso_t *perso_control[3];
    int roulette=0;
    int i=0;
    int morts=0;
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
    SDL_Texture * bgTexture=creer_texture_image(ren,"../graphics/texture/room_textures/fond haricot.png");
    SDL_Texture * murTexture=creer_texture_image(ren, "../graphics/texture/room_textures/texture_mur.bmp");
    SDL_Texture ** joueurTextures=NULL, **entite_test_textures;

    //A ajouter: textures pour carotte et viande
    joueurTextures=creer_tableau_textures(ren,&nbText,"../graphics/sprite/personnage_sprites/Tom vrai neutre.png","../graphics/sprite/personnage_sprites/Tom marche 1.png","../graphics/sprite/personnage_sprites/Tom marche 2.png","");
    //Tomate est le premier personnage que l'on contrôle
    Tom=perso_creer("Tom","tomate",40,salle,chunk,pos,0,0,300,700,60,80,30,60,0.18,0,0,nbText,joueurTextures);
    Carot=perso_creer("Carot","carotte",30,NULL,NULL,pos,0,0,300,600,60,80,30,60,0.17,0,0,nbText,joueurTextures);
    Viande=perso_creer("Viande","viande",60,NULL,NULL,pos,0,0,300,500,60,80,30,60,0.15,0,0,nbText,joueurTextures);

    //servira au changement de perso
    perso_control[0] = Tom;
    perso_control[1] = Carot;
    perso_control[2] = Viande;

    entite_test_textures=creer_tableau_textures(ren,&nbText,"../graphics/sprite/entite_sprites/test.png","");
    printf("%d\n", nbText);
    entite_test=entite_creer("test","test",salle,chunk,pos2,200,-100,400,0.1,40,40,30,30,nbText,entite_test_textures);


    if(bgTexture==NULL || murTexture==NULL || joueurTextures[0]==NULL || joueurTextures[1]==NULL || joueurTextures[2]==NULL){
        printf("La création de la texture a échouée\n");
        SDL_DestroyTexture(murTexture);
        SDL_DestroyTexture(bgTexture);
        Tom->detruire_perso(&Tom);
        Carot->detruire_perso(&Carot);
        Viande->detruire_perso(&Viande);
        entite_test->detruire_ent(&entite_test);
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
        perso_control[roulette]->lastSprite+=sec;
        secAvant=secMaint;
        if(sec>0.001){
            perso_control[roulette]->deplacer((entite_t *)perso_control[roulette],sec);
            entite_test->deplacer(entite_test,sec);
        }
        if(perso_control[roulette]->contact((entite_t*) perso_control[roulette], entite_test) && secDeg > secInvins){
            perso_control[roulette]->prendre_coup(perso_control[roulette],10);
            printf("Le perso prend un coup %d\n", perso_control[roulette]->vie);
            if(en_vie(perso_control[roulette]) == FAUX){ //Si le personnage meurt on le remplace par le prochain dans la liste
               	morts++;
                i = roulette;
                roulette++;
                if(roulette > 2)
                	roulette = 0;
                perso_copie_partiel(perso_control[roulette], perso_control[i]);
                perso_control[roulette]->depop(perso_control[roulette]);
            }
            if(morts == 3)
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
                if (events.key.keysym.sym == SDLK_e){//changement du personnage qu'on controle 
                	if(morts<2){
                		i = roulette;
                		roulette++;
                		if(roulette > 2)
                			roulette = 0;
                		if(en_vie(perso_control[roulette] == FAUX))
                			roulette++;
                		perso_copie_partiel(perso_control[roulette], perso_control[i]);
                		perso_control[roulette]->depop(perso_control[roulette]);
                	}
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
        
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren,bgTexture,NULL,NULL);
        perso_control[roulette]->update_speed(perso_control[roulette], tot_key);
        perso_control[roulette]->afficher_chunk(ren,(entite_t*)perso_control[roulette],WINH,WINW);
        entite_test->afficher_chunk(ren, entite_test,WINH,WINW);

        perso_control[roulette]->hitbox(ren,perso_control[roulette],WINH,WINW);
        entite_test->hitbox(ren, entite_test,WINH,WINW);
        if(render_mur_chunk(ren,murTexture,perso_control[roulette]->chunk,WINW,WINH )==ERR_DEB_MEMOIRE){
            Tom->detruire_perso(&Tom);
            Carot->detruire_perso(&Carot);
            Viande->detruire_perso(&Viande);
            entite_test->detruire_ent(&entite_test);
            niv->detruire(&niv);
            SDL_DestroyTexture(bgTexture);
            SDL_DestroyTexture(murTexture);
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            SDL_Quit();
            return ERR_DEB_MEMOIRE;
        }
        SDL_RenderPresent(ren);
    }
    
    Tom->detruire_perso(&Tom);
    Carot->detruire_perso(&Carot);
    Viande->detruire_perso(&Viande);
    entite_test->detruire_ent(&entite_test);
    niv->detruire(&niv);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(murTexture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}*/

int main(){
    int WINW=1280, WINH=720,x=CHUNKH-CHUNKH*ratioSol-30,y=TAILLE_MUR+40, nbText;
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
    SDL_Texture * bgTexture=creer_texture_image(ren,"../graphics/texture/room_textures/fond haricot.png");
    SDL_Texture * murTexture=creer_texture_image(ren, "../graphics/texture/room_textures/mur 2 essai.png");
    SDL_Texture ** joueurTextures=NULL, **entite_test_textures;

    joueurTextures=creer_tableau_textures(ren,&nbText,"../graphics/sprite/personnage_sprites/Tom immo.png","../graphics/sprite/personnage_sprites/Tom neutre.png","../graphics/sprite/personnage_sprites/Tom marche 1.png","../graphics/sprite/personnage_sprites/Tom marche 2.png","");
    Tom=perso_creer("Tom","tomate",30,salle,chunk,pos,0,0,300,700,60,80,30,60,0.2,0,0,nbText,joueurTextures);

    entite_test_textures=creer_tableau_textures(ren,&nbText,"../graphics/sprite/entite_sprites/test.png","");
    entite_test=entite_creer("test","test",salle,chunk,pos2,200,-100,400,0.1,40,40,30,30,nbText,entite_test_textures);


    if(bgTexture==NULL || murTexture==NULL || joueurTextures[0]==NULL || joueurTextures[1]==NULL || joueurTextures[2]==NULL){
        printf("La création de la texture a échouée\n");
        SDL_DestroyTexture(murTexture);
        SDL_DestroyTexture(bgTexture);
        Tom->detruire_perso(&Tom);
        entite_test->detruire_ent(&entite_test);
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
        
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren,bgTexture,NULL,NULL);
        Tom->update_speed(Tom, tot_key);
        Tom->afficher_chunk(ren,(entite_t*)Tom,WINH,WINW);
        entite_test->afficher_chunk(ren, entite_test,WINH,WINW);

        Tom->hitbox(ren,Tom,WINH,WINW);
        entite_test->hitbox(ren, entite_test,WINH,WINW);
        if(render_mur_chunk(ren,murTexture,Tom->chunk,WINW,WINH )==ERR_DEB_MEMOIRE){
            Tom->detruire_perso(&Tom);
            entite_test->detruire_ent(&entite_test);
            niv->detruire(&niv);
            SDL_DestroyTexture(bgTexture);
            SDL_DestroyTexture(murTexture);
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            SDL_Quit();
            return ERR_DEB_MEMOIRE;
        }
        SDL_RenderPresent(ren);
    }
    
    Tom->detruire_perso(&Tom);
    entite_test->detruire_ent(&entite_test);
    niv->detruire(&niv);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(murTexture);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}