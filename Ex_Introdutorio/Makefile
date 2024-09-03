all: programa

main.o: main.c funcoes.h funcoesFornecidas.h
	gcc -o main.o main.c -c -W -Wall -ansi -pedantic

funcoes.o: funcoes.c funcoes.h
	gcc -o funcoes.o funcoes.c -c -W -Wall -ansi -pedantic

funcoesFornecidas.o: funcoesFornecidas.c funcoesFornecidas.h
	gcc -o funcoesFornecidas.o funcoesFornecidas.c -c -W -Wall -ansi -pedantic

programa: main.o funcoes.o funcoesFornecidas.o
	gcc -o programa main.o funcoes.o funcoesFornecidas.o

clean:
	rm -rf *.o *~ programa