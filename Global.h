#pragma once
#include "Box.h"
#include "Menu.h"


enum Gamestate { MAINMENU, GAME, PAUSE, GAMEOVER };


class Global {
public:

// vars
int xres, yres;
Gamestate state;

Global();

};




// Global Objects declared here:
// Note: use the inline keyword so that only one var is created acrossed
//              all files


inline Global g;
inline Box s_menu_bg;

inline std::string mm_text[] = {"Start 1P Game", "Start 2P Game", "Settings"};
inline Menu mm(3, 120, 120, g.xres/2.0f, g.yres/2.0f, mm_text);