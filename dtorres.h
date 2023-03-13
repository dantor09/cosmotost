#pragma once
#include <iostream>
#include "Box.h"
#include "hzhang.h"
using namespace std;


class InfoBoard : public Box
{
    public: 

    void draw();

};

class FreezeBlock : public Item 
{
    
    public:
    FreezeBlock();
    ~FreezeBlock();
    bool position_set;
    Timer * timer;
    void set_timer(int);
    void draw();


};