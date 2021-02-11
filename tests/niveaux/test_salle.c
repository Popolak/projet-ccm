#include <stdio.h>
#include "../../lib/niveaux/salle.h"

int main(){
    salle_t * salle=NULL;
    printf("Test d'existence:\n");
    salle_existe(salle)?printf("-->OK"):printf("-->KO");
    printf("\nCreation :\n");
    salle= salle_creer("00100212");
    printf("%p\n", salle);
    salle_existe(salle)?printf("-->OK\n"):printf("-->KO\n");
    printf("\n");
    salle->lire(salle);
    salle_afficher_ref();
    printf("\nTest destruction : \n");
    salle->detruire(&salle)? printf("-->KO\n"):printf("-->OK\n");
    salle_afficher_ref();printf("\n");
    return 0;
}