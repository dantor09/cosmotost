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
    private:
        float getVelocityConsideringArea(float);

    public:
        bool position_set;
        int min_block_dimension;
        int max_block_dimension;
        float max_velocity;
        float minimum_velocity;
        float melting_rate;
        Timer * pFreezeTimer;
        Timer * pFollowTimer;
        GLuint * texture;

        FreezeBlock();
        ~FreezeBlock();
        void draw();
        void melt(float);
        void reduceVelocity(float);
        void setFreezeTimer(int);
        void setFollowTimer(int);
        int randomDimension();
        void followItem(Item & );
        void reduceFreezeArea(float);
        void checkBounce();
        void setMinMaxBlockDimensions(int, int);
};