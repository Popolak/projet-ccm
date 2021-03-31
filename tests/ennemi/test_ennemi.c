#include <stdio.h>
#include <stdlib.h>
#include "../../lib/entite/ennemi.h"

int main(){

  ennemi_t * courjette_test = NULL;

  pos_t position;
  position.x = 50;
  position.y = 50;

  printf("test creation\n");
  courjette_test = ennemi_creer("courjette", 50, 10, &position, "courjette.png", 15, 2, "CaC", 20);

  printf( "Test d'existance sur un perso_t existant\n" ) ;
  ennemi_existe(courjette_test);

  printf("test destruction\n");
  ennemi_detruire(&courjette_test);

  printf( "Test d'existance sur un perso_t inexistant\n" ) ;
  ennemi_existe(courjette_test);

  return 0;