prog9.exe : hw9a_win.o parser9.o
	g++ hw9a_win.o parser9.o -o prog9

hw9a_win.o : hw9a_win.cpp parser9.h
	g++ -c hw9a_win.cpp

parser9.o : parser9.cpp parser9.h
	g++ -c parser9.cpp
	
clean :
	rm *.o
	rm *.exe