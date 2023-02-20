
all: cosmotost

cosmotost: cosmotost.cpp
	g++ cosmotost.cpp -Wall -ocosmotost -lX11 -lGL -lGLU -lm

clean:
	rm -f cosmotost

