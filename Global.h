#pragma once
#include "Box.h"
#include "mkausch.h"
#include "hzhang.h"
#include "dtorres.h"
#include <cstring>
#include "aparriott.h"


enum Gamestate { SPLASH, MAINMENU, GAME, PAUSE, GAMEOVER };
enum Substate { NONE, ENTITY, HIGH_SCORES, SETTINGS, DTORRES, HUAIYU, MIKE };



class Global {
public:


// vars
int xres, yres;
Gamestate state;
Substate substate;
int n_Bullet;
// how may bread(enamy) we have
int n_Bread;
int BreadCD=30;
int BulletCD=5;
char keys[65536];
bool show_help_menu;
int levelchance;
Timer gameTimer;



Global();
~Global();
void gameReset();

};



// Global Objects declared here:
// Note: use the inline keyword so that only one var is created acrossed
//              all files


inline Global g;

inline std::string mm_text[] = {"Start Game",
                                "High Scores",
                                "Settings",
                                "Quit"};
inline Menu mm(4, 225, 225, g.xres/2.0f, g.yres/2.0f, mm_text);

inline std::string p_text[] = {"Back to Game",
                                "Start Over",
                                "Main Menu",
                                "Quit Game"};
inline Menu pause_menu(4, 225, 225, g.xres/2.0f, g.yres/2.0f, p_text);

inline Blocky blocky;
inline HealthBar blocky_health(blocky, g.xres/2.0f, 15.0f);
inline InfoBoard info_board_1;
inline FreezeBlock freeze_block;
#ifdef USE_OPENAL_SOUND
inline Sound sounds;

#endif

// Huaiyu veribles================================
const int MAX_bullet = 10000;
const int MAX_bread = 10;
inline Gamerecord record;
inline Toaster tos;
inline HealthBar tos_health(tos, g.xres/2.0f, 40.0f);   // added his health bar *<|8^)
inline Bullet bul[MAX_bullet];
inline Bread bread[MAX_bread];

// Huaiyu veribles================================
