all: lib main

lib:
	gcc -c ./src/funcoesArvoreB.c -I ./include -o ./obj/funcoesArvoreB.o
	gcc -c ./src/funcoesAuxiliares.c -I ./include -o ./obj/funcoesAuxiliares.o
	gcc -c ./src/funcoesBuscaArvore.c -I ./include -o ./obj/funcoesBuscaArvore.o
	gcc -c ./src/funcoesFornecidas.c -I ./include -o ./obj/funcoesFornecidas.o
	gcc -c ./src/funcoesInsereArvore.c -I ./include -o ./obj/funcoesInsereArvore.o
	gcc -c ./src/funcoesTrabalho1.c -I ./include -o ./obj/funcoesTrabalho1.o
	gcc -c ./src/structs.c -I ./include -o ./obj/structs.o

main:
	gcc ./main.c ./obj/*.o -I ./include -o ./bin/programa

clean:
	rm ./bin/* ./obj/*

run:
	./bin/programa;
