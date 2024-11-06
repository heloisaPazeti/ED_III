all: lib main

lib:
	gcc -c ./src/funcoes.cpp -I ./include -o ./obj/funcoesArvoreB.o
	gcc -c ./src/funcoesAuxiliares.cpp -I ./include -o ./obj/funcoesAuxiliares.o
	gcc -c ./src/structs.c -I ./include -o ./obj/structs.o

main:
	gcc ./main.c ./obj/*.o -I ./include -o ./bin/programa

clean:
	rm ./bin/* ./obj/*

run:
	./bin/programa;
