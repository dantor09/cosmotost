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
#include <math.h>

#include "mkausch.h"
#include "Global.h"
//#include "hzhang.h"
#include "aparriott.h"


using namespace std;

#define BSIZE 5
#define PADDING 20
#define PI 3.1415926535

// #define USE_OPENAL_SOUND



Menu::Menu(unsigned int _n_texts,
            float w, float h,
            float x, float y,
            std::string* _words)
    : n_texts{_n_texts}, pos{x, y, 0}
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
        std::cout << "spacing: " << spacing << std::endl;

        for(int i = 0; i < n_texts; i++) {
            t_boxs[i].w = mainbox.w - PADDING;
            t_boxs[i].h = (spacing/2.0) - BSIZE;
            t_boxs[i].pos[0] = mainbox.pos[0];
            t_boxs[i].pos[1] = (pos[1]+mainbox.h)-((i+1)*spacing);
            t_boxs[i].setColor(61, 90, 115);
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
    b->setColor(33,136,171);
}


void Menu::set_orig_color()
{
    for (size_t i = 0; i < n_texts; i++) {
        t_boxs[i].setColor(61,90,115);
    }
}



void Menu::setBcolor(int r, int g, int b)
{
    bcolor[0] = (char)r;
    bcolor[1] = (char)g;
    bcolor[2] = (char)b;
}

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

