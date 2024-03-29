/********************************************************************
*
*		Program: mkausch.cpp
*		Authors: Michael Kausch (Group 1 - DAHM)
*		Date Code Originated: 2/17/23
***********************************************************************/

/***************************************************************************
*       Classes: 
**********************************************************************
*
*               Menu
*
*       Uses:
*           Creates a menu object based on num text params, size and position
*
*               Timer
*       Uses:
*           Creates a timer that will return when its done or you can use it to
*           just count upwards
*
*
*               Sound
*       Uses:
*           encapsulates all sound and soundfx for the game
*
*              
*               PowerBar
*       Uses:
*           draws powerbars for health / energy
*
*      
*               Blocky
*
*       Uses:
*           creates either a horizontal or vertical blocky type enemy
*
*               Bomb
*       
*       Uses:
*           creates a bomb weapon that explodes and kills all enemeies 
*               in the affected area
*
*               
*               Gamerecord
*
*       Uses:
*           manages i/o of highscore / record data
*           creates and draws highscore leaderboard
*
*
*               SoundBar
*
*       Uses:
*           creates clickable soundbars for music and sound effects
*           
**********************************************************************
*       Structs:
**********************************************************************
*         
*               HighScore
*
*       Uses:
*           encapsulates a highscore datum
*
*
*
**********************************************************************
*       Functions:
**********************************************************************
*
*           checkLevel()
*
*       Uses:
*           - monitors gametime and changes the game level state as needed
*           - enables / disables enemy types depending on the game state
*
*
*           setRandColor()
*       Uses:
*           - sets a random color to the passed in item
*
*           checkSound()
*       Uses:
*           - manages sound / sound effects for the game
******************************************************************************/


//                 		INCLUDES

#include <iostream>
#include <string>
#include </usr/include/AL/alut.h>
#include <math.h>
#include <new>
#include <sstream>
#include <GL/glx.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <time.h>
#include <cstdlib>
#include <algorithm>

#include "Global.h"
#include "mkausch.h"
//#include "hzhang.h"
#include "aparriott.h"


using namespace std;

#define BSIZE 5
#define PADDING 20
#define PI 3.1415926535

// #define USE_OPENAL_SOUND

Menu::Menu(int _n_texts,
            float w, float h,
            float x, float y,
            std::string* _words,
            int _centering)
    : n_texts{_n_texts}, pos{x, y, 0}, centering(_centering)
{
    // dynamially allocate boxes/rects for text display
    mainbox.setDim(w, h);
    mainbox.setPos(pos[0], pos[1], pos[2]);
    mainbox.setColor(47, 61, 64);
    boarder.setDim(w+BSIZE, h+BSIZE);
    boarder.setPos(pos[0], pos[1], pos[2]);
    boarder.setColor(69, 85, 89);

    try {
        t_boxs = new Box[n_texts];
        texts = new Rect[n_texts];
        words = new std::string[n_texts];


        float spacing = (2*h)/(n_texts+1);
        // std::g.log << "spacing: " << spacing << std::endl;

        for (int i = 0; i < n_texts; i++) {
            t_boxs[i].w = mainbox.w - PADDING;
            t_boxs[i].h = (spacing/2.0) - BSIZE;
            t_boxs[i].pos[0] = mainbox.pos[0];
            t_boxs[i].pos[1] = (pos[1]+mainbox.h)-((i+1)*spacing);
            t_boxs[i].setColor(61, 90, 115);
            t_boxs[i].id=i; 

            words[i] = _words[i];

        }

    } catch (std::bad_alloc& ba) {
        // if one was allocated and not the other than delete the one that
        if (texts) 
            delete [] texts;
        if (t_boxs) 
            delete [] t_boxs;
        g.log << "Error allocating rectangles in Menu call\n"
                << ba.what() << '\n';
        texts = nullptr;
        t_boxs = nullptr;
    }
}

Menu::~Menu()
{
    if (texts)
        delete [] texts;

    if (t_boxs)
        delete [] t_boxs;

    if (words)
        delete [] words;

}

// draws all sub boxes and text 
void Menu::draw()
{
    // draw boarder

    glColor3ubv(boarder.color);

    glPushMatrix();
    glTranslatef(boarder.pos[0], boarder.pos[1], boarder.pos[2]);
    glBegin(GL_QUADS);
        glVertex2f(-boarder.w, -boarder.h);
        glVertex2f(-boarder.w,  boarder.h);
        glVertex2f( boarder.w,  boarder.h);
        glVertex2f( boarder.w, -boarder.h);
    glEnd();
    glPopMatrix();

    // draw mainbox

    glColor3ubv(mainbox.color);

    glPushMatrix();
    glTranslatef(mainbox.pos[0], mainbox.pos[1], mainbox.pos[2]);
    glBegin(GL_QUADS);
        glVertex2f(-mainbox.w, -mainbox.h);
        glVertex2f(-mainbox.w,  mainbox.h);
        glVertex2f( mainbox.w,  mainbox.h);
        glVertex2f( mainbox.w, -mainbox.h);
    glEnd();
    glPopMatrix();

    // draw all t_boxes

    for (size_t i = 0; i < n_texts; i++) {
        glColor3ubv(t_boxs[i].color);

        glPushMatrix();
        glTranslatef(t_boxs[i].pos[0], t_boxs[i].pos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-t_boxs[i].w, -t_boxs[i].h);
            glVertex2f(-t_boxs[i].w,  t_boxs[i].h);
            glVertex2f( t_boxs[i].w,  t_boxs[i].h);
            glVertex2f( t_boxs[i].w, -t_boxs[i].h);
        glEnd();
        glPopMatrix();

    }

    for (int i = 0; i < n_texts; i++) {
        texts[i].bot = t_boxs[i].pos[1] - 8;
        if (!centering)
            texts[i].left = t_boxs[i].pos[0]-t_boxs[i].w + 100;
        else
            texts[i].left = t_boxs[i].pos[0];
        texts[i].center = centering;

        ggprint12(texts+i, 0, 0x00ffffff, words[i].c_str());
    }

}

// pass in mouse coords to check and see if they are within the bounds
// of the menu's text boxes
Box* Menu::checkTBox(int x, int y)
{
    Box * box_ptr = nullptr;
    size_t i;

    for (i = 0; i < n_texts; i++) {
        // mouse coords are
        if ((x > (t_boxs[i].pos[0]-t_boxs[i].w)) &&
            (x < (t_boxs[i].pos[0]+t_boxs[i].w)) &&
            (y > (t_boxs[i].pos[1]-t_boxs[i].h)) &&
            (y < (t_boxs[i].pos[1]+t_boxs[i].h))) {

            box_ptr = (t_boxs+i);
            break;
        }
    }

    return box_ptr;
}

// sets the highlight color of a sub_box
void Menu::setHighlight(Box * b)
{
    b->setColor(33,136,171);
}

// sets all sub boxes to the original color
void Menu::setOrigColor()
{
    for (size_t i = 0; i < n_texts; i++) {
        t_boxs[i].setColor(61,90,115);
    }
}

// void Menu::setBcolor(int r, int g, int b)
// {
//     bcolor[0] = (char)r;
//     bcolor[1] = (char)g;
//     bcolor[2] = (char)b;
// }

void Menu::setColor(int r, int g, int b)
{
    color[0] = (char)r;
    color[1] = (char)g;
    color[2] = (char)b;
}

