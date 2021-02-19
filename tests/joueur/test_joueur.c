#include <stdio.h>
#include <stdlib.h>
#include "../../lib/personnages/personnage.h"
#include "../../lib/joueur/joueur.h"
#include "../../SDL/include/SDL.h"

//ce test me servira aussi a tester un peu le SDL

int main(int argc, char* argv[]) 
{ 
    if (SDL_Init(SDL_INIT_VIDEO) < 0)     
    {         
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());         
        return EXIT_FAILURE;     
    }  
    SDL_Event events; 
	SDL_bool run = SDL_TRUE;
 
	perso_t * tomate = NULL;
	perso_t * carotte = NULL;
	perso_t * viande = NULL;
	creation(tomate, carotte, viande);

	SDL_Window* pWindow;       
	SDL_Renderer* pRenderer;
    pWindow = SDL_CreateWindow("SDL Programme", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    if(NULL == pWindow)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_Delay(3000);

 	while(1){

 		while (SDL_PollEvent(&events)) {
        	switch(events.type){
      	 	case SDL_WINDOWEVENT:
           		if (events.window.event == SDL_WINDOWEVENT_CLOSE)
             		run = SDL_FALSE;
            break;
        	case SDL_KEYDOWN: // Un événement de type touche enfoncée est effectué
            	SDL_Log("+key");
 				control(tomate, carotte, viande);
 			break;
 		}
 	}

    SDL_DestroyRenderer(pRenderer);     
    SDL_DestroyWindow(pWindow);     
    SDL_Quit();


 return 0;
	}
}