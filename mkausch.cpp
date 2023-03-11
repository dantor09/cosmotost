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
//#include "hzhang.h"
#include "aparriott.h"


using namespace std;

#define BSIZE 5
#define PADDING 20

// #define USE_OPENAL_SOUND



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

Timer::Timer(double sec) : duration(sec), pause_duration(0.00), 
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
    current_track = -1;  // starting track number at splash screen
    is_music_paused = false;
    user_pause = false;
    is_intro = is_game = false;

    //0     "bullet_fire.wav",
    //1 "Edzes-64TheMagicNumber-intro8kHz.wav",
    //2 "Edzes-64TheMagicNumber-loop8kHz.wav",
    //3 "VolkorX-Enclave8kHz.wav" };


    // make individual buffers of all sounds
    alBuffers[0] = alutCreateBufferFromFile(build_song_path(sound_names[0]).c_str());
    alBuffers[1] = alutCreateBufferFromFile(build_song_path(sound_names[1]).c_str());
    alBuffers[2] = alutCreateBufferFromFile(build_song_path(sound_names[2]).c_str());
    alBuffers[3] = alutCreateBufferFromFile(build_song_path(sound_names[3]).c_str());

    // songBuffers[0] = alBuffers[3];
    buffersDone = buffersQueued = 0;
    
    // generate number of sources
    alGenSources(NUM_SOUNDS, alSources);    // keep individual sources for now
    alGenSources(1, &menuQueueSource);

    alSourceQueueBuffers(menuQueueSource, 1, (alBuffers+1));
    alSourcef(menuQueueSource, AL_GAIN, 1.0f);
    alSourcef(menuQueueSource, AL_PITCH, 1.0f);
    // alGenSources(1, &songQueueSource);

    //   [[------- OLD -----------]]
    //Generate a source, and store it in a buffer.
    // set sfx/songs to not loop
    int non_looping_sounds = 2;
    for (int i = 0; i < non_looping_sounds; i++) {
        alSourcei(alSources[i], AL_BUFFER, alBuffers[i]);
        alSourcef(alSources[i], AL_GAIN, 1.0f);
        alSourcef(alSources[i], AL_PITCH, 1.0f);
        alSourcei(alSources[i], AL_LOOPING, AL_FALSE);
    }

    // make all songs to loop
    for (int i = 2; i < NUM_SOUNDS; i++) {
        alSourcei(alSources[i], AL_BUFFER, alBuffers[i]);
        alSourcef(alSources[i], AL_GAIN, 1.0f);
        alSourcef(alSources[i], AL_PITCH, 1.0f);
        alSourcei(alSources[i], AL_LOOPING, AL_TRUE);
    }
    //   [[------- END OLD -----------]]


    // check for errors after setting sources
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

    alDeleteSources(1, &menuQueueSource);

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