void Menu::setPos(float x, float y, float z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

// std::string Menu::getInfo()
// {
//     std::ostringstream temp;
//     temp << std::endl;

//     return temp.str();
// }


/***************************************************************************
*       Class: Timer
*       Uses:
*           Creates a Timer object with duration based on input
******************************************************************************/

Timer::Timer() : duration(-1), pause_duration(0.00),
                pause_timer(nullptr), paused(false)
{    // set starting time
    start = std::chrono::system_clock::now();
}

Timer::Timer(double sec) : duration(sec), pause_duration(0.00),
                pause_timer(nullptr), paused(false)
{    // set starting time
    start = std::chrono::system_clock::now();
}

// delete pause timer if it were active
Timer::~Timer()
{
    // g.log << "in Timer destructor\n";
    if (pause_timer) {
        // g.log << "deleting pause timer\n";
        delete pause_timer;
        pause_timer = nullptr;
    }
}

/****************************** Setters *************************************/

// resets timer to current time
void Timer::reset()
{
    if (isPaused()) {
        unPause();
    }
    pause_duration = 0;
    start = std::chrono::system_clock::now();
}

/****************************** Getters *************************************/

// returns time that has elapsed since the start of the timer
int Timer::getTime(char time_code)
{
    int net_time = 0;
    int time = net_time;
    std::chrono::duration<double> total_elapsed = std::chrono::system_clock::now() - start;

    if (paused) {
        net_time = (total_elapsed.count() - pause_duration - pause_timer->getTime('n'));
    } else {
        net_time = (total_elapsed.count()-pause_duration);
    }

    // D.T - retrieve minutes, seconds, or net time
    // based on time_code passed in getTime parameter
    switch (time_code) {
        case 'm': time = net_time/60;
                  break;
        case 's': time = net_time % 60;
                  break;
        case 'n': time = net_time;
    }
    return time;
}

// checks if the timer has elapsed
// true if the timer has finished
// false if the timer hasn't
bool Timer::isDone()
{
    if (duration == -1) {   // return false for count up timers
        return false;
    } else {    // return net time for countdown timers
        return (getTime('n') > duration);
    }
}

void Timer::pause()
{
    paused = true;
    pause_timer = new Timer();
}

bool Timer::isPaused()
{
    return paused;
}

void Timer::unPause()
{
    if (paused) {
        paused = false;
        pause_duration += pause_timer->getTime('n');
        delete pause_timer;
        pause_timer = nullptr;
    }
}

#ifdef USE_OPENAL_SOUND
Sound::Sound()
{
    //Buffer holds the sound information.
    initOpenal();
    current_track = -1;  // starting track number at splash screen
    is_music_paused = false;
    user_pause = false;
    is_intro = is_game = false;
    m_vol = g.m_vol;
    sfx_vol  = g.sfx_vol;
    


    // make individual buffers of all sounds
    alBuffers[0] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[0]).c_str());
    alBuffers[1] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[1]).c_str());
    alBuffers[2] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[2]).c_str());
    alBuffers[3] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[3]).c_str());
    alBuffers[4] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[4]).c_str());
    alBuffers[5] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[5]).c_str());
    alBuffers[6] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[6]).c_str());
    alBuffers[7] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[7]).c_str());
    alBuffers[8] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[8]).c_str());
    alBuffers[9] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[9]).c_str());
    alBuffers[10] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[10]).c_str());
    alBuffers[11] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[11]).c_str());
    alBuffers[12] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[12]).c_str());
    alBuffers[13] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[13]).c_str());
    alBuffers[14] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[14]).c_str());
    alBuffers[15] = 
        alutCreateBufferFromFile(buildSongPath(sound_names[15]).c_str());


    // songBuffers[0] = alBuffers[3];
    buffersDone = buffersQueued = 0;

    // generate number of sources
    alGenSources(NUM_SOUNDS, alSources);    // keep individual sources for now
    alGenSources(1, &menuQueueSource);

    alSourceQueueBuffers(menuQueueSource, 1, (alBuffers+1));
    alSourcef(menuQueueSource, AL_GAIN, g.m_vol);
    alSourcef(menuQueueSource, AL_PITCH, 1.0f);

    alSourcei(alSources[0], AL_BUFFER, alBuffers[0]);   // bullet_fire
    alSourcef(alSources[0], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[0], AL_PITCH, 1.0f);
    alSourcei(alSources[0], AL_LOOPING, AL_TRUE);

    alSourcei(alSources[1], AL_BUFFER, alBuffers[1]);   // intro
    alSourcef(alSources[1], AL_GAIN, g.m_vol);
    alSourcef(alSources[1], AL_PITCH, 1.0f);
    alSourcei(alSources[1], AL_LOOPING, AL_FALSE);

    alSourcei(alSources[2], AL_BUFFER, alBuffers[2]);   // beep
    alSourcef(alSources[2], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[2], AL_PITCH, 1.0f);
    alSourcei(alSources[2], AL_LOOPING, AL_FALSE);

    alSourcei(alSources[3], AL_BUFFER, alBuffers[3]); // boop
    alSourcef(alSources[3], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[3], AL_PITCH, 1.0f);
    alSourcei(alSources[3], AL_LOOPING, AL_FALSE);

    alSourcei(alSources[4], AL_BUFFER, alBuffers[4]); // intro's loop
    alSourcef(alSources[4], AL_GAIN, g.m_vol);
    alSourcef(alSources[4], AL_PITCH, 1.0f);
    alSourcei(alSources[4], AL_LOOPING, AL_TRUE);

    alSourcei(alSources[5], AL_BUFFER, alBuffers[5]); // AdhesiveWombat song
    alSourcef(alSources[5], AL_GAIN, g.m_vol);
    alSourcef(alSources[5], AL_PITCH, 1.0f);
    alSourcei(alSources[5], AL_LOOPING, AL_TRUE);

    alSourcei(alSources[6], AL_BUFFER, alBuffers[6]); // zap1
    alSourcef(alSources[6], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[6], AL_PITCH, 1.0f);
    alSourcei(alSources[6], AL_LOOPING, AL_TRUE);

    alSourcei(alSources[7], AL_BUFFER, alBuffers[7]); // zap2
    alSourcef(alSources[7], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[7], AL_PITCH, 1.0f);
    alSourcei(alSources[7], AL_LOOPING, AL_TRUE);

    alSourcei(alSources[8], AL_BUFFER, alBuffers[8]); // zap3
    alSourcef(alSources[8], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[8], AL_PITCH, 1.0f);
    alSourcei(alSources[8], AL_LOOPING, AL_TRUE);

    alSourcei(alSources[9], AL_BUFFER, alBuffers[9]); // zap4
    alSourcef(alSources[9], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[9], AL_PITCH, 1.0f);
    alSourcei(alSources[9], AL_LOOPING, AL_TRUE);

    alSourcei(alSources[10], AL_BUFFER, alBuffers[10]); // shield
    alSourcef(alSources[10], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[10], AL_PITCH, 1.0f);
    alSourcei(alSources[10], AL_LOOPING, AL_FALSE);

    alSourcei(alSources[11], AL_BUFFER, alBuffers[11]); // doosh
    alSourcef(alSources[11], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[11], AL_PITCH, 1.0f);
    alSourcei(alSources[11], AL_LOOPING, AL_FALSE);

    alSourcei(alSources[12], AL_BUFFER, alBuffers[12]); // doosh2
    alSourcef(alSources[12], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[12], AL_PITCH, 1.0f);
    alSourcei(alSources[12], AL_LOOPING, AL_FALSE);

    alSourcei(alSources[13], AL_BUFFER, alBuffers[13]); // doosh
    alSourcef(alSources[13], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[13], AL_PITCH, 1.0f);
    alSourcei(alSources[13], AL_LOOPING, AL_FALSE);

    alSourcei(alSources[14], AL_BUFFER, alBuffers[14]); // doosh2
    alSourcef(alSources[14], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[14], AL_PITCH, 1.0f);
    alSourcei(alSources[14], AL_LOOPING, AL_FALSE);

    alSourcei(alSources[15], AL_BUFFER, alBuffers[15]); // zap2 - noloop
    alSourcef(alSources[15], AL_GAIN, g.sfx_vol);
    alSourcef(alSources[15], AL_PITCH, 1.0f);
    alSourcei(alSources[15], AL_LOOPING, AL_FALSE);

    // check for errors after setting sources
    if (alGetError() != AL_NO_ERROR) {
        throw "ERROR: setting source\n";
    }
    g.log << "does it reach here???" << endl;

}

Sound::~Sound()
{
    for (int i = 0; i < NUM_SOUNDS; i++) {
        // delete sources
        alDeleteSources(i+1, (alSources+i));
        // delete buffers
        alDeleteBuffers(i+1, (alBuffers+i));
    }

    alDeleteSources(1, &menuQueueSource);
    closeOpenal();
}

void Sound::initOpenal()
{
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		throw "ERROR: alutInit()\n";
		// printf("ERROR: alutInit()\n");
		// return 0;
	}

	//Clear error state.
	alGetError();

	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
}

void Sound::closeOpenal()
{
	//Close out OpenAL itself.
	//Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context.
	alcMakeContextCurrent(NULL);
	//Release context(s).
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);
}

void Sound::rewindGameMusic()
{
    alSourceStop(alSources[5]);
    alSourceRewind(alSources[5]);
    alSourcePlay(alSources[5]);
}

string Sound::buildSongPath(string s)
{
    // format of the song
    // ./Songs/Edzes-64TheMagicNumber16kHz.wav"

    ostringstream path;
    string song_dir = "Songs";

    path << "./" << song_dir << "/" << s;

    return path.str();
}

void Sound::gunPlay(int btype)
{
    // static int gun_start = 6;

    g.log << "gun shooting..." << endl;
    if (btype == 1) {
        alSourcePlay(alSources[6]);
    } else if (btype == 2) {
        alSourcePlay(alSources[7]);
        // alSourcePlay(alSources[gun_start+1]);
    } else if (btype == 3) {
        alSourcePlay(alSources[8]);
        // alSourcePlay(alSources[gun_start+1]);
        // alSourcePlay(alSources[gun_start+2]);
    } else if (btype == 4) {
        alSourcePlay(alSources[9]);
        // alSourcePlay(alSources[gun_start+3]);
    }
}

void Sound::gunStop()
{
    static int gun_start = 6;
    static int num_guns = 4;
    // g.log << "gun not shooting..." << endl;
    for (int i = 0; i < num_guns; i++) {
        alSourceStop(alSources[i+gun_start]);
    }
}

void Sound::beep()
{
    alSourcePlay(alSources[2]);
}

void Sound::boop()
{
    alSourcePlay(alSources[3]);
}

void Sound::doosh(int select)
{
    int index; // index of doosh sound effect 11 or 12 currently

    if (select == 0) {

        index = 12; // index of doosh sound effect 11 or 12 currently
    } else {
        index = 11;
    }
    alSourcePlay(alSources[index]);
}


void Sound::shieldSFX()
{
    int index = 10; // index of doosh sound effect 
    alSourcePlay(alSources[index]);
}

void Sound::exploSFX()
{
    int index = 14; // index of doosh sound effect 13 or 14 currently
    alSourcePlay(alSources[index]);
}

void Sound::playZap2()
{
    int index = 15; // index of zap2 (non-loop version) 
                    // for blocky crashes, 2nd gun
    alSourcePlay(alSources[index]);
}

bool Sound::checkIntroBufferDone()
{
    resetBufferDone();
    alGetSourcei(menuQueueSource, AL_BUFFERS_PROCESSED, &buffersDone);
    return (buffersDone == 1);
}

// resets buffers_done variable for further checks
void Sound::resetBufferDone()
{
    buffersDone = 0;
}

// unqueue's intro beat so that only loop track is in the buffer queue
// loops buffer queue at this point
void Sound::loopIntro()
{
    alSourceStop(menuQueueSource);
    alSourceRewind(menuQueueSource);
    alSourcePlay(alSources[4]);
}

void Sound::setupGameMode()
{
    // change bools for music state
    is_intro = false; is_game = true;

    // stop both the intro and loop if either are playing
    alSourceStop(alSources[4]);
    alSourceStop(menuQueueSource);
    alSourceRewind(alSources[4]);
    alSourceRewind(menuQueueSource);

    // play the game song
    alSourcePlay(alSources[5]);
}

void Sound::playStartTrack()
{
    // stop game music if it's playing
    if (is_game == true) {
        alSourceStop(alSources[5]);
        alSourceRewind(alSources[5]);
    }

    is_intro = true; 
    is_game = false;

    // begin playing menu music
    alSourcePlay(menuQueueSource);
}

// returns song names, only 2 songs for now
string Sound::getSongName()
{
    string name;
    if (is_intro) {
        name = "Edzes-64TheMagicNumber";
    }
    if (is_game) {
        name = sound_names[5];
    }
    return name;
}

// pauses song (when going to pause menu for instance)
void Sound::pause()
{
    if (!is_music_paused) {
        is_music_paused = true;
        alSourcePause(alSources[5]);
    }
}

// unpauses
void Sound::unpause()
{
    if (!user_pause) {
        if (is_music_paused) {
            is_music_paused = false;
            alSourcePlay(alSources[5]);
        }
    }
}

// separate pause state for when user explicity mutes music
void Sound::toggleUserPause()
{
    user_pause = (user_pause == true) ? false : true;
    if (user_pause)
        pause();
    else
        unpause();
}

// getter to return the pause state
bool Sound::getPause()
{
    return is_music_paused;
}

// changes the volume of music tracks
void Sound::updateMusicVol()
{
    int n_songs = 3;
    int song_indices[n_songs] = {1, 4, 5};

    for (int i = 0; i < n_songs; i++) {
        alSourcef(alSources[song_indices[i]], AL_GAIN, g.m_vol);
    }

    alSourcef(menuQueueSource, AL_GAIN, g.m_vol);
}

