#pragma once
#include <sstream>
#include <string>

class Box {

public:

    // vars
    float w, h;
    float pos[3]; 
    unsigned char color[3];
    std::string text;

    // constructors
    Box();
    Box(float _w, float _h, float _x, float _y);

    // setters
    void set_color(int r, int g, int b);
    void set_pos(float x, float y, float z);
    void set_dim(float _w, float _h);
    void set_text(std::string t);

    unsigned char * get_color();

    // debug
    std::string get_info();

} ;



