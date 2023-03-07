/********************************************************************
*
*		Program: mkausch.cpp
*		Authors: Michael Kausch (Group 1 - DAHM)
*		Date Code Originated: 2/17/23
***********************************************************************/

/***************************************************************************
*       Classes: Menu
*       Uses:
*           Creates a menu object based on num text params, size and position
******************************************************************************/

//                 		INCLUDES

#include <iostream>
#include <string>
#include </usr/include/AL/alut.h>
#include "mkausch.h"
#include "Global.h"
using namespace std;

#define BSIZE 5
#define PADDING 20



Menu::Menu(unsigned int _n_texts, 
            float w, float h, 
            float x, float y,
            std::string* _words)
    : n_texts{_n_texts}, pos{x, y, 0}
{
    // dynamially allocate boxes/rects for text display
    mainbox.set_dim(w, h);
    mainbox.set_pos(pos[0], pos[1], pos[2]);
    mainbox.set_color(47, 61, 64);
    boarder.set_dim(w+BSIZE, h+BSIZE);
    boarder.set_pos(pos[0], pos[1], pos[2]);
    boarder.set_color(69, 85, 89);
    
    
    try {
        t_boxs = new Box[n_texts];
        texts = new Rect[n_texts];
        words = new std::string[n_texts];
        

        float spacing = (2*h)/(n_texts+1);
        std::cout << "spacing: " << spacing << std::endl;

        for(int i = 0; i < n_texts; i++) {
            t_boxs[i].w = mainbox.w - PADDING;
            t_boxs[i].h = (spacing/2.0) - BSIZE;
            t_boxs[i].pos[0] = mainbox.pos[0];
            t_boxs[i].pos[1] = (pos[1]+mainbox.h)-((i+1)*spacing);
            t_boxs[i].set_color(61, 90, 115);
            t_boxs[i].id=i; // set box id for words array check
            std::cout << "t_box[" << i << "].pos[1]: " 
                        << t_boxs[i].pos[1] << std::endl;
            
            words[i] = _words[i];
            // t_boxs[i].set_text(_words[i]);

            // Leaving this here for the next poor soul that
            // comes accross this issue:

            // apparently you need to set their position every single time
            // you print them or they fall off the screen for some stupid reason
            // so this code will now be found below in the draw() function

            // texts[i].bot = mainbox.pos[1]-(i*20);
            // texts[i].left = t_boxs[i].pos[0];
            // texts[i].center = 1;
        }

    } catch (std::bad_alloc& ba) {
        // if one was allocated and not the other than delete the one that 
        if (texts) delete [] texts;
        if (t_boxs) delete [] t_boxs;
        // if (words) delete [] words;
        // print to screen for now until we have logging set up
        std::cerr << "Error allocating rectangles in Menu call\n"
                << ba.what() << '\n';
        texts = nullptr;
        t_boxs = nullptr;
        // words = nullptr;
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

    for(size_t i = 0; i < n_texts; i++) {
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

    // draw texts - need to pass in texts still; this is only for testing
    
    // for(int i = 0; i < n_texts; i++) {
    //     ggprint8b((texts+i), 16, 0x00FFFFFF, "c0sm0t0asT");
    //     // std::cout << "tests[" << i << "].pos[1]: " << texts[i].pos[1] << std::endl;
    // }


    for(int i = 0; i < n_texts; i++) {
        texts[i].bot = t_boxs[i].pos[1] - 5;
        texts[i].left = t_boxs[i].pos[0];
        texts[i].center = 1;


        // r[i].bot = t_boxs[i].pos[1] - 5;
        // r[i].left = t_boxs[i].pos[0];
        // //g.r[i].center = box[i].pos[0];

        // r[i].center = 1;

        ggprint8b(texts+i, 16, 0x00ffffff, words[i].c_str());
    }
        
}

// pass in mouse coords to check and see if they are within the bounds
// of the menu's text boxes
Box* Menu::check_t_box(int x, int y)
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

    // std::cout << "match for " << box_ptr << " aka " << t_boxs+i << std::endl;

    return box_ptr;
}

void Menu::set_highlight(Box * b)
{
    b->set_color(33,136,171);
}


void Menu::set_orig_color()
{
    for (size_t i = 0; i < n_texts; i++) {
        t_boxs[i].set_color(61,90,115);
    }
}



void Menu::set_bcolor(int r, int g, int b) 
{
    bcolor[0] = (char)r; 
    bcolor[1] = (char)g; 
    bcolor[2] = (char)b;
}

void Menu::set_color(int r, int g, int b) 
{
    color[0] = (char)r; 
    color[1] = (char)g; 
    color[2] = (char)b;
}

void Menu::set_pos(float x, float y, float z) 
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

std::string Menu::get_info()
{
    std::ostringstream temp;
    temp <<             std::endl;

    return temp.str();
}


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

Timer::Timer(double s) : duration(s), pause_duration(0.00), 
                pause_timer(nullptr), paused(false)
{    // set starting time
    start = std::chrono::system_clock::now();
}

// delete pause timer if it were active
Timer::~Timer()
{
    cerr << "in Timer destructor\n";
    if (pause_timer){
        cerr << "deleting pause timer\n";
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
double Timer::getTime()
{
    double net_time = 0;
    std::chrono::duration<double> total_elapsed = std::chrono::system_clock::now() - start;

    if (paused)
    {
        net_time = (total_elapsed.count() - pause_duration - pause_timer->getTime());
    } else {
        net_time = (total_elapsed.count()-pause_duration);
    }

    return net_time;

}

// checks if the timer has elapsed
// true if the timer has finished
// false if the timer hasn't
bool Timer::isDone()
{
    if (duration == -1) {   // return false for count up timers
        return false;
    } else {    // return net time for countdown timers
        return (getTime() > duration);  
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
        pause_duration += pause_timer->getTime();
        delete pause_timer;
        pause_timer = nullptr;
    }
    
}

#ifdef USE_OPENAL_SOUND
Sound::Sound()
{
    
    //Buffer holds the sound information.
    init_openal();
    current_track = 1;  // starting track number at splash screen
    is_music_paused = false;
    user_pause = false;
    alBuffers[0] = alutCreateBufferFromFile("./Songs/bullet_fire.wav");
    // alBuffers[1] = alutCreateBufferFromFile("./Songs/Edzes-64TheMagicNumber16kHz.wav");
    // alBuffers[2] = alutCreateBufferFromFile("./Songs/Estrayk-TheHerSong1016kHz.wav");
    alBuffers[1] = alutCreateBufferFromFile("./Songs/VolkorX-Enclave8kHz.wav.wav");
    // alBuffers[4] = alutCreateBufferFromFile("./Songs/Quazar-FunkyStars16kHz.wav");
    // alBuffers[5] = alutCreateBufferFromFile("./Songs/XRay-Zizibum-16kHz.wav");
    // alBuffers[6] = alutCreateBufferFromFile("./Songs/Zalza-8bitTheClock16kHz.wav");
    // alBuffers[7] = alutCreateBufferFromFile("./Songs/AdhesiveWombat-8bitAdventure_16kHz.wav");

    alBuffers[0] = alutCreateBufferFromFile(build_song_path(sound_names[0]).c_str());
    alBuffers[1] = alutCreateBufferFromFile(build_song_path(sound_names[1]).c_str());
    // alBuffers[2] = alutCreateBufferFromFile(build_song_path(sound_names[2]).c_str());
    // alBuffers[3] = alutCreateBufferFromFile(build_song_path(sound_names[3]).c_str());
    // alBuffers[4] = alutCreateBufferFromFile(build_song_path(sound_names[4]).c_str());
    // alBuffers[5] = alutCreateBufferFromFile(build_song_path(sound_names[5]).c_str());
    // alBuffers[6] = alutCreateBufferFromFile(build_song_path(sound_names[6]).c_str());
    // alBuffers[7] = alutCreateBufferFromFile(build_song_path(sound_names[7]).c_str());

    alGenSources(NUM_SOUNDS, alSources);
    //Generate a source, and store it in a buffer.

    // make all songs to loop
    for (int i = 1; i < NUM_SOUNDS; i++) {
        alSourcei(alSources[i], AL_BUFFER, alBuffers[i]);
        alSourcef(alSources[i], AL_GAIN, 0.5f);
        alSourcef(alSources[i], AL_PITCH, 1.0f);
        alSourcei(alSources[i], AL_LOOPING, AL_TRUE);
    }

    // set sfx to not loop
    alSourcei(alSources[0], AL_BUFFER, alBuffers[0]);
    alSourcef(alSources[0], AL_GAIN, 1.0f);
    alSourcef(alSources[0], AL_PITCH, 1.0f);
    alSourcei(alSources[0], AL_LOOPING, AL_FALSE);
    
    if (alGetError() != AL_NO_ERROR) {
        throw "ERROR: setting source\n";
    }
}

Sound::~Sound()
{
    for (int i = 0; i < NUM_SOUNDS; i++) {
        // delete sources
        alDeleteSources(i+1, (alSources+i));
        // delete buffers
        alDeleteBuffers(i+1, (alBuffers+i));
    }

    close_openal();

}

void Sound::init_openal() 
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

void Sound::close_openal()
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

string Sound::build_song_path(string s)
{
    // format of the song
    // ./Songs/Edzes-64TheMagicNumber16kHz.wav"
    
    ostringstream path;
    string song_dir = "Songs";

    path << "./" << song_dir << "/" << s;

    return path.str();

}

void Sound::cycle_songs()
{
    int first_track = 1;    // first track in list of tracks,this will make 
                            //  it easier if they're grouped if more sfx are added
    // int starting_track = 1; // starting track when game opens at splash menu
    static int track = current_track;   // id of starting track number when game opens
	static int prev_track = -1; // prevents stopping a song that's not playing when game opens


    if (prev_track != -1)
        alSourceStop(alSources[prev_track]);

    alSourcePlay(alSources[track]);
    current_track = track;
    
    prev_track = track;

    track = (track == NUM_SOUNDS-1) ? first_track : track+1;
}

string & Sound::get_song_name()
{
    return sound_names[current_track];
}

void Sound::pause()
{
    if (!is_music_paused) {
        is_music_paused = true;
        alSourcePause(alSources[current_track]);
    }
}
void Sound::unpause()
{
    if (!user_pause) {
        if (is_music_paused) {
            is_music_paused = false;
            alSourcePlay(alSources[current_track]);
        }
    }
}

void Sound::toggle_user_pause()
{
    user_pause = (user_pause == true) ? false : true;
    if (user_pause)
        pause();
    else
        unpause();
}

bool Sound::get_pause()
{
    return is_music_paused;
}

#endif



    /*
private:
    Box total;     // a box that is proportionate to the overall hp of the Item pointed to by itm
    Box hp;     // a box that is proportionate to the size of the current hp of itm
    const Item * itm;   // item that this healthbar is attached to
    void hp_resize();   // resizes hp box based on passed on item's health
*/


HealthBar::HealthBar(const Item & _itm_)
{
    total.set_dim(150,40);
    total.set_pos(g.xres/2.0f,40.0f,0);
    total.set_color(255,0,0);   // set it to red

    hp.set_dim(total.w,total.h);
    hp.set_pos(total.pos[0],total.pos[1],total.pos[2]);
    hp.set_color(0,255,0);

    text.bot = total.pos[1] + 


}

void HealthBar::draw()
{
    // draw total box
    // then draw hp box
    // then draw letters


}



/*
                Color Scheme

.feeling-blue-1-hex { color: #3D5A73; }
.feeling-blue-2-hex { color: #2F3D40; }
.feeling-blue-3-hex { color: #455559; }
.feeling-blue-4-hex { color: #28403D; }
.feeling-blue-5-hex { color: #182625; }

.feeling-blue-1-rgba { color: rgba(61, 89, 114, 1); }
.feeling-blue-2-rgba { color: rgba(47, 61, 63, 1); }
.feeling-blue-3-rgba { color: rgba(68, 84, 89, 1); }
.feeling-blue-4-rgba { color: rgba(40, 63, 61, 1); }
.feeling-blue-5-rgba { color: rgba(24, 38, 36, 1); }

.feeling-blue-1-hsla { color: hsla(207, 30, 34, 1); }
.feeling-blue-2-hsla { color: hsla(190, 15, 21, 1); }
.feeling-blue-3-hsla { color: hsla(192, 12, 30, 1); }
.feeling-blue-4-hsla { color: hsla(172, 23, 20, 1); }
.feeling-blue-5-hsla { color: hsla(175, 22, 12, 1); }
*/