// changes the volume of all sfx in game
void Sound::updateSFXVol()
{
    int n_sfx = 12;
    int sfx_indices[n_sfx] = {0, 2, 3, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    for (int i = 0; i < n_sfx; i++) {
        alSourcef(alSources[sfx_indices[i]], AL_GAIN, g.sfx_vol);
    }
}

// play's bombExplosion sound
void Sound::bombExplosion()
{
    int bomb_index = 13;

    alSourcePlay(alSources[bomb_index]);
}

#endif

// initializes settings for powerbar used with donut
PowerBar::PowerBar(const Donut & _itm_, PBType _type_, float x, float y)
{
    // maybe put max_health of each enemy type in case were going to
    // use this healthbar for the boss as well
    donut = &_itm_;
    itm = nullptr;
    tos = nullptr;
    type = _type_;

    if (type != DONUT) {
        g.log << "WARNING** : BAD POWERBAR CONSTRUCTER, TYPE != DONUT" << endl;
    }
    
    total.setColor(0,0,0);   // set lost health to black
    health.setColor(255,0,0);  // set health to r
    total.setDim(g.xres/2.0f,2.0f);
    total.setPos(x, y, 0);
    
    // mimic other bar based on what health was set to
    health.setDim(total.w,total.h);
    health.setPos(total.pos[0],total.pos[1],total.pos[2]);

    text.bot = total.pos[1]-5;
    text.left = total.pos[0];
    text.center = 1;
    // g.log << "finished itm constructor" << endl;
}

// initializes settings for itm class
PowerBar::PowerBar(const Item & _itm_, PBType _type_, float x, float y)
{
    // maybe put max_health of each enemy type in case were going to
    // use this healthbar for the boss as well
    itm = &_itm_;
    tos = nullptr;
    type = _type_;

    if (type == HEALTH) {
        total.setColor(255,0,0);   // set lost health to red
        health.setColor(0,255,0);  // set health to green
        total.setDim(75,10);
        total.setPos(x, y, 0);
    } else if (type == COOLDOWN) {
        total.setColor(108,122,137);
        health.setColor(0,0,0);
        total.setDim(75,4);
        total.setPos(x, y, 0);
    }
    // mimic other bar based on what health was set to
    health.setDim(total.w,total.h);
    health.setPos(total.pos[0],total.pos[1],total.pos[2]);

    text.bot = total.pos[1]-5;
    text.left = total.pos[0];
    text.center = 1;
    // g.log << "finished itm constructor" << endl;
}

// initializes settings for use with toaster class
PowerBar::PowerBar(const Toaster & _tos_, PBType _type_, float x, float y)
{
    // maybe put max_health of each enemy type in case were going to 
    // use this healthbar for the boss as well
    tos = &_tos_;
    itm = &_tos_;
    type = _type_;

    if (type == HEALTH) {
        total.setColor(255,0,0);   // set lost health to red
        health.setColor(0,255,0);  // set health to green
        total.setDim(75,10);
        total.setPos(x, y, 0);
    } else if (type == COOLDOWN) {
        total.setColor(196,145,2);
        health.setColor(255,255,0);
        total.setDim(75,8);
        total.setPos(x, y, 0);
    }
    // mimic other bar based on what health was set to
    health.setDim(total.w,total.h);
    health.setPos(total.pos[0],total.pos[1],total.pos[2]);

    text.bot = total.pos[1]-5;
    text.left = total.pos[0];
    text.center = 1;

    // g.log << "finished tos constructor" << endl;
}

// draw power bars as a proportion of current health / power
void PowerBar::draw()
{
    if (type == HEALTH) {
        glColor3ubv(total.color);
        glPushMatrix();
        glTranslatef(total.pos[0], total.pos[1], total.pos[2]);
        glBegin(GL_QUADS);
            glVertex2f(-total.w, -total.h);
            glVertex2f(-total.w,  total.h);
            glVertex2f( total.w,  total.h);
            glVertex2f( total.w, -total.h);
        glEnd();
        glPopMatrix();

        // draw mainbox
        // hp_resize();
        glColor3ubv(health.color);
        
        glPushMatrix();
        glTranslatef(health.pos[0]-health.w, health.pos[1], health.pos[2]);
        glBegin(GL_QUADS);
            glVertex2f(0, -health.h);
            glVertex2f(0,  health.h);
            glVertex2f((((float)(itm->hp))/(float)(itm->starting_hp))*2.0f*health.w,  health.h);
            glVertex2f((((float)(itm->hp))/(float)(itm->starting_hp))*2.0f*health.w, -health.h);
            
        glEnd();
        glPopMatrix();

        ggprint8b(&text, 0, 0x00000000, "%i/%i  Lives: %i", 
                            (int)(itm->hp), (int)itm->starting_hp, itm->lives);
    } else if (type == COOLDOWN) {

        glColor3ubv(total.color);
        glPushMatrix();
        glTranslatef(total.pos[0], total.pos[1], total.pos[2]);
        glBegin(GL_QUADS);
            glVertex2f(-total.w, -total.h);
            glVertex2f(-total.w,  total.h);
            glVertex2f( total.w,  total.h);
            glVertex2f( total.w, -total.h);
        glEnd();
        glPopMatrix();

        glColor3ubv(health.color);
        glPushMatrix();
        glTranslatef(health.pos[0]-health.w, health.pos[1], health.pos[2]);
        glBegin(GL_QUADS);
            glVertex2f(0, -health.h);
            glVertex2f(0,  health.h);
            glVertex2f( (((tos->energy))/((float)(tos->max_energy)))*2.0f*health.w,  health.h);
            glVertex2f( (((tos->energy))/((float)(tos->max_energy)))*2.0f*health.w, -health.h);
            
        glEnd();
        glPopMatrix();

        ggprint8b(&text, 0, 0x00FF0000, "Jump Energy: %i/%i", (int)tos->energy, (int)tos->max_energy);
        // g.log << "tos->energy: " << tos->energy << " max_energy: " << max_energy << endl;
    } else if (type == DONUT) {
        
        glColor3ubv(total.color);
        glPushMatrix();
        glTranslatef(total.pos[0], total.pos[1], total.pos[2]);
        glBegin(GL_QUADS);
            glVertex2f(-total.w, -total.h);
            glVertex2f(-total.w,  total.h);
            glVertex2f( total.w,  total.h);
            glVertex2f( total.w, -total.h);
        glEnd();
        glPopMatrix();

        glColor3ubv(health.color);
        glPushMatrix();
        glTranslatef(health.pos[0]-health.w, health.pos[1], health.pos[2]);
        glBegin(GL_QUADS);
            glVertex2f(0, -health.h);
            glVertex2f(0,  health.h);
            glVertex2f((((donut->hp))/(10000.0f))*2.0f*health.w,  health.h);
            glVertex2f((((donut->hp))/(10000.0f))*2.0f*health.w, -health.h);
            
        glEnd();
        glPopMatrix();

        // ggprint8b(&text, 0, 0x00000000, "Boss Health: %i/%i", (int)donut->hp, 10000);
    }
}

// modified from hzhang's file by mkausch
// detects collision between item class and entity class
bool Entity::collision(Item & a)
{
    // for (x0,y0,x1,y1)and(x2,y2,x3,y3) squares
    // if collison -->(x0-x3)*(x1-x2)<0
    // same for y
    bool x = (((pos[0]+dim[0])-(a.pos[0]-a.w))*((pos[0]-dim[0])-(a.pos[0]+a.w))) < 0;
  	bool y = (((pos[1]+dim[1])-(a.pos[1]-a.h))*((pos[1]-dim[1])-(a.pos[1]+a.h))) < 0;
  	return x&&y;
}

// sets hp of enentiy items
void Entity::setHP(float life)
{
    hp = life;
}

// sets damage of entity class
void Entity::setDamage(float x)
{
    damage = x;
}

// damages entity item based on item passed in
void Entity::hpDamage(Item & a) 
{
    hp = hp - a.damage;
    stats.damageTaken += a.damage;
}

// checks to see if hp is gone
bool Entity::hpCheck()
{
    stats.UpdateKills(); // declared in aparriott.cpp
    return (hp < 0.01);
}

// damages item object based on passed in entity object
void Item::hpDamage(Entity & e)
{
    hp = hp - e.damage;
    stats.damageTaken += e.damage;

}

// Blocky constructor
// pass in type (verticle or horizontal)
// pass in whether the gun is active on the blocky 
Blocky::Blocky(char type, bool g_act)
{
    srand(time(NULL));
    float sub_blocky_size = sqrt((25.0*100.0)/SUB_BLOCK_N);
    if (type == 'v') {
        setDim(60.0f, 85.0f);
        tex = (g_act == false) ? &g.blocky_silhouette : &g.whomp_silhouette;
    } else {
        if (g_act == false) {
            setDim(125.0f, 75.0f);
            tex = &g.mitt_silhouette;
        } else {
            setDim(125.0f, 50.0f);
            tex = &g.ptm_silhouette;
        }
        // tex = (g_act == false) ? &g.ptm_silhouette : &g.mitt_silhouette;
        // tex = &g.ptm_silhouette;
    }

    setRandColor(*this);
    setRandPosition();
    setAcc(0.0f,-0.25f,0.0f);
    setVel(0.0f, -4.0f, 0.0f);
    setDamage(20);
    starting_hp = 30;
    setHP(starting_hp);
    point = starting_hp;
    bul_point = 5;
    was_hit = false;
    lives = 1;
    explode_done = true;
    did_shoot = false;
    delay = nullptr;
    delay_t = 0.5;
    gun_active = g_act;
    

    // sub box assignment
    // assignes itself and it's mirror image (i+4 in this case)
    int angle = 80;
    float angle_offset = (angle*2/SUB_BLOCK_N);
    angle = 70;
    int rvel = 8;
    float deg_to_rad = (PI / 180.0f);
    for (int i = 0; i < SUB_BLOCK_N; i++) {
        sub_boxes[i].setDim(sub_blocky_size, sub_blocky_size);
        sub_boxes[i].setColor(255,0,0);    // make them red for now
        sub_boxes[i].setAcc(0, 0, 0);
        sb_angles[i] = angle;
        angle -= angle_offset;
        sub_boxes[i].setVel((rvel*cos(deg_to_rad * sb_angles[i])),
                                    (rvel*sin(deg_to_rad * sb_angles[i])), 0);
    }

    initRotationVel();
}

Blocky::~Blocky()
{
    if (delay) {
        delete delay;
        delay = nullptr;
    }
}

void Blocky::initRotationVel()
{
    // 0 the starting angle and assign random change in rotation angle
    for (int i = 0; i < SUB_BLOCK_N; i++) {
        rot_angle[i] = 0;
        rot_speed[i] = -40 + (rand() % 41);
    }
}

void Blocky::setRandPosition()
{
    static int pm_dir = 1;
    // float curr_player_xpos;

    // float curr_player_ypos = (tos.pos[1] > (g.xres - tos.h - 5)) ? g.xres / 2.0 : tos.pos[0];
   
    float curr_player_xpos = (tos.pos[0] < 10 ) ? g.xres / 2.0 : tos.pos[0];
    float curr_player_ypos = tos.pos[1];

    // check and make sure toaster is not right up against the top of the screen
    if ((g.yres - (tos.pos[1] + tos.h)) < 20) {
        setPos(tos.pos[0], 0-h, 0);
    } else {
        
        int delta_from_xpos = rand() % 50;
        float block_xpos = curr_player_xpos + (delta_from_xpos * pm_dir);

        // set to be this new random position situated near the player char
        // that is above the yres and out of view of the screen
        setPos(block_xpos, g.yres+h,0);

        // if this block was generated in front of the player then
        // next time make it randomly behind the player (it'll keep switching)
        pm_dir *= -1;

    }
    
}

// sets random color of item object passed in
void setRandColor(Item & it)
{
    static int color[5][3] =   {{242, 4, 159},
                        {4, 177, 216},
                        {4, 216, 78},
                        {242, 202, 4},
                        {242, 135, 4}};
    static int index = rand() % 5;
    it.setColor(color[index][0], color[index][1], color[index][2]);
    index = (index + 1) % 5;
}

// tests to see if the sub boxes are on screen
bool Blocky::subScreenIn()
{
    bool subs_onscreen = false;

    for (int i = 0; i < SUB_BLOCK_N; i++) {
        subs_onscreen = sub_boxes[i].screenIn();
        if (subs_onscreen)
            break;
    }

    return subs_onscreen;
}

// delays blocky once he goes off screen
// controls the timer associated with the delay
bool Blocky::delayBlocky()
{
    bool finish_delay = false;

    if (delay == nullptr) {
        delay = new Timer(delay_t);
        g.log << "delaying Blocky" << endl;
        
    } else if (delay->isDone()) {
        // reset();
        delete delay;
        delay = nullptr;
        g.log << "blocky delay done" << endl;
        finish_delay = true;
    } else if (!delay->isDone() && !delay->isPaused()) {
        if (g.state == PAUSE) {
            delay->pause();
            g.log << "pausing blocky delay timer" << endl;
        }
    } else if (!delay->isDone() && delay->isPaused()) {
        if (g.state == GAME) {
            delay->unPause();
            g.log << "unpausing blocky delay timer" << endl;

        }
    }
    return finish_delay;
}

// draws blocky and sub blockies if he's exploded
// draws blocky's bullets if he shoots
void Blocky::draw()
{
    // draw bullets 
    if (gun_active && isAlive() && did_shoot) {
        // draw blocky's bullets if they've been shot

        for (auto bul = bullets.begin(); bul != bullets.end(); bul++) {
            if (g.state != PAUSE)
                // g.log << "drawing bullet " << &(*bul) << endl;
            
            glPushMatrix();
            glColor3ub(bul->color[0], bul->color[1], bul->color[2]);
            if (g.state != PAUSE)
                // g.log << "\tcurrent pos[x]: " << bul->pos[0] << endl;
                // g.log << "\tcurrent pos[y]: " << bul->pos[1] << endl;
            glTranslatef(bul->pos[0], bul->pos[1], bul->pos[2]);
            glBegin(GL_QUADS);
                    glVertex2f(-bul->w, -bul->h);
                    glVertex2f(-bul->w,  bul->h);
                    glVertex2f( bul->w,  bul->h);
                    glVertex2f( bul->w, -bul->h);
            glEnd();
            glPopMatrix();
        }
    }
    
    // draw big blocky
    if (isAlive() && explode_done) {

        // delay blocky if he went out of screen
        if (screenOut() && delayBlocky()) {
            reset();
        }
    // TODO 

        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, *tex);
        // glColor3ub(color[0], color[1], color[2]);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);

        // draw pissed-off coloring if his health is below 50%
        if (hp < starting_hp / 2) {
            setRandColor(*this);
            glColor4f(color[0]/255.0f, color[1]/255.0f, color[2]/255.0f, 0.5f);
        } else {
            glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
        }

        glTranslatef(pos[0], pos[1], pos[2]);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(-w, -h);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(-w,  h);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f( w,  h);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f( w, -h);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        glPopMatrix();

    } else {    // draw little blockies
        // g.log << "checking if sub boxes are in the screen...\n";
        if (subScreenIn()) {
            for (int i = 0; i < SUB_BLOCK_N; i++) {
                setRandColor(sub_boxes[i]);
                glPushMatrix();
                glColor3ub(sub_boxes[i].color[0],
                            sub_boxes[i].color[1],
                            sub_boxes[i].color[2]);
                glTranslatef(sub_boxes[i].pos[0], sub_boxes[i].pos[1], sub_boxes[i].pos[2]);
                glMatrixMode(GL_MODELVIEW);
                glRotatef(rot_angle[i], 0, 0, 1.0);
                glBegin(GL_QUADS);
                        glVertex2f(-sub_boxes[i].w, -sub_boxes[i].h);
                        glVertex2f(-sub_boxes[i].w,  sub_boxes[i].h);
                        glVertex2f( sub_boxes[i].w,  sub_boxes[i].h);
                        glVertex2f( sub_boxes[i].w, -sub_boxes[i].h);
                glEnd();
                glPopMatrix();

                rot_angle[i] -= rot_speed[i];
            }
        } else {
            // rot_angle = 0;
            initRotationVel();
            explode_done = true;
            // reset_sub_boxes();
        }
    }
}

