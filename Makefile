all: run

run: main.o utilities.o navigate.o cmdMode.o copy.o goto.o rename.o delete.o search.o move.o create.o
	g++ main.o utilities.o navigate.o cmdMode.o copy.o goto.o rename.o delete.o search.o move.o create.o -o run

main.o: main.cpp
	g++ -c main.cpp

copy.o: copy.cpp
	g++ -c copy.cpp

move.o: move.cpp
	g++ -c move.cpp

create.o: create.cpp
	g++ -c create.cpp

goto.o: goto.cpp
	g++ -c goto.cpp

rename.o: rename.cpp
	g++ -c rename.cpp

delete.o: delete.cpp
	g++ -c delete.cpp

utilities.o: utilities.cpp
	g++ -c utilities.cpp

cmdMode.o: cmdMode.cpp
	g++ -c cmdMode.cpp

search.o: search.cpp
	g++ -c search.cpp

navigate.o: navigate.cpp
	g++ -c navigate.cpp

clear:
	rm -f *.o