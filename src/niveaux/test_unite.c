#include <stdio.h>
#include "unite.h"

int main(){
    unite_t * unite=NULL;
    printf("Test d'existence:\n");
    unite_existe(unite)?printf("-->OK"):printf("-->KO");
    printf("Creation :\n");
    unite=unite_creer(VIDE,0,0);
    unite_existe(unite)?printf("-->OK\n"):printf("-->KO\n");
    printf("Contenu : %d\n", unite->lire(unite));
    unite->ecrire(&unite,JOUEUR);
    printf("Contenu : %d\n", unite->lire(unite));
    unite_afficher_ref();
    printf("\nTest destruction : \n");
    unite->detruire(&unite)? printf("-->KO\n"):printf("-->OK\n");
    unite_afficher_ref();printf("\n");
    return 0;
}