// resets blocky's vars when he dies or goes off screen
void Blocky::reset()
{
    if (hpCheck()) {
        lives--;
        explode();
        // g.log << "explode called\n";
        explode_done = false;
        if (lives > 0) {
            hp = starting_hp;   // give back full health
        }

        stats.UpdateKills(); // aparriott.cpp
    }
    was_hit = false;

    if (tex == &g.ptm_silhouette) {
        float xv = tos.vel[0]*0.75;
        g.log << "setting xv: " << xv << endl;

        setVel(xv, -4.0f, 0.0f);
    } else {
        setVel(0, -4.0f, 0.0f);
    }

    setRandPosition();    // put at a new random position
    did_shoot = false;
    // was_hit = false;
    // g.log << "was_hit set to " << boolalpha << was_hit << endl;
}

void Blocky::gamereset()
{
    lives = 1;
    hp = starting_hp;
    setVel(0.0f, -4.0f, 0.0f);
    setRandPosition();    // put at a new random position
    was_hit = false;
}

bool Blocky::didDamage()
{
    return was_hit;
}

void Blocky::move()
{
    // static float shoot_point = g.yres*(4/5.0f);
    static float shoot_point = 100.0;   // distance away he shoots
    float distance = abs(pos[1] - tos.pos[1]);

        // move main blocky
    if (isAlive() && explode_done) {
        pos[0] += vel[0];
        pos[1] += vel[1];
        pos[2] += vel[2];
        vel[0] += acc[0];
        vel[1] += acc[1];
        vel[2] += acc[2];
    } else if (!explode_done) { // move sub boxes until they fall off screen
        if (subScreenIn()) {
            for (int i = 0; i < SUB_BLOCK_N; i++) {
                sub_boxes[i].pos[0] += sub_boxes[i].vel[0];
                sub_boxes[i].pos[1] += sub_boxes[i].vel[1];
                sub_boxes[i].pos[2] += sub_boxes[i].vel[2];
                sub_boxes[i].vel[0] += sub_boxes[i].acc[0];
                sub_boxes[i].vel[1] += sub_boxes[i].acc[1];
                sub_boxes[i].vel[2] += sub_boxes[i].acc[2];
            }
        }
    }

    // shoot at player once blocky gets past 1/2 screen distance
    // g.log << "pos[1]: " << pos[1] << "\tshoot_point: " << shoot_point << endl;
    // if (did_shoot == false && pos[1] <= shoot_point) {
    
    if (gun_active) {
        if (did_shoot == false && distance <= shoot_point) {

            did_shoot = true;
            // g.log << "blocky below shootpoint" << endl;
            setBulletVectors();
        }

        if (did_shoot) {

            for (auto it = bullets.begin(); it != bullets.end(); it++) {
                it->moveBullet();
            }
        }
    }

}

// returns an iterator to the bullet that causes a collision with an item
list<Bullet>::iterator Blocky::bulCollision(Item & a) 
{
    for (auto it = bullets.begin(); it != bullets.end(); it++) {
        if (it->collision(a)) {
            return it;
        }
    }

    return bullets.end();
}

// returns iterator to end of bullet list
list<Bullet>::iterator Blocky::getEndBullets()
{
    return bullets.end();
}

// removes a bullet from the linked list
bool Blocky::removeBullet(list<Bullet>::iterator bul)
{
    bullets.erase(bul);
    return true;
}

// tests for collisions with Items on blocky's subBoxes
bool Blocky::subBoxCollision(Item & itm)
{
    for (int i = 0; i < SUB_BLOCK_N; i++) {
        if (sub_boxes[i].collision(itm)) {
            return true;
        }
    }
    return false;
}

// tests for collisions with Entitys on blocky's subBoxes
bool Blocky::subBoxCollision(Entity & ent)
{
    for (int i = 0; i < SUB_BLOCK_N; i++) {
        if (ent.collision(sub_boxes[i])) {
            return true;
        }
    }
    return false;
}

// damages the itemp based on the passed in blocky's damage val
// sets was_hit bool so that toaster is only hit once per fall
void Item::hpDamage(Blocky & bf)
{
    // g.log << "blocky's hpDamage called" << endl;
    if (!bf.was_hit) {
        if (item_type == 0) {   // toaster collision
            hp = hp - bf.damage;
            bf.was_hit = true;
        } else {
            hp = hp - bf.damage;
        }
            stats.damageTaken += bf.damage;
        // g.log << "blocky hit something" << endl;
    }
}

// returns true if lives > 0
bool Blocky::isAlive()
{
    return (lives > 0);
}

// void Blocky::setHit()
// {
//     was_hit = true;
//     // g.log << "setting hit in blocky" << endl;
// }

