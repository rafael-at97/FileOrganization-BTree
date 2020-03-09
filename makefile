all: arvoreB clean

arvoreB: arvoreB.o filesio.o tads.o btree.o
	g++ -Wall -ansi -std=c++98 arvoreB.o filesio.o tads.o btree.o -o arvoreB	

arvoreB.o: arvoreB.cpp
	g++ -Wall -ansi -std=c++98 arvoreB.cpp -c -o arvoreB.o

filesio.o: filesio.cpp filesio.h
	g++ -Wall -ansi -std=c++98 filesio.cpp -c -o filesio.o
	
tads.o: tads.cpp tads.h
	g++ -Wall -ansi -std=c++98 tads.cpp -c -o tads.o	

btree.o: btree.cpp btree.h
	g++ -Wall -ansi -std=c++98 btree.cpp -c -o btree.o
	
.PHONY: clean

clean:		
	rm -f *.o
