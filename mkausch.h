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


#include "Box.h"
#include "fonts.h"
#include "hzhang.h"


using namespace std;

inline const int NUM_SOUNDS = 10;
inline const int NUM_SONGS = 1;

enum PBType {HEALTH, COOLDOWN};

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

    // constructors
    Menu(unsigned int n_texts, 
            float _w, float _h, 
            float _x, float _y, 
            std::string* _words);
    ~Menu();

    // setters
    void set_bcolor(int r, int g, int b);   // sets boarder color
    void set_color(int r, int g, int b);   // sets mainbox color
    void set_pos(float x, float y, float z);
    void set_highlight(Box * b);
    void set_orig_color();

    // getters
    // unsigned char * get_tcolor();
    std::string get_info(); // get debug info

    // other
    void draw();
    Box* check_t_box(int x, int y);

    

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

    void init_openal();
    void close_openal();
    int current_track;
    string build_song_path(string s);
    bool is_music_paused;
    bool user_pause;
    
    string sound_names[NUM_SOUNDS] = {
                    "bullet_fire.wav",
                    "Edzes-64TheMagicNumber-intro8kHz.wav",
                    "beep.wav",
                    "boop.wav",
                    "Edzes-64TheMagicNumber-loop8kHz.wav",
                    "Timecop1983-OnTheRun8kHz.wav",
                    "pop1.wav",
                    "pop2.wav",
                    "pop_gun.wav",
                    "ray_pop.wav"};


public:
	// Source refers to the sound.
    
    // new
    void play_start_track();
	Sound();
	~Sound();
    bool check_intro_buffer_done();
    void reset_buffer_done();
    void loop_intro();
    void setup_game_mode();
    string get_song_name();
    void pause();
    void unpause();
    void toggle_user_pause();
    bool get_pause();
    bool gun_shooting;
    void rewind_game_music();
    void gun_play(int btype);
    void gun_stop();
    void beep();
    void boop();

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
    // void hp_resize();   // resizes hp box based on passed on item's health

public:
    PowerBar(const Toaster & _tos_, PBType type, float x, float y);
    PowerBar(const Item & _itm_, PBType type, float x, float y);
    
    void draw();
};


class Blocky : public Item
{
    private:
    // void set_rand_color();
    void set_rand_position();
    void init_rotation_vel();
    bool was_hit; // set when the block strikes the toaster that fall
                        // so it doesn't continuously damage it
    Item sub_boxes[8];
    int sb_angles[8];
    int rot_speed[8];
    int rot_angle[8];

    public:
    int point;
    // int lives;
    Blocky();
    ~Blocky();
    void reset(); // tests to see if the player killed poor forky
    void draw();    // overload function to include redraw
    void move();
    bool is_alive();
    bool did_damage();
    void set_hit();
    void explode();
    bool explode_done;
    bool sub_ScreenIn();
    void gamereset();


    // inherited void Item::draw() 
    // inherited bool Item::ScreenOut()
    // inherited bool Item::HP_check()
    // inherited bool Item::Collison()

};

void set_rand_color(Item & it);