# add object files here and make a rule down below
OBJECTS = cosmotost.o Box.o Global.o libggfonts.a mkausch.o aparriott.o hzhang.o dtorres.o image.o
SOBJECTS = s_cosmotost.o Box.o Global.o libggfonts.a s_mkausch.o aparriott.o hzhang.o dtorres.o image.o

FLAGS =  -Wall -lX11 -lGL -lGLU -lm
SSHFLAGS = -Wall -lssl -lcrypto 
COMP = g++ -std=c++17


all: cosmotost s_cosmotost cosmoconnect

cosmotost: cosmotost.o Box.o Global.o mkausch.o aparriott.o dtorres.o hzhang.o image.o
	$(COMP) -o cosmotost $(OBJECTS) $(FLAGS)

cosmotost.o: cosmotost.cpp
	$(COMP) -c cosmotost.cpp

# making sound executable
s_cosmotost: s_cosmotost.o Box.o Global.o s_mkausch.o aparriott.o dtorres.o hzhang.o image.o
	$(COMP) -o s_cosmotost $(SOBJECTS) $(FLAGS) \
			-D USE_OPENAL_SOUND \
			/usr/lib/x86_64-linux-gnu/libopenal.so \
			/usr/lib/x86_64-linux-gnu/libalut.so 2>sound_compilation.log

s_cosmotost.o: cosmotost.cpp
	$(COMP) -c cosmotost.cpp  -os_cosmotost.o\
			-D USE_OPENAL_SOUND

s_mkausch.o: mkausch.cpp mkausch.h Box.h Global.h
	$(COMP) -c mkausch.cpp  -os_mkausch.o\
			-D USE_OPENAL_SOUND \
			
image.o: image.cpp image.h
	$(COMP) -c image.cpp

Box.o: Box.cpp Box.h Global.h Global.cpp
	$(COMP) -c Box.cpp

Global.o: Global.cpp Global.h mkausch.h hzhang.h image.cpp image.h
	$(COMP) -c Global.cpp

mkausch.o: mkausch.cpp mkausch.h Box.h Global.h
	$(COMP) -c mkausch.cpp

hzhang.o: hzhang.cpp hzhang.h mkausch.cpp
	$(COMP) -c hzhang.cpp

# timing of state needs to be done
aparriott.o: aparriott.cpp aparriott.h Global.h mkausch.cpp
	$(COMP) -c aparriott.cpp mkausch.cpp

# Empty files but development on bottom screen info soon
dtorres.o: dtorres.cpp dtorres.h mkausch.cpp
	$(COMP) -c dtorres.cpp

cosmoconnect: cosmoconnect.cpp
	$(COMP) cosmoconnect.cpp $(SSHFLAGS) -o cosmoconnect 

clean:
	rm -f cosmotost s_cosmotost *.o 2>/dev/null

# calling make run ensures cerr is redirected to a log file
# for the purposes of debugging

run:
	./cosmotost 2> err.log
