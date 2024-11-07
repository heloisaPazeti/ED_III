all: lib main

lib:
	g++ -c ./src/funcoes.cpp -I ./include -o ./obj/funcoes.o
	g++ -c ./src/funcoesAuxiliares.cpp -I ./include -o ./obj/funcoesAuxiliares.o
	g++ -c ./src/structs.cpp -I ./include -o ./obj/structs.o

main:
	g++ ./main.cpp ./obj/*.o -I ./include -o ./bin/programa

clean:
	rm ./bin/* ./obj/*

run:
	./bin/programa;
