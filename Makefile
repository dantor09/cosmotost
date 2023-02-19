# add object files here and make a rule down below
OBJECTS = cosmotost.o Box.o Global.o libggfonts.a

FLAGS =  -Wall -lX11 -lGL -lGLU -lm
COMP = g++ -std=c++17 


all: cosmotost

cosmotost: cosmotost.o Box.o Global.o
	$(COMP) -o cosmotost $(OBJECTS) $(FLAGS)

cosmotost.o: cosmotost.cpp
	$(COMP) -c cosmotost.cpp

Box.o: Box.cpp Box.h
	$(COMP) -c Box.cpp

Global.o: Global.cpp Global.h
	$(COMP) -c Global.cpp

# still in development
# Menu.o: Menu.cpp Menu.h
# 	$(COMP) -c Menu.cpp

clean:
	rm -f cosmotost *.o

# calling make run ensures cerr is redirected to a log file
# for the purposes of debugging

run:
	./cosmotost 2> err.log