void Blocky::explode()
{
    int rvel = 8;
    float deg_to_rad = PI/180.0f;
    int pixel_offset = 8;   // sets origin of offset to be 8 left and 8 down
    int xcoord = pos[0] - pixel_offset;
    int ycoord = pos[1] - pixel_offset;
    int rand_offset; // pixel_offset pixel offset randomly from center of blocky

    for (int i = 0; i < SUB_BLOCK_N; i++) {
        rand_offset = rand() % (pixel_offset * 2);
        sub_boxes[i].setPos(pos[0]+rand_offset, pos[1]+rand_offset, 0);
        sub_boxes[i].setVel((rvel*cos(deg_to_rad * sb_angles[i])),
                                    (rvel*sin(deg_to_rad * sb_angles[i])), 0);
    }
}

// gets relative quadrant on cartesian plane of toaster with
// respect to blocky
int Blocky::getPlayerRelativeQuad(float xvec, float yvec)
{
    // g.log << "xvec: " << xvec << " yvec: " << yvec << endl;
    if (xvec >= 0 && yvec >= 0) {
        // g.log << "quad 1" << endl;
        return 1;
    }
    else if (xvec >=0 && yvec < 0) {
        // g.log << "quad 4" << endl;
        return 4;
    }
    else if (xvec < 0 && yvec >= 0) {
        // g.log << "quad 2" << endl;
        return 2;
    }
    else if (xvec < 0 && yvec < 0) {
        // g.log << "quad 3" << endl;
        return 3;
    }
    else
        throw "bad quad calc";
}

// initializes bullets 
void Blocky::setBulletVectors()
{
    bullets.clear();
    g.log << "setBulletVectors called" << endl;
    float bullet_vel = 15.0;
    // float spread = ((5*PI)/(180));
    float spread = 4;
    float vec[2] = {(tos.pos[0] - pos[0]), (tos.pos[1] - pos[1])};
    int quad = getPlayerRelativeQuad(vec[0], vec[1]);
    int quad_coeffs[2] = {(quad==1 || quad ==4)?1:-1, (quad<3?1:-1)};
    float hyp = sqrt((pow(vec[0],2) + pow(vec[1],2)));
    // float angle_rad = abs(atan(vec[1]/vec[0]));
    float angle_rad = atan(vec[1]/vec[0]);
    float angle_deg = abs(angle_rad*(180/PI));
    // g.log << "quadrand: " << quad << endl;
    // g.log << "quad_coeff[x]: " << quad_coeffs[0] << 
    //         " quad_coeff[y]: " << quad_coeffs[1] << endl;
    // g.log << "angle_deg: " << angle_rad*(180/PI) << endl;

    // start from first angle and center bullet spread around angle_rad
    float start_angle = angle_deg - (NUM_BLOCKY_BULLETS/2.0f)*spread;    

    // make one incomplete bullet and modify this one when adding the
    // different angles to the linked list
    Bullet temp;
    temp.setPos(pos[0], pos[1], pos[2]);
    temp.setDim(4.0, 4.0);
    temp.setColor(255, 0,0);
    temp.setDamage(1);
    temp.setHP(1);
    temp.setAcc(0.0, 0.0, 0.0);
    temp.item_type = 41; // just using this bullet type for now

    for (int i = 0; i < NUM_BLOCKY_BULLETS; i++) {
        // g.log << "start_angle: " << start_angle << endl;
        temp.setVel(bullet_vel*cos(start_angle*(PI/180))*quad_coeffs[0], 
                    bullet_vel*sin(start_angle*(PI/180))*quad_coeffs[1], 
                                                            0.0);
        // setRandColor(temp);
        bullets.push_front(temp);
        start_angle += spread;
    }

    // g.log << "pushed the following bullets to blocky's list:\n";
    // for (auto it = bullets.begin(); it != bullets.end(); it++) {
    //     g.log << "addr: " << &(*it) << endl;
    //     g.log << "vel[x]: " << it->vel[0] << " vel[y]: " << it->vel[1] << endl;
    //     g.log << it->getInfo() << endl;

    // }

}


#ifdef USE_OPENAL_SOUND

void checkSound(void)
{
	static bool initial_play = false;
	static bool loop_set = false;
	static bool initial_game_setup = false;
	static int prev_btype = 1;
    static int exploded = 0;
    static int bhit_occured = 0;
    static bool bomb_playing = false;


    // Main menu / music SFX loop check
	if (g.state == SPLASH || g.state == MAINMENU || g.state == GAMEOVER) {
		// init_game_setup will unque intro buffers and queue game songs
		initial_game_setup = false;	// switch to false if it was prev true

		if (initial_play == false) {
			// g.log << "calling playStartTrack()" << endl;
			sounds.playStartTrack();	// queues intro songs and plays
			initial_play = true;
		}

		if (sounds.checkIntroBufferDone() && !loop_set) {
			// sounds.resetBufferDone();
			// g.log << "sounds.checkintobuffer == true" << endl;
			// g.log << "calling loopIntro()" << endl;
			sounds.loopIntro();
			loop_set = true;
		}
	} else if (g.state == GAME && initial_game_setup == false) {
			// reset initial play so that intro plays
		initial_play = loop_set = false;
		initial_game_setup = true;
		// g.log << "calling setupGameMode()" << endl;
		sounds.setupGameMode();

	}

	// *******     SFX NOISES      **********//

    if (g.state == GAME) {
        // start playing new sound if leveled up gun
        if ((tos.bullet_type_prime != prev_btype) && (sounds.gun_shooting)) {
            sounds.gunStop();
            sounds.gunPlay(tos.bullet_type_prime);
            prev_btype = tos.bullet_type_prime;
        }
        // if space bar is pressed down and gun not already shooting
        if ((g.keys[XK_space] == 1) && (!sounds.gun_shooting)) {
            g.log << "tos.bullet_type_prime: " << tos.bullet_type_prime << endl;
            sounds.gunPlay(tos.bullet_type_prime);
            sounds.gun_shooting = true;
            // if spacebar not pressed down and gun noise currently set to shoot
        } else if (g.keys[XK_space] == 0 && (sounds.gun_shooting)) {
            sounds.gunStop();
            sounds.gun_shooting = false;
        }

        if (blocky->explode_done == false && exploded == 0) {
            sounds.exploSFX();
            exploded = true;
        } else if (blocky->explode_done == true && exploded == 1) {
            exploded = false;
        }

        if (blocky->was_hit == true && bhit_occured == 0) {
            g.log << "should be playing doosh";
            sounds.doosh(1);
            bhit_occured = true;
            blocky->was_hit = false;
        } else if (blocky->was_hit == false && bhit_occured == 1) {
            bhit_occured = false;
        }

        // bomb explosion noise
        if (bomb.is_thrown && !bomb_playing) {
            sounds.bombExplosion();
            bomb_playing = true;
        } else if (!bomb.is_thrown && 
                    !bomb.is_exploding && 
                        bomb_playing) {
            bomb_playing = false;
        }

    } else {
        if (sounds.gun_shooting == true) {
            sounds.gunStop();
            sounds.gun_shooting = false;
        }
    }
}
#endif

// directs enemies to be present during specified states
// also changes enemy settings if it's relevent
void checkLevel()
{
    static bool lvl_change = false;

    if (g.substate != DEBUG) {
        int level_duration = 20; // 20 second levels at the moment
        int level_time = g.gameTimer.getTime('n');
        
        static int lvl_change_time;

        // wait until the next clock tick
        if (lvl_change && lvl_change_time != level_time) {
            lvl_change = false;
            g.log << "lvl_change toggled to false\n";
        }

        if (g.state == GAME && 
            lvl_change == false && 
            level_time != 0 &&
            // g.level != LEVEL10 &&
            ((level_time % (level_duration)) == 0)) {
            lvl_change = true;
            g.log << "lvl change being toggled to true\n";
            lvl_change_time = level_time; 
            
            // level up handler
            switch (g.level) {
                case LEVEL1:
                    // Level2: Bread(2)
                    makeBread(g.xres-10 ,0.5*g.yres,0.0,2,1); // gun level up
                    makeBread(g.xres-10 ,0.35*g.yres,0.0,5,1); // full power
                    g.level = LEVEL2;
                    g.log << "level changed to level 2" << endl;
                    // g.bread_active = true;
                    // change bread vars
                    break;
                case LEVEL2:
                    // Level3: Entities(1) + Bread(1)
                    makeBread(g.xres-10 ,0.5*g.yres,0.0,2,1); // gun level up
                    makeBread(g.xres-10 ,0.35*g.yres,0.0,5,1); // full power
                    makeBread(g.xres-10 ,0.75*g.yres,0.0,6,1); // full health
                    g.level = LEVEL3;
                    g.log << "level changed to level 3" << endl;
                    g.entity_active = true;
                    break;
                case LEVEL3:
                    // Level4: Entities(2) + Bread(2)
                    makeBread(g.xres-10 ,0.5*g.yres,0.0,2,1); // gun level up
                    makeBread(g.xres-10 ,0.25*g.yres,0.0,7,1); // extra life
                    makeBread(g.xres-10 ,0.35*g.yres,0.0,5,1); // full power
                    g.level = LEVEL4;
                    g.log << "level changed to level 4" << endl;

                    // g.entity_active = true;
                    // change entity vars
                    break;
                case LEVEL4:
                    // Level5: Blocky(1) + Bread(2) + Entities(2)
                    // makeBread(g.xres-10 ,0.25*g.yres,0.0,7,1); // extra life
                    makeBread(g.xres-10 ,0.5*g.yres,0.0,2,1); // gun level up
                    makeBread(g.xres-10 ,0.75*g.yres,0.0,6,1); // full health
                    g.level = LEVEL5;
                    g.log << "level changed to level 5" << endl;
                    // g.entity_active = true;
                    g.mike_active = true;
                    blocky = &vblocky;
                    blocky->gamereset();
                    blocky_health = &vblocky_health;
                    break;
                case LEVEL5:
                    // Level6: Blocky(2) + Bread(2) + Entities(2)
                    makeBread(g.xres-10 ,0.5*g.yres,0.0,2,1); // gun level up
                    makeBread(g.xres-10 ,0.25*g.yres,0.0,7,1); // extra life
                    makeBread(g.xres-10 ,0.75*g.yres,0.0,6,1); // full health
                    g.level = LEVEL6;
                    g.log << "level changed to level 6" << endl;
                    blocky = &v2blocky;
                    blocky->gamereset();
                    blocky_health = &v2blocky_health;
                    // g.mike_active = true;

                    // g.entity_active = true;
                    g.dtorres_active = true;
                    freeze_block_velocity_reduction_rate = 0.001;
                    
                    // change blocky vars
                    break;
                case LEVEL6:
                    // Level7: HBlocky(1) + Bread(2) + Entities(2)
                    g.level = LEVEL7;
                    g.log << "level changed to level 7" << endl;
                    makeBread(g.xres-10 ,0.5*g.yres,0.0,2,1); // gun level up
                    makeBread(g.xres-10 ,0.25*g.yres,0.0,7,1); // extra life
                    makeBread(g.xres-10 ,0.35*g.yres,0.0,5,1); // full power

                    // change blocky to horizontal
                    blocky = &hblocky;
                    blocky_health = &hblocky_health;
                    blocky->gamereset();
                    // g.mike_active = true;
                    // g.entity_active = true;
                     g.dtorres_active = true;
                     freeze_block_velocity_reduction_rate = 0.0009;
                    break;
                case LEVEL7:
                    // Level8: HBlocky(2) + Bread(2) + Entities(2)
                    g.level = LEVEL8;
                    g.log << "level changed to level 8" << endl;
                    makeBread(g.xres-10 ,0.5*g.yres,0.0,2,1); // gun level up
                    makeBread(g.xres-10 ,0.25*g.yres,0.0,7,1); // extra life
                    makeBread(g.xres-10 ,0.75*g.yres,0.0,6,1); // full health

                    // change HBlocky vars
                    blocky = &h2blocky;
                    blocky_health = &h2blocky_health;
                    blocky->gamereset();
                    // g.mike_active = true;
                    // g.entity_active = true;
                    g.dtorres_active = true;
                    freeze_block_velocity_reduction_rate = 0.00019;
                    break;
                case LEVEL8:
                    // Level9: Boss
                    makeBread(g.xres-10 ,0.5*g.yres,0.0,2,1); // gun level up
                    makeBread(g.xres-10 ,0.25*g.yres,0.0,7,1); // extra life
                    makeBread(g.xres-10 ,0.35*g.yres,0.0,5,1); // full power
                    g.level = LEVEL9;
                    g.log << "level changed to level 9" << endl;
                    // g.entity_active = true;
                    // unleash bossman randy savage
                    g.entity_active = false;
                    g.huaiyu_active = true;
                    g.dtorres_active = false;
                    tos.disable_keys = false;
                    break;
                case LEVEL9:
                    // should transition to game over
                    // g.level = LEVEL1;
                    makeBread(g.xres-10 ,0.25*g.yres,0.0,7,1);  // extra life
                    makeBread(g.xres-10 ,0.75*g.yres,0.0,6,1); // full health
                    tos.disable_keys = false;
                    g.bread_active = true;
                    g.entity_active = false;
                    g.huaiyu_active = false;
                    // g.dtorres_active = false;
                    donut.donutReset();
                    g.donut_active = true;
                    g.level = LEVEL10;
                    g.log << "level changed to level 10" << endl;
                    break;
                case LEVEL10:
                    // do nothing during the boss level
                    makeBread(g.xres-10 ,0.75*g.yres,0.0,6,1); // full health
                    break;
                default:    // Level 1 behavior (Bread(1))   // shouldn't need
                    g.level = LEVEL1;
                    break;
            }
        }      

    } else {
        tos.bullet_type_prime = 4;
    }

    if (g.state == GAMEOVER) {
            g.level = LEVEL1;
            g.bread_active = false;
            g.huaiyu_active = false;
            g.entity_active = false;
            g.dtorres_active = false;
            g.mike_active = false;
            g.donut_active = false;
            blocky = &vblocky;
            blocky_health = &vblocky_health;
    }

}

