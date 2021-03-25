main_fich= main.o room_rendering.o  unite.o porte.o chunk.o salle.o niveau.o entite.o personnage.o joueur.o

BIN= main
OBJ=  main.o room_rendering.o  unite.o porte.o chunk.o salle.o niveau.o entite.o personnage.o joueur.o

main: $(main_fich)
	gcc $(main_fich) -o main -lSDL2main -lSDL2 -lSDL2_image

room_rendering.o: affichage/room_rendering.c ../lib/affichage/room_rendering.h
	gcc affichage/room_rendering.c -c

personnage.o: entite/personnage.c ../lib/entite/personnage.h
	gcc  entite/personnage.c -c

entite.o: entite/entite.c ../lib/entite/entite.h
	gcc entite/entite.c -c

unite.o: niveaux/unite.c ../lib/niveaux/unite.h
	gcc niveaux/unite.c -c

chunk.o: niveaux/chunk.c ../lib/niveaux/chunk.h
	gcc niveaux/chunk.c -c

salle.o: niveaux/salle.c ../lib/niveaux/salle.h
	gcc niveaux/salle.c -c

porte.o: niveaux/porte.c ../lib/niveaux/porte.h
	gcc niveaux/porte.c -c

niveau.o: niveaux/niveau.c ../lib/niveaux/niveau.h
	gcc niveaux/niveau.c -c

joueur.o: entite/joueur.c ../lib/entite/joueur.h
	gcc entite/joueur.c -c

main.o: main.c
	gcc main.c -c

all: $(OBJ) $(BIN) clean

clean: 
	rm *.o