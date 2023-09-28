program.exe: main.o Node.o Stack.o List.o Display.o Chip8.o 
	g++ -o program.exe main.o Node.o Stack.o List.o Display.o Chip8.o -Isrc/include -Lsrc/lib -lmingw32 -lSDL2main -lSDL2

main.o: main.cpp
	g++ -c main.cpp -Isrc/include -Lsrc/lib -o main.o

Node.o: Node.cpp Node.h
	g++ -c Node.cpp -o Node.o

Stack.o: Stack.cpp Stack.h
	g++ -c Stack.cpp -o Stack.o

List.o: List.cpp List.h
	g++ -c List.cpp -o List.o

Display.o: Display.cpp Display.h
	g++ -c Display.cpp -Isrc/include -Lsrc/lib -o Display.o

Chip8.o: Chip8.cpp Chip8.h
	g++ -c Chip8.cpp -Isrc/include -Lsrc/lib -o Chip8.o

clear:
	del *.o program.exe