main_fich= main.o room_rendering.o  unite.o porte.o chunk.o salle.o niveau.o entite.o personnage.o element_generation.o attaque.o ennemi.o

BIN= main
OBJ=  main.o room_rendering.o  unite.o porte.o chunk.o salle.o niveau.o entite.o personnage.o element_generation.o attaque.o ennemi.o

main: $(main_fich)
	gcc $(main_fich) -o main -lSDL2main -lSDL2 -lSDL2_image -lm


room_rendering.o: src/affichage/room_rendering.c lib/affichage/room_rendering.h
	gcc src/affichage/room_rendering.c -c

attaque.o: src/entite/attaque.c lib/entite/attaque.h
	gcc src/entite/attaque.c -c

ennemi.o: src/entite/ennemi.c lib/entite/ennemi.h 
	gcc src/entite/ennemi.c -c

personnage.o: src/entite/personnage.c lib/entite/personnage.h
	gcc src/entite/personnage.c -c

entite.o: src/entite/entite.c lib/entite/entite.h
	gcc src/entite/entite.c -c

unite.o: src/niveaux/unite.c lib/niveaux/unite.h
	gcc src/niveaux/unite.c -c

chunk.o: src/niveaux/chunk.c lib/niveaux/chunk.h
	gcc src/niveaux/chunk.c -c

salle.o: src/niveaux/salle.c lib/niveaux/salle.h
	gcc src/niveaux/salle.c -c

porte.o: src/niveaux/porte.c lib/niveaux/porte.h
	gcc src/niveaux/porte.c -c

niveau.o: src/niveaux/niveau.c lib/niveaux/niveau.h
	gcc src/niveaux/niveau.c -c

element_generation.o: src/generation/element_generation.c lib/generation/element_generation.h
	gcc src/generation/element_generation.c -c

all: $(OBJ) $(BIN) clean

clean: 
	rm *.o