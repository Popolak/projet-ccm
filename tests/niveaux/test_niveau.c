/* Par Matthis */

#include <stdio.h>
#include "../../lib/niveaux/niveau.h"

int main(){
    niveau_t * niveau =NULL;
    printf("Test d'existence:\n");
    niveau_existe(niveau)?printf("-->OK"):printf("-->KO");
    printf("\nCreation :\n");
    niveau= niveau_creer("../../src/niveaux/generation/test_niv.txt");
    printf("%p\n", niveau);
    niveau_existe(niveau)?printf("-->OK\n"):printf("-->KO\n");
    printf("\n");
    niveau->lire(niveau);
    unite_afficher_ref();
    chunk_afficher_ref();
    salle_afficher_ref();
    niveau_afficher_ref();
    printf("\nTest destruction : \n");
    niveau->detruire(&niveau)? printf("-->KO\n"):printf("-->OK\n");
    unite_afficher_ref();
    chunk_afficher_ref();
    salle_afficher_ref();
    niveau_afficher_ref();printf("\n");
    return 0;
}