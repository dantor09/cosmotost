#pragma once
#include "Box.h"
#include "mkausch.h"
#include "hzhang.h"
#include "dtorres.h"
// #include <cstring>
#include "aparriott.h"
#include "image.h"

//                  0       1       2       3       4
enum Gamestate { SPLASH, MAINMENU, GAME, PAUSE, GAMEOVER };
enum Substate { NONE, HIGH_SCORES, SETTINGS, DEBUG };
enum Levels { LEVEL1=1, LEVEL2, LEVEL3, LEVEL4, LEVEL5, LEVEL6, LEVEL7, LEVEL8, LEVEL9 };

// removing Substates ENTITY, DTORRES, HUAIYU, MIKE and made into bools


class Global {
public:

// vars
int xres, yres, yres_start;
Gamestate state;
Substate substate;
int n_Bullet;
// how may bread(enamy) we have
int n_Bread;
int n_Spear;
int n_donut_bullet;
int n_effect;
int n_laser;
int BreadCD=30;
int BulletCD=5;
char keys[65536];
bool show_help_menu;
int levelchance;
Timer gameTimer;
Levels level;
// bools made from states
bool entity_active,
        dtorres_active,
        huaiyu_active,
        mike_active,
        donut_active;

float sfx_vol;
float m_vol;

GLuint bkg_texture;
GLuint toaster_texture;
GLuint toaster_silhouette;
GLuint bomb_texture;
GLuint icecube_texture;
// GLuint blocky_texture;
GLuint blocky_silhouette;


Global();
~Global();
void gameReset();

};



// Global Objects declared here:
// Note: use the inline keyword so that only one var is created acrossed
//              all files


inline Global g;

inline std::string mm_text[] = {"Start Game",
                                "Debug Mode",
                                "High Scores",
                                "Settings",
                                "Quit"};
inline Menu mm(5, 225, 225, g.xres/2.0f, g.yres/2.0f, mm_text);

inline std::string p_text[] = {"Back to Game",
                                "Start Over",
                                "Main Menu",
                                "Quit Game"};
inline Menu pause_menu(4, 225, 225, g.xres/2.0f, g.yres/2.0f, p_text);

inline Blocky hblocky('h', false);      // gun off horizontal
inline Blocky vblocky('v', false);      // gun off vertical
inline Blocky h2blocky('h', true);      // gun on horizontal
inline Blocky v2blocky('v', true);      // gun on vertical
inline PowerBar vblocky_health(vblocky, HEALTH, g.xres/2.0f, 15.0f);
inline PowerBar v2blocky_health(v2blocky, HEALTH, g.xres/2.0f, 15.0f);
inline PowerBar hblocky_health(hblocky, HEALTH, g.xres/2.0f, 15.0f);
inline PowerBar h2blocky_health(h2blocky, HEALTH, g.xres/2.0f, 15.0f);
inline Blocky * blocky;
inline PowerBar * blocky_health;
inline SoundBar vol_slider(&g.m_vol, g.xres*(1.0f/3.0f), g.yres/2.0f, "Music Volume");
inline SoundBar sfx_slider(&g.sfx_vol, g.xres*(2.0f/3.0f), g.yres/2.0f, "SFX Volume");
inline InfoBoard info_board_1;
inline FreezeBlock * pfreeze_block;
inline Bomb bomb;

/*******************************
 * 
 *      Textures
 * 
********************************/

inline Image background("textures/background.jpg");
inline Image toaster_img("textures/toaster.png");
inline Image bomb_img("textures/firebullet.png");
inline Image icecube_img("textures/iceSmallSpike.png");
inline Image blocky_img("textures/blocky.png");

#ifdef USE_OPENAL_SOUND
inline Sound sounds;
#endif

// Huaiyu veribles================================
const int MAX_bullet = 1000;
const int MAX_bread = 10;
const int MAX_partical = 5000;
inline Gamerecord record;
inline Toaster tos;
inline PowerBar tos_health(tos, HEALTH, g.xres/2.0f, 40.0f);   // added his health bar *<|8^)
inline PowerBar tos_cd(tos, COOLDOWN, (g.xres*2/3.0f), 40.0f);   // added his health bar *<|8^)
inline Bullet bul[MAX_bullet];
inline Bread bread[MAX_bread];
inline Spear spear[10];
inline Bullet do_bul[MAX_bullet];
// inline EffectBox effect_partical[MAX_partical];
inline DonutLaser d_laser[10];
inline Donut donut;

// Huaiyu veribles================================

// AILAND VARIABLES ============

inline EntitySpawn EntySpwn;
