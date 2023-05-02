/********************************************************************
*
*		Program: mkausch.h
*		Authors: Michael Kausch (Group 1 - DAHM)
*		Date Code Originated: 2/17/23
*
*       Classes: Manu
*       Uses:
*           Creates a menu object based on num text params, size and position
**********************************************************************/

//                 		INCLUDES

#pragma once
#include <GL/glx.h>
#include <iostream>
#include <new>
#include <sstream>
#include <chrono>
#include <time.h>
#include <cstdlib>
#include <list>
#include "Box.h"
#include "fonts.h"
#include "hzhang.h"

#define SUB_BLOCK_N 150
#define NUM_BLOCKY_BULLETS 5

using namespace std;

inline const int NUM_SOUNDS = 16;
// inline const int NUM_SONGS = 1;

enum PBType {HEALTH, COOLDOWN, DONUT};

// #define USE_OPENAL_SOUND

#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

// const int U_TO_S = 1000000;
// const int SONG_LENGTH = U_TO_S * ((4 * 60) + 30);

class Menu
{

public:

    // vars
    Box boarder;
    Box mainbox;
    //float w, h, bw, bh; // width, height, boarder width, boarder height
    float pos[3];
    unsigned char color[3];
    unsigned char bcolor[3];
    unsigned int n_texts;
    Rect *texts;
    // Rect texts[4];
    Box *t_boxs;
    std::string *words;
    int centering;

    // constructors
    Menu(unsigned int n_texts,
            float _w, float _h,
            float _x, float _y,
            std::string* _words,
            int _centering = 1);
    ~Menu();

    // setters
    // void setBcolor(int r, int g, int b);   // sets boarder color
    void setColor(int r, int g, int b);   // sets mainbox color
    void setPos(float x, float y, float z);
    void setHighlight(Box * b);
    void setOrigColor();

    // getters
    // unsigned char * get_tcolor();
    // std::string getInfo(); // get debug info

    // other
    void draw();
    Box* checkTBox(int x, int y);
} ;

class Timer
{

private:

// variables
std::chrono::time_point<std::chrono::system_clock> start;
double duration;

bool paused;
Timer * pause_timer;
double pause_duration;

public:

Timer();    // used to create a timer that counts up or dc about it expiring
            // note: the isDone() function is pretty useless and you should
            //      be looking at getTime to see how long the timer has gone
            //      for
Timer(double sec);    // used to create a timer that counts down and will tell
                    // you if it expires
~Timer();

// setters
void reset();
void pause();
void unPause();

// getters
int getTime(char);
bool isDone();
bool isPaused();

};

#ifdef USE_OPENAL_SOUND
class Sound
{
private:

    ALuint alBuffers[NUM_SOUNDS];
	ALuint alSources[NUM_SOUNDS];

    ALuint menuQueueSource;
    ALuint songQueueSource;
    ALint buffersDone;
    ALint buffersQueued;

    bool is_intro;
    bool is_game;
    bool is_music_paused;
    bool user_pause;
    int current_track;

    void initOpenal();
    void closeOpenal();
    string buildSongPath(string s);

    string sound_names[NUM_SOUNDS] = {
                    "bullet_fire.wav",
                    "Edzes-64TheMagicNumber-intro8kHz.wav",
                    "beep.wav",
                    "boop.wav",
                    "Edzes-64TheMagicNumber-loop8kHz.wav",
                    "AdhesiveWombat-8bitAdventure_16kHz.wav",
                    "zap1.wav",
                    "zap2.wav",
                    "zap3.wav",
                    "zap4.wav",
                    "shield.wav",
                    "doosh.wav",
                    "doosh2.wav",
                    "explo3.wav",
                    "explo4.wav",
                    "zap2.wav"};


public:
	// Source refers to the sound.
    bool gun_shooting;
    float m_vol;
    float sfx_vol;

	Sound();
	~Sound();
    void playStartTrack();
    bool checkIntroBufferDone();
    void resetBufferDone();
    void loopIntro();
    void setupGameMode();
    string getSongName();
    void pause();
    void unpause();
    void toggleUserPause();
    bool getPause();
    void rewindGameMusic();
    void gunPlay(int btype);
    void gunStop();
    void doosh(int i = 0);
    void shieldSFX();
    void exploSFX();
    void beep();
    void boop();
    void updateMusicVol();
    void updateSFXVol();
    void playZap2();
    void bombExplosion();

};

#endif

class PowerBar
{
private:
    Box total;     // a box that is proportionate to the overall hp of the Item pointed to by itm
    Box health;     // a box that is proportionate to the size of the current hp of itm
    Rect text;
    PBType type;
    const Toaster * tos;   // item that this healthbar is attached to
    const Item * itm;   // item that this healthbar is attached to
    const Donut * donut;    // item that this healthbar is attached to
    // void hp_resize();   // resizes hp box based on passed on item's health

public:
    PowerBar(const Toaster & _tos_, PBType type, float x, float y);
    PowerBar(const Item & _itm_, PBType type, float x, float y);
    PowerBar(const Donut & _itm_, PBType _type_, float x, float y);

    
    void draw();
};


