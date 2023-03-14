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
    
    bool position_set;
    Timer * ptimer;

    FreezeBlock();
    ~FreezeBlock();
    void setTimer(int);
    void draw();
};