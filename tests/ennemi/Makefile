ennemi_test: perso.o ennemi.o entite.o
	gcc -o ennemi_test perso.o ennemi.o entite.o

perso.o: ../../src/entite/personnage.c ../../lib/entite/personnage.h 
	gcc -o perso.o -c ../../src/entite/personnage.c -c

ennemi.o: ../../src/entite/ennemi.c ../../lib/entite/ennemi.h 
	gcc -o ennemi.o -c ../../src/entite/ennemi.c -c

entite.o: ../../src/entite/entite.c ../../lib/entite/entite.h
	gcc -0 entite.o -c ../../src/entite/entite.c -c
	