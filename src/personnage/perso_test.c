#include <stdio.h>
#include <stdlib.h>
#include <personnage.h>

int main(){

	perso_t * salade_test = NULL;

    pos_t position;
	position.x = 50;
	position.y = 50;

	printf("test creation\n");
	salade_test = perso_creer("salade", 50, 10, &position, "salade.png", 10);

	printf( "Test d'existance sur un perso_t existant\n" ) ;
	perso_existe(salade_test);

	while(en_vie(salade_test)){
        printf("test prise d'un coup pv = %i\n", salade_test->vie);
        prendre_coup(salade_test, 10);
        printf("pv = %i\n",salade_test->vie);
	}
	printf("le perso est mort\n");

	if(perso_existe(salade_test)){
	printf("test destruction\n");
	salade_test->detruire(&salade_test);
	}

	printf( "Test d'existance sur un perso_t inexistant\n" ) ;
	perso_existe(salade_test);

	return 0;
}