void Sound::rewind_game_music()
{
    alSourceStop(alSources[3]);
    alSourceRewind(alSources[3]);
    alSourcePlay(alSources[3]);
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


bool Sound::check_intro_buffer_done()
{
    reset_buffer_done();
    alGetSourcei(menuQueueSource, AL_BUFFERS_PROCESSED, &buffersDone);
    cerr << "checking intro buffer done, buffers is: " << buffersDone << endl;
    return (buffersDone == 1);
}

// resets buffers_done variable for further checks
void Sound::reset_buffer_done()
{
    buffersDone = 0;
}

// unqueue's intro beat so that only loop track is in the buffer queue
// loops buffer queue at this point
void Sound::loop_intro()
{

    alSourceStop(menuQueueSource);
    alSourceRewind(menuQueueSource);
    alSourcePlay(alSources[2]);

}

void Sound::setup_game_mode()
{
    // change bools for music state
    is_intro = false; is_game = true;

    // stop both the intro and loop if either are playing
    alSourceStop(alSources[2]);
    alSourceStop(menuQueueSource);
    alSourceRewind(alSources[2]);
    alSourceRewind(menuQueueSource);
    
    // play the game song
    alSourcePlay(alSources[3]);
}

void Sound::play_start_track()
{
    // stop game music if it's playing
    if (is_game == true) {
        alSourceStop(alSources[3]);
        alSourceRewind(alSources[3]);
    }

    is_intro = true; is_game = false;

    // begin playing menu music
    alSourcePlay(menuQueueSource);
}

// returns song names, only 2 songs for now
string Sound::get_song_name()
{
    string name;
    if (is_intro) {
        name = "Edzes-64TheMagicNumber";
    }
    if (is_game) {
        name = "VolkorX-Enclave";
    }
    return name;
}

// pauses song (when going to pause menu for instance)
void Sound::pause()
{
    if (!is_music_paused) {
        is_music_paused = true;
        alSourcePause(alSources[3]);
    }
}

// unpauses
void Sound::unpause()
{
    if (!user_pause) {
        if (is_music_paused) {
            is_music_paused = false;
            alSourcePlay(alSources[3]);
        }
    }
}

// separate pause state for when user explicity mutes music
void Sound::toggle_user_pause()
{
    user_pause = (user_pause == true) ? false : true;
    if (user_pause)
        pause();
    else
        unpause();
}

// getter to return the pause state
bool Sound::get_pause()
{
    return is_music_paused;
}

#endif

HealthBar::HealthBar(const Item & _itm_, float x, float y)
{
    // maybe put max_health of each enemy type in case were going to 
    // use this healthbar for the boss as well
    total.set_dim(75,10);
    total.set_pos(x, y, 0);
    total.set_color(255,0,0);   // set it to red

    health.set_dim(total.w,total.h);
    health.set_pos(total.pos[0],total.pos[1],total.pos[2]);
    health.set_color(0,255,0);

    text.bot = total.pos[1]-5;
    text.left = total.pos[0];
    text.center = 1;

    itm = &_itm_;
    
}

void HealthBar::draw()
{
    
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
        glVertex2f( (((float)(itm->HP))/(itm->starting_hp))*2.0f*health.w,  health.h);
        glVertex2f( (((float)(itm->HP))/(itm->starting_hp))*2.0f*health.w, -health.h);
    glEnd();
    glPopMatrix();

    ggprint8b(&text, 0, 0x00000000, "%i/%i  Lives: %i", itm->HP, itm->starting_hp, itm->lives);
}

// modified from hzhang's file by mkausch
bool Entity::collision(Item & a) 
{
    // for (x0,y0,x1,y1)and(x2,y2,x3,y3) squares
    // if collison -->(x0-x3)*(x1-x2)<0
    // same for y
    bool x = (((pos[0]+dim[0])-(a.pos[0]-a.w))*((pos[0]-dim[0])-(a.pos[0]+a.w))) < 0;
  	bool y = (((pos[1]+dim[1])-(a.pos[1]-a.h))*((pos[1]-dim[1])-(a.pos[1]+a.h))) < 0;
  	return x&&y;
}

void Entity::set_HP(int life)
{
    HP = life;
}

void Entity::set_damage(int x)
{
    damage = x;
}

void Entity::HPdamage(Item & a) {
    HP = HP - a.damage;
}

bool Entity::HP_check()
{
    return (HP <= 0);
}

void Item::HPdamage(Entity & e)
{
    HP = HP - e.damage;
}

BlockyForky::BlockyForky()
{
    srand(time(NULL));
    set_dim(25.0f, 100.0f);
    set_rand_color();
    set_rand_position();
    set_acc(0.0f,-0.25f,0.0f);
    set_vel(0.0f, -4.0f, 0.0f);
    set_damage(20);
    starting_hp = 200;
    set_HP(starting_hp);
    point = starting_hp;
    was_hit = false;
    lives = 2;
}

BlockyForky::~BlockyForky()
{

}

void BlockyForky::set_rand_position()
{
    static int pm_dir = 1;
    float curr_player_xpos = tos.pos[0];
    int delta_from_xpos = rand() % 50;
    float block_xpos = curr_player_xpos + (delta_from_xpos * pm_dir);

    // set to be this new random position situated near the player char
    // that is above the yres and out of view of the screen
    set_pos(block_xpos, g.yres+h,0);

    // if this block was generated in front of the player then
    // next time make it randomly behind the player (it'll keep switching) 
    pm_dir *= -1;
    
}

void BlockyForky::set_rand_color()
{
    // colors based on color scheme defined at the bottom
    // int color[5][3] = {{61, 89, 114},
    //                     {47, 61, 63},
    //                     {68, 84, 89},
    //                     {40, 63, 61},
    //                     {24, 38, 36}};

    static int color[5][3] =   {{242, 4, 159},
                        {4, 177, 216},
                        {4, 216, 78}, 
                        {242, 202, 4},
                        {242, 135, 4}};
    static int index = rand() % 5;
    set_color(color[index][0], color[index][1], color[index][2]);
    index = (index + 1) % 5;
}

void BlockyForky::draw()
{
    // draw item
    if (is_alive()) {
        set_rand_color();
        glPushMatrix();
        glColor3ub(color[0], color[1], color[2]);
        glTranslatef(pos[0], pos[1], pos[2]);
        glBegin(GL_QUADS);
                glVertex2f(-w, -h);
                glVertex2f(-w,  h);
                glVertex2f( w,  h);
                glVertex2f( w, -h);
        glEnd();
        glPopMatrix();
    }
    
}

void BlockyForky::reset()
{
    if (HP_check() && (lives > 0)) {
        lives--;
        HP = starting_hp;   // give back full health
    }

    set_vel(0.0f, -4.0f, 0.0f);
    set_rand_position();    // put at a new random position
    was_hit = false;

}

bool BlockyForky::did_damage()
{
    return was_hit;
}

void BlockyForky::move()
{
    if (is_alive()) {
        pos[0] += vel[0];
        pos[1] += vel[1];
        pos[2] += vel[2];
        vel[0] += acc[0];
        vel[1] += acc[1];
        vel[2] += acc[2];
    }


}

void Item::HPdamage(BlockyForky & bf)
{
    if (!bf.did_damage()) {
        HP = HP - bf.damage;
        bf.set_hit();
    }
}

bool BlockyForky::is_alive()
{
    return (lives > 0);
}

void BlockyForky::set_hit()
{
    was_hit = true; 
}

/*
                Color Scheme


/* Color Theme Swatches in Hex 
.Top-view-of-many-colorful-balls-in-ball-pool-at-indoors-playground-1-hex { color: #F2059F; }
.Top-view-of-many-colorful-balls-in-ball-pool-at-indoors-playground-2-hex { color: #04B2D9; }
.Top-view-of-many-colorful-balls-in-ball-pool-at-indoors-playground-3-hex { color: #04D94F; }
.Top-view-of-many-colorful-balls-in-ball-pool-at-indoors-playground-4-hex { color: #F2CB05; }
.Top-view-of-many-colorful-balls-in-ball-pool-at-indoors-playground-5-hex { color: #F28705; }

 Color Theme Swatches in RGBA 
.Top-view-of-many-colorful-balls-in-ball-pool-at-indoors-playground-1-rgba { color: rgba(242, 4, 159, 1); }
.Top-view-of-many-colorful-balls-in-ball-pool-at-indoors-playground-2-rgba { color: rgba(4, 177, 216, 1); }
.Top-view-of-many-colorful-balls-in-ball-pool-at-indoors-playground-3-rgba { color: rgba(4, 216, 78, 1); }
.Top-view-of-many-colorful-balls-in-ball-pool-at-indoors-playground-4-rgba { color: rgba(242, 202, 4, 1); }
.Top-view-of-many-colorful-balls-in-ball-pool-at-indoors-playground-5-rgba { color: rgba(242, 135, 4, 1); }


                            
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
