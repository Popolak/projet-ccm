ennemi_test: perso.o ennemi.o
	gcc -o ennemi_test perso.o ennemi.o

perso.o: ../../src/entite/personnage.c ../../lib/entite/personnage.h 
	gcc -o perso.o -c ../../src/entite/personnage.c -c

ennemi.o: ../../src/entite/ennemi.c ../../lib/entite/ennemi.h 
	gcc -o ennemi.o -c ../../src/entite/ennemi.c -c
	