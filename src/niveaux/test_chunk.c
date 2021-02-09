#include <stdio.h>
#include "chunk.h"

int main(){
    chunk_t * chunk =NULL;
    printf("Test d'existence:\n");
    chunk_existe(chunk)?printf("-->OK"):printf("-->KO");
    printf("\nCreation :\n");
    chunk= chunk_creer(0,0);
    printf("%p\n", chunk);
    chunk_existe(chunk)?printf("-->OK\n"):printf("-->KO\n");
    printf("\n");
    chunk->lire(chunk);
    chunk_afficher_ref();
    printf("\nTest destruction : \n");
    chunk->detruire(&chunk)? printf("-->KO\n"):printf("-->OK\n");
    chunk_afficher_ref();printf("\n");
    return 0;
}
