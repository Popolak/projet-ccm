#include <stdio.h>
#include <stdlib.h>
#include "../../../lib/entite/personnage.h"

int main(){

	perso_t * salade_test = NULL;
	chunk_t * chunk=chunk_creer(0,0,0,"000");
    pos_t position;
	position.x = 80;
	position.y = CHUNKW-TAILLE_MUR;

	printf("test creation\n");
	salade_test = perso_creer("salade","oui",80,position,80,80,80,80,2,NULL,20,20,20,2,20,chunk,NULL);

	printf( "Test d'existance sur un perso_t existant\n" ) ;
	if (perso_existe(salade_test)==VRAI)
		printf("oui\n");
	else {
		printf("non\n");
		return 1;
	}
	printf("%s\n", salade_test->nom);
	printf("%d\n", salade_test->contact_obstacle(salade_test));
	
	while(salade_test->en_vie){
        printf("test prise d'un coup pv = %i\n", salade_test->vie);
    	salade_test->prendre_coup(salade_test, 10);
        printf("pv = %i\n",salade_test->vie);
		if(salade_test->vie <= 0)
			salade_test->en_vie=FAUX;
	}
	printf("le perso est mort\n");
	if(perso_existe(salade_test)){
	printf("test destruction\n");
	salade_test->detruire(&salade_test);
	}

	printf( "Test d'existance sur un perso_t inexistant\n" ) ;
	if(perso_existe(salade_test))
		printf("oui\n");
	else
		printf("non\n");

	return 0;
}
