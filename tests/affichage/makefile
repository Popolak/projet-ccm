fich_test_affichage= test_affichage.o room_rendering.o  unite.o porte.o chunk.o salle.o niveau.o

BIN=test_affichage
OBJ= test_affichage.o room_rendering.o unite.o porte.o chunk.o salle.o niveau.o

test_affichage: $(fich_test_affichage)
	gcc $(fich_test_affichage) -o test_affichage -lSDL2main -lSDL2 -lSDL2_image

test_affichage.o: test_affichage.c
	gcc test_affichage.c -c

room_rendering.o: ../../src/affichage/room_rendering.c ../../lib/affichage/room_rendering.h
	gcc ../../src/affichage/room_rendering.c -c

unite.o: ../../src/niveaux/unite.c ../../lib/niveaux/unite.h
	gcc ../../src/niveaux/unite.c -c

chunk.o: ../../src/niveaux/chunk.c ../../lib/niveaux/chunk.h
	gcc ../../src/niveaux/chunk.c -c

salle.o: ../../src/niveaux/salle.c ../../lib/niveaux/salle.h
	gcc ../../src/niveaux/salle.c -c

porte.o: ../../src/niveaux/porte.c ../../lib/niveaux/porte.h
	gcc ../../src/niveaux/porte.c -c

niveau.o: ../../src/niveaux/niveau.c ../../lib/niveaux/niveau.h
	gcc ../../src/niveaux/niveau.c -c

clean: 
	rm *.o

all: $(OBJ) $(BIN) clean