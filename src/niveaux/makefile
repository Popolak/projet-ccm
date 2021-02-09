fich_test_unite=unite.o test_unite.o
fich_test_chunk=unite.o chunk.o test_chunk.o
fich_test_salle=unite.o chunk.o test_chunk.o porte.o salle.o
BIN= test_unite test_chunk test_salle
OBJ= unite.o chunk.o test_chunk.o test_unite.o salle.o porte.o

all: $(BIN) $(OBJ)

test_unite: $(fich_test_unite)
	gcc $(fich_test_unite) -o test_unite

test_chunk: $(fich_test_chunk)
	gcc $(fich_test_chunk) -o test_chunk

test_salle: $(fich_test_salle)
	gcc $(fich_test_salle) -o test_salle


unite.o: unite.c unite.h
	gcc unite.c -c

chunk.o: chunk.c chunk.h
	gcc chunk.c -c

salle.o: salle.c salle.h
	gcc salle.c -c

porte.o: porte.c porte.h
	gcc porte.c -c

test_unite.o: test_unite.c
	gcc test_unite.c -c

test_chunk.o: test_chunk.c 
	gcc test_chunk.c -c