std::string Menu::getInfo()
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
int Timer::getTime(char time_code)
{
    int net_time = 0;
    int time = net_time;
    std::chrono::duration<double> total_elapsed = std::chrono::system_clock::now() - start;

    if (paused)
    {
        net_time = (total_elapsed.count() - pause_duration - pause_timer->getTime('n'));
    } else {
        net_time = (total_elapsed.count()-pause_duration);
    }

    // D.T - retrieve minutes, seconds, or net time
    // based on time_code passed in getTime parameter
    switch(time_code) {
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
    init_openal();
    current_track = -1;  // starting track number at splash screen
    is_music_paused = false;
    user_pause = false;
    is_intro = is_game = false;

    // make individual buffers of all sounds
    alBuffers[0] = alutCreateBufferFromFile(build_song_path(sound_names[0]).c_str());
    alBuffers[1] = alutCreateBufferFromFile(build_song_path(sound_names[1]).c_str());
    alBuffers[2] = alutCreateBufferFromFile(build_song_path(sound_names[2]).c_str());
    alBuffers[3] = alutCreateBufferFromFile(build_song_path(sound_names[3]).c_str());
    alBuffers[4] = alutCreateBufferFromFile(build_song_path(sound_names[4]).c_str());
    alBuffers[5] = alutCreateBufferFromFile(build_song_path(sound_names[5]).c_str());
    alBuffers[6] = alutCreateBufferFromFile(build_song_path(sound_names[6]).c_str());
    alBuffers[7] = alutCreateBufferFromFile(build_song_path(sound_names[7]).c_str());
    alBuffers[8] = alutCreateBufferFromFile(build_song_path(sound_names[8]).c_str());
    alBuffers[9] = alutCreateBufferFromFile(build_song_path(sound_names[9]).c_str());

    // songBuffers[0] = alBuffers[3];
    buffersDone = buffersQueued = 0;

    // generate number of sources
    alGenSources(NUM_SOUNDS, alSources);    // keep individual sources for now
    alGenSources(1, &menuQueueSource);

    alSourceQueueBuffers(menuQueueSource, 1, (alBuffers+1));
    alSourcef(menuQueueSource, AL_GAIN, 1.0f);
    alSourcef(menuQueueSource, AL_PITCH, 1.0f);



    //Generate a source, and store it in a buffer.
    // set sfx/songs to not loop
    int non_looping_sounds = 4;
    for (int i = 0; i < non_looping_sounds; i++) {
        alSourcei(alSources[i], AL_BUFFER, alBuffers[i]);
        alSourcef(alSources[i], AL_GAIN, 1.0f);
        alSourcef(alSources[i], AL_PITCH, 1.0f);
        alSourcei(alSources[i], AL_LOOPING, AL_FALSE);
    }

        alSourcef(alSources[2], AL_GAIN, 5.0f);
        alSourcef(alSources[3], AL_GAIN, 5.0f);

    // make all songs to loop
    for (int i = non_looping_sounds; i < NUM_SOUNDS; i++) {
        alSourcei(alSources[i], AL_BUFFER, alBuffers[i]);
        alSourcef(alSources[i], AL_GAIN, 0.25f);
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
    alSourceStop(alSources[5]);
    alSourceRewind(alSources[5]);
    alSourcePlay(alSources[5]);
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

void Sound::gun_play(int btype)
{
    static int gun_start = 6;

    cerr << "gun shooting..." << endl;
    if (btype == 1) {
        alSourcePlay(alSources[gun_start]);
    } else if (btype == 2) {
        alSourcePlay(alSources[gun_start]);
        alSourcePlay(alSources[gun_start+1]);
    } else if (btype == 3) {
        alSourcePlay(alSources[gun_start]);
        alSourcePlay(alSources[gun_start+1]);
        alSourcePlay(alSources[gun_start+2]);
    } else if (btype == 4) {
        alSourcePlay(alSources[gun_start+2]);
        alSourcePlay(alSources[gun_start+3]);
    }

}

void Sound::gun_stop()
{
    static int gun_start = 6;
    static int num_guns = 4;
    cerr << "gun not shooting..." << endl;
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

bool Sound::check_intro_buffer_done()
{
    reset_buffer_done();
    alGetSourcei(menuQueueSource, AL_BUFFERS_PROCESSED, &buffersDone);
    // cerr << "checking intro buffer done, buffers is: " << buffersDone << endl;
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
    alSourcePlay(alSources[4]);

}

void Sound::setup_game_mode()
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

void Sound::play_start_track()
{
    // stop game music if it's playing
    if (is_game == true) {
        alSourceStop(alSources[5]);
        alSourceRewind(alSources[5]);
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
    total.setDim(75,10);
    total.setPos(x, y, 0);
    total.setColor(255,0,0);   // set it to red

    health.setDim(total.w,total.h);
    health.setPos(total.pos[0],total.pos[1],total.pos[2]);
    health.setColor(0,255,0);

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
        glVertex2f( (((float)(itm->hp))/(itm->starting_hp))*2.0f*health.w,  health.h);
        glVertex2f( (((float)(itm->hp))/(itm->starting_hp))*2.0f*health.w, -health.h);
    glEnd();
    glPopMatrix();

    ggprint8b(&text, 0, 0x00000000, "%i/%i  Lives: %i", itm->hp, itm->starting_hp, itm->lives);
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

void Entity::setHP(int life)
{
    hp = life;
}

void Entity::setDamage(int x)
{
    damage = x;
}

void Entity::hpDamage(Item & a) {
    hp = hp - a.damage;
}

bool Entity::hpCheck()
{
    return (hp <= 0);
}

void Item::hpDamage(Entity & e)
{
    hp = hp - e.damage;
}

Blocky::Blocky()
{
    srand(time(NULL));
    setDim(25.0f, 100.0f);
    set_rand_color(*this);
    set_rand_position();
    setAcc(0.0f,-0.25f,0.0f);
    setVel(0.0f, -4.0f, 0.0f);
    setDamage(20);
    starting_hp = 10;
    setHP(starting_hp);
    point = starting_hp;
    was_hit = false;
    lives = 2;
    explode_done = true;

    // sub box assignment
    // assignes itself and it's mirror image (i+4 in this case)
    int angle = 80;
    int rvel = 8;
    float deg_to_rad = (PI / 180.0f);
    for (int i = 0; i < 4; i++) {
        // set dim
        sub_boxes[i].setDim(w/2.0f, h/4.0f);   // should create a box 1/8 size
        sub_boxes[i+4].setDim(w/2.0f, h/4.0f);   // should create a box 1/8 size

        // set color
        sub_boxes[i].setColor(255,0,0);    // make them red for now
        sub_boxes[i+4].setColor(255,0,0);    // make them red for now

        // set accel
        // sub_boxes[i].set_acc(0, -0.25, 0);
        // sub_boxes[i+4].set_acc(0, -0.25, 0);
        sub_boxes[i].setAcc(0, 0, 0);
        sub_boxes[i+4].setAcc(0, 0, 0);

        // set angle first so we can calc vel vectors
        sb_angles[i] = angle;
        sb_angles[i+4] = -sb_angles[i];
        angle -= 20;

        // set velocity of x and y components based on above angle
        sub_boxes[i].setVel((rvel*cos(deg_to_rad * sb_angles[i])),
                                    (rvel*sin(deg_to_rad * sb_angles[i])), 0);
        sub_boxes[i+4].setVel((rvel*cos(deg_to_rad * sb_angles[i+4])),
                                    (rvel*sin(deg_to_rad * sb_angles[i+4])), 0);
    }


    init_rotation_vel();
}

Blocky::~Blocky()
{

}

void Blocky::init_rotation_vel()
{
    // 0 the starting angle and assign random change in rotation angle
    for (int i = 0; i < 8; i++) {
        rot_angle[i] = 0;
        rot_speed[i] = -40 + (rand() % 41);
    }
}

void Blocky::set_rand_position()
{
    static int pm_dir = 1;
    float curr_player_xpos = tos.pos[0];
    int delta_from_xpos = rand() % 50;
    float block_xpos = curr_player_xpos + (delta_from_xpos * pm_dir);

    // set to be this new random position situated near the player char
    // that is above the yres and out of view of the screen
    setPos(block_xpos, g.yres+h,0);

    // if this block was generated in front of the player then
    // next time make it randomly behind the player (it'll keep switching)
    pm_dir *= -1;

}

void set_rand_color(Item & it)
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
    it.setColor(color[index][0], color[index][1], color[index][2]);
    index = (index + 1) % 5;
}

bool Blocky::sub_ScreenIn()
{
    bool subs_onscreen = false;


    for (int i = 0; i < 8; i++) {
        subs_onscreen = sub_boxes[i].screenIn();
        if (subs_onscreen)
            break;
    }

    return subs_onscreen;
}

void Blocky::draw()
{
    // static int rot_angle = 0;
    // draw item

        // reset blocky if he's out of screen

    // draw big blocky
    if (is_alive() && explode_done) {
        if (screenOut()) {
            reset();
        }

        set_rand_color(*this);
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


    } else {    // draw little blockies
        // cerr << "checking if sub boxes are in the screen...\n";
        if (sub_ScreenIn()) {

            for (int i = 0; i < 8; i++) {
                set_rand_color(sub_boxes[i]);
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
            init_rotation_vel();
            explode_done = true;
            // reset_sub_boxes();
        }
    }
}

// void Blocky::reset_sub_boxes()
// {

// }

void Blocky::reset()
{
    if (hpCheck()) {
        lives--;
        explode();
        cerr << "explode called\n";
        explode_done = false;
        if (lives > 0) {
            hp = starting_hp;   // give back full health
        }
    }

    setVel(0.0f, -4.0f, 0.0f);
    set_rand_position();    // put at a new random position
    was_hit = false;
}

void Blocky::gamereset()
{
    lives = 2;
    hp = starting_hp;
    setVel(0.0f, -4.0f, 0.0f);
    set_rand_position();    // put at a new random position
    was_hit = false;
}

bool Blocky::did_damage()
{
    return was_hit;
}

void Blocky::move()
{
        // move main blocky
    if (is_alive() && explode_done) {
        pos[0] += vel[0];
        pos[1] += vel[1];
        pos[2] += vel[2];
        vel[0] += acc[0];
        vel[1] += acc[1];
        vel[2] += acc[2];
    } else if (!explode_done) { // move sub boxes until they fall off screen
        if (sub_ScreenIn()) {
            for (int i = 0; i < 8; i++) {
                sub_boxes[i].pos[0] += sub_boxes[i].vel[0];
                sub_boxes[i].pos[1] += sub_boxes[i].vel[1];
                sub_boxes[i].pos[2] += sub_boxes[i].vel[2];
                sub_boxes[i].vel[0] += sub_boxes[i].acc[0];
                sub_boxes[i].vel[1] += sub_boxes[i].acc[1];
                sub_boxes[i].vel[2] += sub_boxes[i].acc[2];
            }
        }
    }
}

void Item::hpDamage(Blocky & bf)
{
    if (!bf.did_damage()) {
        hp = hp - bf.damage;
        bf.set_hit();
    }
}

bool Blocky::is_alive()
{
    return (lives > 0);
}

void Blocky::set_hit()
{
    was_hit = true;
}

void Blocky::explode()
{
    int rvel = 8;
    float deg_to_rad = PI/180.0f;
    int pixel_offset = 8;   // sets origin of offset to be 8 left and 8 down
    int xcoord = pos[0] - pixel_offset;
    int ycoord = pos[1] - pixel_offset;
    int rand_offset;    // pixel_offset pixel offset randomly from center of blocky

    for (int i = 0; i < pixel_offset; i++) {
        rand_offset = rand() % (pixel_offset * 2);
        sub_boxes[i].setPos(pos[0]+rand_offset, pos[1]+rand_offset, 0);
        sub_boxes[i].setVel((rvel*cos(deg_to_rad * sb_angles[i])),
                                    (rvel*sin(deg_to_rad * sb_angles[i])), 0);
    }
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
