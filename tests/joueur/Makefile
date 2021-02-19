ex_joueur_test: perso.o joueur.o
	gcc -o ex_joueur_test perso.o joueur.o

perso.o: ../../src/personnages/personnage.c ../../lib/personnages/personnage.h 
	gcc -o personnage.o -c personnage.c -Wall -0

joueur.o: ../../src/personnages/joueur.c ../../lib/personnages/joueur.h 
	gcc -o joueur.o -c joueur.c -Wall -0
	