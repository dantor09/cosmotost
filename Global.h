#pragma once
// #include "Box.h"

enum Gamestate { MAINMENU, GAME, PAUSE, GAMEOVER };

// class Box;
class Global {

public:

// vars
int xres, yres;
Gamestate state;
// Box b1;

Global()
{
	xres = 640;
	yres = 480;
}

} ;

