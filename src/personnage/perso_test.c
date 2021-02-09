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

	printf("test destruction\n");
	salade_test->detruire(&salade_test);

	printf( "Test d'existance sur un perso_t inexistant\n" ) ;
	perso_existe(salade_test);

	return 0;
}