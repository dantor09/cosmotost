#pragma once
#include <GL/glx.h>
#include <iostream>
#include <new>
#include <sstream>

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
    Box *t_boxs;

    // constructors
    Menu(unsigned int n_texts, float _w, float _h, float _x, float _y);
    ~Menu();

    // setters
    void set_bcolor(int r, int g, int b);   // sets boarder color
    void set_color(int r, int g, int b);   // sets mainbox color
    void set_pos(float x, float y, float z);

    // other
    void draw();

    // debug
    std::string get_info();

} ;



