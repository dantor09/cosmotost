#include <sstream>
#include <iostream>
#include <iomanip>

#include "Box.h"
#include "Global.h"


// Constructors

Box::Box()
{
    w = 80.0f;
    h = 20.0f;
    pos[0] = g.xres/2.0f;
    pos[1] = g.yres/2.0f;

}

Box::Box(float _w, float _h, float _x, float _y)
        : w{_w}, h{_h}, pos{_x, _y, 0} {}

// Setters

void Box::set_color(int r, int g, int b) 
{
    color[0] = (char)r; 
    color[1] = (char)g; 
    color[2] = (char)b;
}

void Box::set_pos(float x, float y, float z) 
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void Box::set_dim(float _w, float _h)
{
    w = _w;
    h = _h;
}

std::string Box::get_info()
{
    std::ostringstream temp;
    temp << "box.w: " << w << std::endl
        << "box.h: " << h << std::endl
        << "box.pos[x]: " << pos[0] << std::endl
        << "box.pos[y]: " << pos[1] << std::endl;

    return temp.str();
}