#pragma once
#include <sstream>
#include <string>

class Box {

public:

    // vars
    float w, h;
    float pos[3];
    unsigned char color[3];
    // std::string text;
    int id;

    // constructors
    Box();
    Box(float _w, float _h, float _x, float _y);

    // setters
    void setColor(int r, int g, int b);
    void setPos(float x, float y, float z);
    void setDim(float _w, float _h);
    // void set_text(std::string t);

    unsigned char * getColor();

    // debug
    std::string getInfo();

} ;
