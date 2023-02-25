# add object files here and make a rule down below
OBJECTS = cosmotost.o Box.o Global.o libggfonts.a mkausch.o aparriott.o hzhang.o

FLAGS =  -Wall -lX11 -lGL -lGLU -lm
COMP = g++ -std=c++17


all: cosmotost

cosmotost: cosmotost.o Box.o Global.o mkausch.o aparriott.o dtorres.o hzhang.o
	$(COMP) -o cosmotost $(OBJECTS) $(FLAGS)

cosmotost.o: cosmotost.cpp
	$(COMP) -c cosmotost.cpp

Box.o: Box.cpp Box.h
	$(COMP) -c Box.cpp

Global.o: Global.cpp Global.h
	$(COMP) -c Global.cpp

# still in development
mkausch.o: mkausch.cpp mkausch.h
	$(COMP) -c mkausch.cpp

hzhang.o: hzhang.cpp hzhang.h
	$(COMP) -c hzhang.cpp


# spawning complete, collisions need to be done
aparriott.o: aparriott.cpp aparriott.h
	$(COMP) -c aparriott.cpp

# Empty files but development on bottom screen info soon
dtorres.o: dtorres.cpp dtorres.h
	$(COMP) -c dtorres.cpp

clean:
	rm -f cosmotost *.o

# calling make run ensures cerr is redirected to a log file
# for the purposes of debugging

run:
	./cosmotost 2> err.log
