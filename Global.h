#pragma once
#include "Box.h"


enum Gamestate { MAINMENU, GAME, PAUSE, GAMEOVER };
class Global {

public:

// vars
int xres, yres;
Gamestate state;


Global();

};

// Global Objects declared here:
// Note: you use extern keyword to declare them when the header file is included
//         ~~ Their definition is in cpp file ~~
//              - This is how you make them accessable in other files without
//                      causing them to be defined twice.


inline Global g;
inline Box s_menu_bg;