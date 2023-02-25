#pragma once
#include "Box.h"
#include "mkausch.h"
#include "hzhang.h"
#include <cstring>

enum Gamestate { SPLASH, MAINMENU, GAME, PAUSE, GAMEOVER };
enum Substate { NONE, ENTITY, SETTINGS };



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





Global();
~Global();

};



// Global Objects declared here:
// Note: use the inline keyword so that only one var is created acrossed
//              all files


inline Global g;
// inline Box s_menu_bg;

inline std::string mm_text[] = {"Start Game",
                                "High Scores",
                                "Settings",
                                "Quit"};
inline Menu mm(4, 120, 120, g.xres/2.0f, g.yres/2.0f, mm_text);

inline std::string p_text[] = {"Back to Game",
                                "Start Over",
                                "Main Menu",
                                "Quit Game"};
inline Menu pause_menu(4, 120, 120, g.xres/2.0f, g.yres/2.0f, p_text);


// Huaiyu veribles================================
const int MAX_bullet = 10000;
const int MAX_bread = 10;
// inline Global g;
inline Toaster tos;
inline Bullet bul[MAX_bullet];
inline Bread bread[MAX_bread];
//make a bullet need the pos of where this bullet creat
//bool(tb) is means this bullet from toster or not
//type is id of Bullet_type
void make_Bullet(float x, float y,float z, bool tb, int type);
// {
//   if (g.n_Bullet < MAX_bullet){
//       bul[g.n_Bullet].set_Bullet(x, y, z, tb, type);
//       ++g.n_Bullet;
//       // cout<<"make bullet"<<endl;
//     }
//
// }

//make a bread need the pos of where this bread creat
//int Bread_t is the type of bread
//type is id of Bullet_type this bread carrying
void make_Bread(float x, float y,float z, int Bread_t, int type);
// {
//   if (g.n_Bread < MAX_bread){
//       bread[g.n_Bread].set_Bread(x, y, z, Bread_t, type);
//       ++g.n_Bread;
//       // cout<<"make Bread!!!"<<endl;
//     }
// }
