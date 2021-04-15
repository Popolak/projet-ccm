#include <stdio.h>
#include <stdlib.h>
#include "../../lib/entite/ennemi.h"

int main(){

  ennemi_t * courjette_test = NULL;

  pos_t position;
  position.x = 50;
  position.y = 50;

  printf("test creation\n");
  courjette_test = ennemi_creer("courjette","courjette", "cac", 50, 10, 15, 12, 0, 0,&position, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"courjette.png");

  printf( "Test d'existance sur un perso_t existant\n" ) ;
  courjette->ennemi_existe(courjette_test);

  printf("test destruction\n");
  courjette->ennemi_detruire(&courjette_test);

  printf( "Test d'existance sur un perso_t inexistant\n" ) ;
  courjette->ennemi_existe(courjette_test);

  return 0;