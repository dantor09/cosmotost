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
    id = -1;    // no id given initially

}

Box::Box(float _w, float _h, float _x, float _y)
        : w{_w}, h{_h}, pos{_x, _y, 0} {}

// Setters

void Box::setColor(int r, int g, int b)
{
    color[0] = (unsigned char)r;
    color[1] = (unsigned char)g;
    color[2] = (unsigned char)b;
}

void Box::setPos(float x, float y, float z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void Box::setDim(float _w, float _h)
{
    w = _w;
    h = _h;
}

unsigned char * Box::getColor()
{
    return color;
}

// void Box::set_text(std::string t)
// {
//     text = t;
// }

std::string Box::getInfo()
{
    std::ostringstream temp;
    temp << "box.w: " << w << std::endl
        << "box.h: " << h << std::endl
        << "box.pos[x]: " << pos[0] << std::endl
        << "box.pos[y]: " << pos[1] << std::endl;

    return temp.str();
}