HighScore::HighScore(string n, int s)
    : uname(n), score(s) { }

// overloaded < operator to compare scores in algorithm lib
bool HighScore::operator < (const HighScore & rhs)
{
    return (score < rhs.score);
}

// overloaded = operator used to see if it's exactly equal to the last
// score in the highscores list.. if it is then it keeps the new val
bool HighScore::operator == (const HighScore & rhs)
{
    return (score == rhs.score);
}

bool HighScore::operator == (int val)
{
    return score == val;
}

bool HighScore::operator == (string str)
{
    return uname == str;
}

Gamerecord::Gamerecord()
{
	bool read_success = getRecord();
	
	// make new blank record with fake names
	if (!read_success) {
        // generate and load fake scores into file so that there's always 10
		genFakeNames();			
        // write file to disk so that it now exists for the future
        writeRecord();	
	}

	memset(gamer,' ',9); gamer[9] = '\0';
	highscore = scores[scores.size()-1].score;
	user_score = nullptr;
    hs_menu = nullptr;
    place = -1;
    makeMenu();
}

Gamerecord::~Gamerecord()
{
    g.log << "destructor called..." << endl;
	writeRecord();
	if (user_score) {
		delete user_score;
		user_score = nullptr;
	}

    if (hs_menu) {
        delete hs_menu;
        hs_menu = nullptr;
    }

    g.log << "destructor constructor finishing..." << endl;
}

bool Gamerecord::testConnection()
{
    bool result;
    string line;

    // GET request header
    system("curl -sI https://cs.csub.edu/~mkausch/3350/cosmo_connect.php?op_type=read > tempheader.txt"); 
    ifstream hs_file("tempheader.txt");
    getline(hs_file, line);
    size_t found = line.find("200"); // return true of a 200 code was found
    if (found != std::string::npos) {
        // g.log << "200 was found at position: " << found << endl;
        result = true;
    }
    else {
        // g.log << "couldn't find 200" << endl;
        result = false;
    }

    hs_file.close();
    system("rm tempheader.txt");

    return result;
}

// reads highscores from local file and loads them into the scores vector
bool Gamerecord::getRecord()
{
    ifstream fin;
    bool result = true;

    string filename;
    if (testConnection()) {
            // open remote highscore file
        system("curl -s https://cs.csub.edu/~mkausch/3350/cosmo_connect.php?op_type=read > remote-Highscore.txt"); 
        filename = "remote-Highscore.txt";
        g.log << "using remote..." << endl;
        // fin.open("remote-Highscore.txt");        
    }

    if (!fin){
        g.log << "error, bad connection with remote" << endl;
        g.log << "attempting to open local Highscore.txt file" << endl;
        filename = "Highscore.txt";
        // fin.open("Highscore.txt");
    }
    
    fin.open(filename);

	if (!fin) {
        g.log << "error opening local Highscore file" << endl;
		result = false;
	} else {
        string user;
        int score;
        int count = 0;

        while (fin >> user >> score) {
            // g.log << user << " : " << score << endl;
            HighScore entry = HighScore(user, score);
            scores.push_back(entry);
            count++;
        }

        if (count < 10) {
            genFakeNames();
        }
    }

    sortRecord();

    fin.close();
    if (filename == "remote-Highscore.txt") {
        system("rm remote-Highscore.txt");
    }

	return result;
}

// writes top ten records to disk
void Gamerecord::writeRecord()
{
    ostringstream temp;
	ofstream fout("Highscore.txt");
    bool good_conn = testConnection();

	if (!fout) {
		throw "could not write to local highscore file";
	}

	// only write top 10 scores
    /* example url:
    * https://cs.csub.edu/~mkausch/3350/cosmo_connect.php?op_type=write&n1=11&t1=mike&n2=20&t2=mike&n3=30&t3=mike&n4=40&t4=mike&n5=50&t5=mike&n6=60&t6=mike&n7=70&t7=mike&n8=80&t8=mike&n9=90&t9=mike&n10=100&t10=mike
    */
    temp << "curl -s \"https://cs.csub.edu/~mkausch/3350/cosmo_connect.php?op_type=write";
	
    for (int i = 0; i < scores.size(); i++) {
		fout << scores[i].uname << "\t" << scores[i].score;
        if (i != (scores.size() - 1)) {
            fout << endl;
        }
        temp << "&t" << i+1 << "=" << scores[i].uname << "&n" << i+1 << "=" << scores[i].score;
	}

    temp << "\" > response.txt";
    string tstr = temp.str();

    g.log << "<<< URL  >>> " << endl;
    g.log << temp.str() << endl;
    
    system(tstr.c_str());
    ifstream fin("response.txt");
    if (!fin) {
        g.log << "error, no response" << endl;
    } else {
        string l;
        getline(fin, l);
        g.log << "line: " << l << endl;
        size_t found = l.find("done");
        if (found != std::string::npos) {
            g.log << "transmitted successfully" << endl;
        } else {
            g.log << "error transmitting file, no response after write" << endl;
        }

        fin.close();
        system("rm response.txt");
    }
	g.log << "Highscore.txt written successfully...\n";
}

string removeSpaces(string s)
{
    const string SPACE = " ";
    size_t last = s.find_last_not_of(SPACE);
    return (s.substr(0, last+1));
}

void Gamerecord::submitRecord(int s)
{
    g.log << "originally: |" << string(gamer) << "|" << endl;
    string gamer_tag = removeSpaces(string(gamer));
    g.log << "now: |" << gamer_tag << "|" << endl;

	if (user_score == nullptr) {
		user_score = new HighScore(gamer_tag, s);
        
	} else {
        delete user_score;
		user_score = new HighScore(gamer_tag, s);
	}

    if (user_score) {
        g.log << user_score->uname << "'s score is " 
                << user_score->score << endl;

        g.log << "adding to records..." << endl;
        addRecord(*user_score);
        for (int i = 0; i < scores.size(); i++) {
            if ((scores[i].score == user_score->score) && 
                    (scores[i].uname == user_score->uname))
                place = i;
        }
    }
	scores.pop_back();	// delete the lowest

    if (isHighScore()) {
        highscore = s;
    }

    g.log << "making new menu... " << endl;
    makeMenu(); // make the menu with 11 people

}

// sorts the records
void Gamerecord::sortRecord()
{
	sort(scores.begin(), scores.end());	// sorts in ascending
	reverse(scores.begin(), scores.end());	// changes to descending

    g.log << "finished sorting scores\n";
}

// adds a record then sorts the records
void Gamerecord::addRecord(HighScore s)
{
	scores.push_back(s);
    g.log << "finished adding score\n";
	sortRecord();
}

// tests to see if the user's score is equal to the high score
bool Gamerecord::isHighScore()
{
	// return (place == (*user_score).score);
    if (place == -1)
        return false;
    
    return (place == 0);     
}

bool Gamerecord::isTopTen()
{
    if (place == -1)
        return false;

	return (place >= 0 && place < 11);
}

