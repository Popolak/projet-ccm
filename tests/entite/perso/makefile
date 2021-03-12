fichier_test_perso= perso_test.o personnage.o

BIN= test_perso
OBJ= perso_test.o personnage.o

test_perso: $(fichier_test_perso)
	gcc $(fichier_test_perso) -o test_perso

personnage.o: ../../../src/personnages/personnage.c ../../../lib/personnages/personnage.h
	gcc ../../src/personnages/personnage.c -c

perso_test.o: perso_test.c
	gcc perso_test.c -c

all: $(OBJ) $(BIN) clean

clean: 
	rm *.o