class Blocky : public Item
{
private:
    // void setRandColor();
    void setRandPosition();
    void initRotationVel();
                        // so it doesn't continuously damage it
    Item sub_boxes[SUB_BLOCK_N];
    int sb_angles[SUB_BLOCK_N];
    int rot_speed[SUB_BLOCK_N];
    int rot_angle[SUB_BLOCK_N];
    list<Bullet> bullets;
    double delay_t;


public:
    bool was_hit; // set when the block strikes the toaster that fall
    int point;
    int bul_point;
    bool gun_active;

    // int lives;
    Blocky(char type, bool gun_active);
    ~Blocky();
    void reset(); // tests to see if the player killed poor forky
    void draw();    // overload function to include redraw
    void move();
    bool isAlive();
    bool didDamage();
    void setHit();
    void explode();
    bool explode_done;
    bool subScreenIn();
    void gamereset();
    bool subBoxCollision(Item & itm);
    bool subBoxCollision(Entity & ent);
    void setBulletVectors();
    bool allBulletsGone();
    Timer * delay;
    int getPlayerRelativeQuad(float xvec, float yvec);
    bool did_shoot;
    bool removeBullet(list<Bullet>::iterator bul);
    list<Bullet>::iterator bulCollision(Item & a);
    list<Bullet>::iterator getEndBullets();
    bool delayBlocky();

    // inherited void Item::draw()
    // inherited bool Item::ScreenOut()
    // inherited bool Item::HP_check()


};

void setRandColor(Item & it);
void checkSound(void);
void checkLevel();

class Bomb
{

public:

    // vars
    // float radius;
    float start_radius;
    float curr_radius;
    float stop_radius;
    // float angle;
    float pos[3];
    float w, h;
    float hitbox_dim[3];
    Timer * bomb_timer;
    // Item * shards;
    unsigned char color[3];
    unsigned char launch_color[3];
    bool is_thrown;
    bool is_exploding;
    int num_bombs;
    // bool is_gone;
    GLuint * tex;
    

    // constructors
    Bomb();
    ~Bomb();

    // setters
    void setColor(unsigned char * col, int r, int g, int b);
    void setPos(float x, float y, float z);
    // void setRad(float _r);
    void launch();
    void move();
    void draw();
    void explode();
    void updateHitbox();
    bool hitboxCollision(Item & itm);
    bool hitboxCollision(Entity & ent);
    bool collision(Item & itm);
    bool collision(Entity & ent);
    // bool onScreen();
};


struct HighScore
{
    string uname;
    int score;

    HighScore(string n, int s);
    bool operator < (const HighScore & rhs);
    bool operator == (const HighScore & rhs);
    bool operator == (int val);
    bool operator == (string str);
};

class Gamerecord
{

private:
    void genFakeNames();
    bool testConnection();
public:
    
    int n = 0;  // letter input index

    // current gamers name, set for the first time when the first game ends
    char gamer[10]; // type in name

    HighScore * user_score; // struct of last scores info
    int place;  // index of the last score
    int highscore; //the highest score in file

    // string reName;  //the name in the file
    
    vector<HighScore> scores; // vector of all scores from highscore file

	// true mean no record in it
	// false means file exist
    // bool delt;

    Gamerecord();
    ~Gamerecord();
    void submitRecord(int); // submits score to be added to list of scores
                            // and makes a new menu

    bool getRecord();   // gets all the records (from local txt file currently)
    void writeRecord();// writes out the records (to a local txt file currently)
    
    void sortRecord();  // sorts the records
    void addRecord(HighScore s);    // adds a record
    bool isHighScore(); // tests if the user's score is a high score
    bool isTopTen();    // tests if the the scores is in top ten

    Menu * hs_menu; // use menu class to display the names nicely
    void makeMenu();    // makes the high score menu
    
};

class SoundBar  
{          
private:
    float slider_left_stop_pos;
    float slider_right_stop_pos;
             
             
                    //---------- design ---------------//
public:           // ______________________________
    Box boarder;//  /  ________________________   /
    Box bckgr; //  / /                        /  /
    Box leftb;  //   < L |
    Box rightb; //                  | H >
    Box line;  //         ----------
                //            _
    Box slider; //        <--|_| -->
    float pos[3];
    unsigned char color[3];
    Rect texts[3];
    std::string words[2];    
    float slider_position;
    float * value;
    float getSliderPosition();
    void moveSliderDown();
    void moveSliderUp();
    std::string bar_name;

    SoundBar(float * _val, float _x, float _y, std::string _bn_);
    // ~SoundBar();
    
    void draw();
    Box* checkButtons(float x, float y);
    // void setColor(int r, int g, int b);
    void setOrigColor();
    void setHighlight(Box * b);
    // void move(float y
};

