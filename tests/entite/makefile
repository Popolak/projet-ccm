fich_test_affichage= room_rendering.o  unite.o porte.o chunk.o salle.o niveau.o entite.o

BIN=
OBJ=room_rendering.o unite.o porte.o chunk.o salle.o niveau.o entite.o


entite.o: ../../src/entite/entite.c ../../lib/entite/entite.h
	gcc ../../src/entite/entite.c -c

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