void Gamerecord::genFakeNames()
{
	string names[10] = {"Amy", "Mike", "Grayson", "Gavin", "Dan", 
						"Huaiyu", "Ailand", "Newb1234", "tehBest", "Gordon"};
	int nums[10] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
	int i = 0;

	while (scores.size() < 10) {
		// make sure the dummy value is unique,.
		auto it = find(scores.begin(), scores.end(), nums[i]);
		if (it == scores.end())
			scores.push_back(HighScore(names[i],nums[i]));
		i++;
	}
	
	sortRecord();
}

void Gamerecord::makeMenu()
{
    ostringstream temp;
    // string name_list[scores.size()];
    string name_list[10];

    for (int i = 0; i < 10; i++) {
        temp << left << setw(12) << scores[i].uname 
             << right << setw(60) << scores[i].score;
        g.log << left << setw(12) << scores[i].uname 
             << right << setw(60) << scores[i].score << endl;

        name_list[i] = temp.str();
        temp.str("");
    }
    
    // allocate mem for new menu
    if (!hs_menu) {
        // g.log << "menu set to null so far" << endl;
        hs_menu = new Menu(10, 300.0f, 300.0f, 
                            g.xres/2.0f, g.yres/2.0f, name_list, 1);
    } else if (hs_menu) {   // make a new menu
        // g.log << "deleting the prev menu" << endl;
        delete hs_menu;
        // g.log << "now making a new menu" << endl;
        hs_menu = new Menu(10, 300.0f, 300.0f, 
                            g.xres/2.0f, g.yres/2.0f, name_list, 1);
    }

    // g.log << "checking if high score" << endl;
    if (isHighScore()) {
        // g.log << "setting high score color" << endl;
        // (hs_menu->t_boxs[0]).setColor((int)124,(int)10,(int)2);
        (hs_menu->t_boxs[0]).color[0] = (unsigned char)124;
        (hs_menu->t_boxs[0]).color[1] = (unsigned char)10;
        (hs_menu->t_boxs[0]).color[2] = (unsigned char)2;


    } else if (isTopTen()) {
        // g.log << "setting top ten color" << endl;
        // (hs_menu->t_boxs[place]).setColor((int)178,(int)222,(int)39);
        (hs_menu->t_boxs[place]).color[0] = (unsigned char)178;
        (hs_menu->t_boxs[place]).color[1] = (unsigned char)222;
        (hs_menu->t_boxs[place]).color[2] = (unsigned char)39;
    }

    // set 11th element to yellow (will be deleted)
    // g.log << "setting 11th element color" << endl;
    // if (scores.size() == 11) {

    //     // (hs_menu->t_boxs[scores.size()-1]).setColor((int)189,(int)195,(int)199);
    //     (hs_menu->t_boxs[scores.size()-1]).color[0] = (unsigned char)189;
    //     (hs_menu->t_boxs[scores.size()-1]).color[1] = (unsigned char)195;
    //     (hs_menu->t_boxs[scores.size()-1]).color[2] = (unsigned char)199;
    // }
    // g.log << "finished making menu...\n" << endl;
}

SoundBar::SoundBar(float * _val, float _x, float _y, std::string _bn_)
    : value(_val), pos{_x,_y,0}, bar_name(_bn_)
{
    const float total_box_length = 200.0f;
    const float button_box_width = 20.0f; 
    const float sound_bar_thickness = 60.0f;
    const float line_thickness = 5.0f;
    const float line_length = total_box_length 
                                            -((2*button_box_width) + (10*2));
    const float half_length = line_length / 2.0f;


    bckgr.setDim(total_box_length, sound_bar_thickness);
    bckgr.setPos(pos[0], pos[1], pos[2]);
    bckgr.setColor(47, 61, 64);

    const float left_pos = bckgr.pos[0]-line_length+button_box_width;
    const float right_pos = bckgr.pos[0]+line_length-button_box_width;
    slider_left_stop_pos = left_pos + 2*button_box_width;
    slider_right_stop_pos = right_pos - 2*button_box_width;


    boarder.setDim(total_box_length+BSIZE, sound_bar_thickness + BSIZE);
    boarder.setPos(pos[0], pos[1], pos[2]);
    boarder.setColor(69, 85, 89);

    leftb.setDim(button_box_width, button_box_width);
    leftb.setPos(left_pos, bckgr.pos[1], pos[2]);
    leftb.setColor(61, 90, 115);
    leftb.id=0;

    rightb.setDim(button_box_width, button_box_width);
    rightb.setPos(right_pos, bckgr.pos[1], pos[2]);
    rightb.setColor(61, 90, 115);
    rightb.id=1;

    line.setDim(line_length, line_thickness);
    line.setPos((bckgr.pos[0]), bckgr.pos[1], pos[2]);
    line.setColor(27, 27, 27);

    slider.setDim(button_box_width, 30);
            //    (beginnning of line ) + (proportion of volume to full vol)
    slider_position = getSliderPosition();
    slider.setPos(slider_position,pos[1],0); 
    slider.setColor(69,85,89);

    words[0] = "<";
    words[1] = ">";
    
    // button text
    texts[0].bot = leftb.pos[1]-4;
    texts[0].left = leftb.pos[0];
    texts[0].center = 1;

    texts[1].bot = rightb.pos[1]-4;
    texts[1].left = rightb.pos[0];
    texts[1].center = 1;

    // bar title
    texts[2].bot = bckgr.pos[1] + 5;
    texts[2].left = bckgr.pos[0];
    texts[2].center = 1;
}

void SoundBar::draw()
{
    // draw boarder

    glColor3ubv(boarder.color);

    glPushMatrix();
    glTranslatef(boarder.pos[0], boarder.pos[1], boarder.pos[2]);
    glBegin(GL_QUADS);
        glVertex2f(-boarder.w, -boarder.h);
        glVertex2f(-boarder.w,  boarder.h);
        glVertex2f( boarder.w,  boarder.h);
        glVertex2f( boarder.w, -boarder.h);
    glEnd();
    glPopMatrix();

    // draw mainbox

    glColor3ubv(bckgr.color);

    glPushMatrix();
    glTranslatef(bckgr.pos[0], bckgr.pos[1], bckgr.pos[2]);
    glBegin(GL_QUADS);
        glVertex2f(-bckgr.w, -bckgr.h);
        glVertex2f(-bckgr.w,  bckgr.h);
        glVertex2f( bckgr.w,  bckgr.h);
        glVertex2f( bckgr.w, -bckgr.h);
    glEnd();
    glPopMatrix();

    // draw line

    glColor3ubv(line.color);

    glPushMatrix();
    glTranslatef(line.pos[0], line.pos[1], 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-line.w, -line.h);
        glVertex2f(-line.w,  line.h);
        glVertex2f( line.w,  line.h);
        glVertex2f( line.w, -line.h);
    glEnd();
    glPopMatrix();

    // draw leftb

    glColor3ubv(leftb.color);

    glPushMatrix();
    glTranslatef(leftb.pos[0], leftb.pos[1], 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-leftb.w, -leftb.h);
        glVertex2f(-leftb.w,  leftb.h);
        glVertex2f( leftb.w,  leftb.h);
        glVertex2f( leftb.w, -leftb.h);
    glEnd();
    glPopMatrix();

    // draw rightb

    glColor3ubv(rightb.color);

    glPushMatrix();
    glTranslatef(rightb.pos[0], rightb.pos[1], 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-rightb.w, -rightb.h);
        glVertex2f(-rightb.w,  rightb.h);
        glVertex2f( rightb.w,  rightb.h);
        glVertex2f( rightb.w, -rightb.h);
    glEnd();
    glPopMatrix();

    // draw slider

    glColor3ubv(slider.color);

    glPushMatrix();
    glTranslatef(slider.pos[0], slider.pos[1], 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-slider.w, -slider.h);
        glVertex2f(-slider.w,  slider.h);
        glVertex2f( slider.w,  slider.h);
        glVertex2f( slider.w, -slider.h);
    glEnd();
    glPopMatrix();

    ggprint8b(&texts[0], 0, 0x00ffffff, words[0].c_str());
    ggprint8b(&texts[1], 0, 0x00ffffff, words[1].c_str());
    ggprint12(&texts[2], 0, 0x00ffffff, bar_name.c_str());
}

float SoundBar::getSliderPosition()
{
    return (slider_left_stop_pos + ((*value)*(slider_right_stop_pos-
                                                        slider_left_stop_pos)));
}

// pass in mouse coords to check and see if they are within the bounds
// of the menu's text boxes
Box* SoundBar::checkButtons(float x, float y)
{
    Box * box_ptr = nullptr;

    size_t i;

        if ((x > (leftb.pos[0]-leftb.w)) &&
            (x < (leftb.pos[0]+leftb.w)) &&
            (y > (leftb.pos[1]-leftb.h)) &&
            (y < (leftb.pos[1]+leftb.h))) {
            
            box_ptr = &leftb;
        }
        else if ((x > (rightb.pos[0]-rightb.w)) &&
            (x < (rightb.pos[0]+rightb.w)) &&
            (y > (rightb.pos[1]-rightb.h)) &&
            (y < (rightb.pos[1]+rightb.h))) {
            
            box_ptr = &rightb;
        }

    return box_ptr;
}

void SoundBar::setOrigColor()
{
    leftb.setColor(61, 90, 115);
    rightb.setColor(61, 90, 115);
}

void SoundBar::setHighlight(Box * b)
{
    b->setColor(33,136,171);
}

void SoundBar::moveSliderDown()
{
    // if (slider_position > slider_left_stop_pos) {
    if (*value >= 0.18) {
        *value -= 0.1f;
        slider_position = getSliderPosition();
        slider.setPos(slider_position, slider.pos[1], slider.pos[2]);
    } else {
        *value = 0;
        slider_position = getSliderPosition();
        slider.setPos(slider_position, slider.pos[1], slider.pos[2]);
    }
}

void SoundBar::moveSliderUp()
{

    // if (slider_position < slider_right_stop_pos) {
    if (*value < 0.82f) {
        *value += 0.1f;
        slider_position = getSliderPosition();
        slider.setPos(slider_position, slider.pos[1], slider.pos[2]);
    } else {
        *value = 1;
        slider_position = getSliderPosition();
        slider.setPos(slider_position, slider.pos[1], slider.pos[2]);
    }
}


Bomb::Bomb()
{
    curr_radius = start_radius = 6;
    stop_radius = 450;
    pos[0] = g.xres/2.0;
    pos[1] = g.yres/2.0;
    pos[2] = 0;
    setPos(g.xres/2.0f, g.yres/2.0f, 0);
    setColor(color, 255, 91, 20);
    setColor(launch_color, 255, 255, 255);
    // is_gone = false;
    is_thrown = false;
    is_exploding = false;
    num_bombs = 99;
    w = 6;
    h = 6;
    // button text
    // message.bot = pos[1];
    // message.left = pos[0];
    message.center = 1;
    tex = &g.bomb_texture;
    text_accel = -1;
}

