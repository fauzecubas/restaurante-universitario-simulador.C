all: 
	gcc -c libfila.c -o libfila.o
	gcc -c liblista.c -o liblista.o
	gcc -c libpilha.c -o libpilha.o
	
	gcc -Wall -std=c90 libfila.o liblista.o libpilha.o ru_na_pandemia.c -o ru_na_pandemia
	
clean:
	rm -f *.o *.gch
