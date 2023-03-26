#pragma once
#include <iostream>
#include "Box.h"
#include "hzhang.h"
using namespace std;

class InfoBoard : public Box
{
    public:
    InfoBoard();
    void draw();
};

class FreezeBlock : public Item 
{
    public:
    
    bool position_set;
    int min_block_dimension;
    int max_block_dimension;
    Timer * ptimer;

    FreezeBlock();
    ~FreezeBlock();
    int randomDimension();
    void followPlayer(Item & player);
    void setTimer(int);
    void setMinMaxBlockDimensions(int, int);

    void draw();
};