Bomb::~Bomb()
{
    // destructor
    if (bomb_timer) {
        delete bomb_timer;
        bomb_timer = nullptr;
    }
}

void Bomb::draw()
{
    

    if (is_thrown && !is_exploding) {

        int size = 50;

        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, *(bomb.tex));
        // glColor3ub(color[0], color[1], color[2]);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTranslatef(pos[0], pos[1], pos[2]);
        glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(0,0);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2f(0, size);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2f(size, size);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(size, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        glPopMatrix();

        /*
            glColor3ubv(launch_color);


            glPushMatrix();
            glTranslatef(pos[0], pos[1], pos[2]);
            glBegin(GL_QUADS);
                glVertex2f(-6, -6);
                glVertex2f(-6,  6);
                glVertex2f( 6,  6);
                glVertex2f( 6, -6);
            glEnd();
            glPopMatrix();
        */
    } else if (is_exploding) {
        float angle1 = (2 * PI * 0)/100;
        float angle2 = (2 * PI * 1)/100;
        float vert1[2] = {cos(angle1) * curr_radius, sin(angle1) * curr_radius};
        float vert2[2] = {cos(angle2) * curr_radius, sin(angle2) * curr_radius};
        float center[3] = {pos[0], pos[1], pos[2]};
        float orig_vert[2] = {vert1[0], vert1[1]};


        // g.log << "center: " << pos[0] << "," << pos[1] << "," << pos[2] << endl; 

        glPushMatrix();
        // glEnable(GL_ALPHA_TEST);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        // glAlphaFunc(GL_GREATER, 0.0f);
        glTranslatef(center[0], center[1], center[2]);
        glBegin(GL_TRIANGLE_FAN);
        glColor4ub(204, 204, 0, 255);
        glVertex2f(0,0);
        for (int i = 1; i < 102; i++) {
            // glColor3ubv(color);
            glColor4ub(color[0], color[1], color[2], 50);
            glVertex2f(vert1[0], vert1[1]);
            // g.log << "vert1: " << vert1[0] << "," << vert1[1] << endl; 

            glVertex2f(vert2[0], vert2[1]);
            // g.log << "vert2: " << vert2[0] << "," << vert2[1] << endl; 

            glColor4ub(204, 204, 0, 255);
            glVertex2f(0, 0);

            // move vert2 to vert1 vertices
            vert1[0] = vert2[0];
            vert1[1] = vert2[1];

            // calc new vert for vert2
            angle1 = (2 * PI * i)/100;
            vert2[0] = cos(angle1) * curr_radius;
            vert2[1] = sin(angle1) * curr_radius;
            if (i == 101) {
                vert2[0] = orig_vert[0];
                vert2[1] = orig_vert[1];
            }
        }
        glEnd();
        // glDisable(GL_ALPHA_TEST);
        glDisable(GL_BLEND);
        glPopMatrix();
    }

    if (is_thrown && show_message) {
        message.left = pos[0];
        ggprint16(&message, 0, 0x00ffffff, "LAUNCHING BOMB!!!");
        if (g.state != PAUSE) {
            if (text_vel > -16) {
                text_vel += text_accel;
                message.bot += text_vel;
            }
        }
    }

}

void Bomb::setColor(unsigned char * col, int r, int g, int b)
{
    col[0] = (char)r;
    col[1] = (char)g;
    col[2] = (char)b;
}

void Bomb::toggleDisplayMessage()
{
    if (!show_message) {
        show_message = true;
    }
}

void Bomb::setPos(float x, float y, float z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void Bomb::explode()
{
    if (bomb_timer) {
        delete bomb_timer;
        bomb_timer = nullptr;
    }

    if (curr_radius >= stop_radius) {
        // is_gone = true;
        is_exploding = false;
        // g.log << "is_exploding to false" << endl;
        is_thrown = false;
        show_message = false;
        // message.bot = (g.yres*3)/4;
        curr_radius = start_radius;

    } else {
        // g.log << "is_exploding to true" << endl;
        is_exploding = true;
        // is_gone = false;
        curr_radius += 12;
        updateHitbox();
    }
}

void Bomb::move()
{
    if (bomb_timer && !(bomb_timer->isDone())) {
        pos[0] += 7;
    } else if (bomb_timer && bomb_timer->isDone()) {
        explode();
    } else if (is_exploding) {
        explode();
    }
}

void Bomb::launch()
{
    // if (!is_thrown && num_bombs > 0) {
    if (!is_thrown && (tos.energy > 20)) {
        is_thrown = true;
        tos.energy -= 20;
        setPos(tos.pos[0],tos.pos[1],tos.pos[2]);
        message.left = pos[0];
        message.bot = pos[1] - 10;
        text_vel = 24;
        bomb_timer = new Timer(0.7);
#ifdef USE_OPENAL_SOUND
        sounds.bombExplosion();
#endif
        // num_bombs--;
    } else {
        // g.log << "no bombs or one already active" << endl;
    }
}

void Bomb::updateHitbox()
{
    w = h = 2*curr_radius;
}

// hitbox collision with item class
// check this first before checking resultant to bomb center from vertices
bool Bomb::hitboxCollision(Item & itm)
{
    bool x = (((pos[0]+w)-(itm.pos[0]-itm.w))*((pos[0]-w)-(itm.pos[0]+itm.w))) < 0;
  	bool y = (((pos[1]+h)-(itm.pos[1]-itm.h))*((pos[1]-h)-(itm.pos[1]+itm.h))) < 0;
  	
    return x&&y;
}

// hitbox collision with ent class
// check this first before checking resultant to bomb center from vertices
bool Bomb::hitboxCollision(Entity & ent)
{
    bool x = (((pos[0]+w)-(ent.pos[0]-ent.dim[0]))*((pos[0]-w)-(ent.pos[0]+ent.dim[0]))) < 0;
  	bool y = (((pos[1]+h)-(ent.pos[1]-ent.dim[1]))*((pos[1]-h)-(ent.pos[1]+ent.dim[0]))) < 0;
  	
    return x&&y;
}

// tests collision with hitbox first then itm's corners 
bool Bomb::collision(Item & itm)
{
    // g.log << "checking bomb collision with " << &itm << endl;
    if (!hitboxCollision(itm)) {
        // g.log << "not a hitbox collision on " << &itm << endl;
        return false;
    }

    // g.log << "hitbox collision on " << endl;
/*
    This would be for center of the box:

    double xvec = itm.pos[0]-pos[0];
    double yvec = itm.pos[1]-pos[1];

    We're going to calculate it for the 4 corners though so we need to
    do +/- the width and height

        0               3
        *---------------*
        |               |
        |               |
        |               |
        *---------------*
        1               2

*/

    // double xvec[4] = { itm.pos[0] - (itm.w/2.0f) - pos[0],
    //                     itm.pos[0] - (itm.w/2.0f) - pos[0],
    //                     (itm.pos[0] + (itm.w/2.0f)) - pos[0],
    //                     (itm.pos[0] + (itm.w/2.0f)) - pos[0] }

    double xvec[4] = { itm.pos[0] - (itm.w/2.0f) - pos[0],
                        xvec[0],
                        xvec[0]+itm.w,
                        xvec[2]};

    // double yvec[4] = { itm.pos[1] + (itm.h/2.0f) - pos[1],
    //                     itm.pos[1] - (itm.h/2.0f) - pos[1],
    //                     (itm.pos[1] - (itm.h/2.0f)) - pos[1],
    //                     (itm.pos[1] + (itm.h/2.0f)) - pos[1] };

    double yvec[4] = { itm.pos[1] + (itm.h/2.0f) - pos[1],
                    yvec[0] - itm.h,
                    yvec[1],
                    yvec[0]};

    double resultant;

    for (int i = 0; i < 4; i++) {
        resultant = sqrt(pow(xvec[i],2) + pow(yvec[i],2));
        if (resultant <= curr_radius)
            return true;
    }

    return false;
}

// tests collision with hitbox first then ent's corners 
bool Bomb::collision(Entity & ent)
{
    if (!hitboxCollision(ent)) {
        return false;
    }

/*
    This would be for center of the box:

    double xvec = itm.pos[0]-pos[0];
    double yvec = itm.pos[1]-pos[1];

    We're going to calculate it for the 4 corners though so we need to
    do +/- the width and height

        0               3
        *---------------*
        |               |
        |               |
        |               |
        *---------------*
        1               2

*/

    double xvec[4] = { ent.pos[0] - (ent.dim[0]/2.0f) - pos[0],
                        xvec[0],
                        xvec[0]+ent.dim[0],
                        xvec[2]};

    double yvec[4] = { ent.pos[1] + (ent.dim[1]/2.0f) - pos[1],
                    yvec[0] - ent.dim[1],
                    yvec[1],
                    yvec[0]};

    double resultant;

    for (int i = 0; i < 4; i++) {
        resultant = sqrt(pow(xvec[i],2) + pow(yvec[i],2));
        if (resultant <= curr_radius)
            return true;
    }

    return false;
}

// TODO
FeatureModeBlock::FeatureModeBlock()
{
    tex = nullptr;
    setPos(g.xres*0.75, (g.yres-info_board_1.pos[1])*0.25+75, 0);
    setDim(200, 75);
}
void FeatureModeBlock::setTexture()
{
    if (g.fstate == REGULAR) {
        tex = nullptr;
    } else if (g.fstate == MKAUSCH) {
        tex = &g.mkfm_texture;
    } else if (g.fstate == DTORRES) {
        tex = &g.dtfm_texture;
    } else if (g.fstate == HZHANG) {
        tex = &g.hzfm_texture;
    } else if (g.fstate == APARRIOTT) {
        tex = &g.apfm_texture;
    }
}

void FeatureModeBlock::draw()
{
    if (tex != nullptr) {
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, *tex);
        // glColor3ub(color[0], color[1], color[2]);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
        glTranslatef(pos[0], pos[1], pos[2]);
        glBegin(GL_QUADS);

            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(-w, -h);

            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(-w,  h);
            
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f( w,  h);
            
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f( w, -h);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        glPopMatrix();
    }
}

void initializeLogFile(ofstream & fout)
{
    fout.open("cosmo_log.log");
    if (!fout)
        cerr << "couldn't open log file" << endl;
    const auto now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
    // std::g.log << "The system clock is currently at " << std::ctime(&t_c);
    fout << "******************************************" << endl;
    fout << "STARTING LOG " << std::ctime(&t_c) << endl;
    fout << "*******************************************" << endl << endl;
}