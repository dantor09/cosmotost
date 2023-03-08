# add object files here and make a rule down below
OBJECTS = cosmotost.o Box.o Global.o libggfonts.a mkausch.o aparriott.o hzhang.o
SOBJECTS = s_cosmotost.o Box.o Global.o libggfonts.a s_mkausch.o aparriott.o hzhang.o

FLAGS =  -Wall -lX11 -lGL -lGLU -lm
COMP = g++ -std=c++17


all: cosmotost s_cosmotost

cosmotost: cosmotost.o Box.o Global.o mkausch.o aparriott.o dtorres.o hzhang.o
	$(COMP) -o cosmotost $(OBJECTS) $(FLAGS)

cosmotost.o: cosmotost.cpp
	$(COMP) -c cosmotost.cpp

# making sound executable
s_cosmotost: s_cosmotost.o Box.o Global.o s_mkausch.o aparriott.o dtorres.o hzhang.o
	$(COMP) -o s_cosmotost $(SOBJECTS) $(FLAGS) \
			-D USE_OPENAL_SOUND \
			/usr/lib/x86_64-linux-gnu/libopenal.so \
			/usr/lib/x86_64-linux-gnu/libalut.so

s_cosmotost.o: cosmotost.cpp
	$(COMP) -c cosmotost.cpp  -os_cosmotost.o\
			-D USE_OPENAL_SOUND

s_mkausch.o: mkausch.cpp mkausch.h Box.h Global.h
	$(COMP) -c mkausch.cpp  -os_mkausch.o\
			-D USE_OPENAL_SOUND \
			

Box.o: Box.cpp Box.h Global.h Global.cpp
	$(COMP) -c Box.cpp

Global.o: Global.cpp Global.h mkausch.h hzhang.h
	$(COMP) -c Global.cpp

# still in development
mkausch.o: mkausch.cpp mkausch.h Box.h Global.h
	$(COMP) -c mkausch.cpp


hzhang.o: hzhang.cpp hzhang.h
	$(COMP) -c hzhang.cpp

# spawning complete, collisions need to be done
aparriott.o: aparriott.cpp aparriott.h Global.h mkausch.cpp
	$(COMP) -c aparriott.cpp mkausch.cpp

# Empty files but development on bottom screen info soon
dtorres.o: dtorres.cpp dtorres.h
	$(COMP) -c dtorres.cpp

clean:
	rm -f cosmotost s_cosmotost *.o 2>/dev/null

# calling make run ensures cerr is redirected to a log file
# for the purposes of debugging

run:
	./cosmotost 2> err.log
