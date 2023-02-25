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
    // std::string *words;

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

public:


Timer(double s);
// ~Timer();

// setters
void reset();

// getters
double getTime();
bool isDone();

};

