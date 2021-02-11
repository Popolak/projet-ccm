fich_test_unite=test_unite.o unite.o
fich_test_chunk=test_chunk.o unite.o chunk.o porte.o
fich_test_salle=test_salle.o unite.o chunk.o porte.o salle.o
fich_test_niveau=test_niveau.o unite.o chunk.o porte.o salle.o niveau.o
BIN= test_unite test_chunk test_salle test_niveau
OBJ=  test_chunk.o test_unite.o test_niveau.o test_salle.o unite.o chunk.o salle.o porte.o niveau.o 

all: $(BIN) $(OBJ)

test_unite: $(fich_test_unite)
	gcc $(fich_test_unite) -o test_unite

test_chunk: $(fich_test_chunk)
	gcc $(fich_test_chunk) -o test_chunk

test_salle: $(fich_test_salle)
	gcc $(fich_test_salle) -o test_salle
test_niveau:$(fich_test_niveau)
	gcc $(fich_test_niveau) -o test_niveau

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

test_unite.o: test_unite.c
	gcc test_unite.c -c

test_chunk.o: test_chunk.c 
	gcc test_chunk.c -c

test_salle.o: test_salle.c
	gcc test_salle.c -c

test_niveau.o: test_niveau.c
	gcc test_niveau.c -c

all: $(BIN) $(OBJ) clean

clean:
	- rm *.o