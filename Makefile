
all: cosmotost

cosmotost: cosmotost.cpp Box.o Global.h
	g++ -o cosmotost cosmotost.cpp Box.o libggfonts.a -Wall -lX11 -lGL -lGLU -lm

Box.o: Box.cpp Box.h
	g++ -c Box.cpp

clean:
	rm -f cosmotost *.o

