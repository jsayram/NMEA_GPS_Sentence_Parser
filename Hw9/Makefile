prog9.exe : hw9.o parser9.o
	g++ hw9.o parser9.o -o prog9

hw9.o : hw9.cpp parser9.h
	g++ -c hw9.cpp

parser9.o : parser9.cpp parser9.h
	g++ -c parser9.cpp
	
clean :
	rm *.o
	rm *.exe