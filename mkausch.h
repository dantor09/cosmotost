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

#include "Box.h"
#include "fonts.h"

using namespace std;

inline const int NUM_SOUNDS = 2;

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
Timer(double s);    // used to create a timer that counts down and will tell
                    // you if it expires
~Timer();

// setters
void reset();
void pause();
void unPause();

// getters
double getTime();
bool isDone();
bool isPaused();

};

#ifdef USE_OPENAL_SOUND
class Sound
{
private:
   
    int current_track;
    ALuint alBuffers[NUM_SOUNDS];
	ALuint alSources[NUM_SOUNDS];
    void init_openal();
    void close_openal();
    string build_song_path(string s);
    bool is_music_paused;
    bool user_pause;
    
    string sound_names[NUM_SOUNDS] = {
                    "bullet_fire.wav",
                    // "Edzes-64TheMagicNumber16kHz.wav",
                    // "Estrayk-TheHerSong1016kHz.wav",
                    "VolkorX-Enclave8kHz.wav" };
                    // "Quazar-FunkyStars16kHz.wav",
                    // "XRay-Zizibum-16kHz.wav",
                    // "Zalza-8bitTheClock16kHz.wav",
                    // "AdhesiveWombat-8bitAdventure_16kHz.wav"};

public:
	// Source refers to the sound.
    
    void cycle_songs();
    void toggle_user_pause();
    void pause();
    void unpause();
    bool get_pause();
    string & get_song_name();

	Sound();
	~Sound();

};

#endif