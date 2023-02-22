/* Ailand Parriott
 * 23.02.21
 * 3350
 * Cosmotost
 * entity.h
 */

#pragma once
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>

const int MAX_ENTITIES = 128;

class Entity {
	public:
	float dim[2];
	float vel[2];
	float pos[2];
    float curve[2];
	unsigned char color[3];
	/*void set_color(unsigned char col[3]) {
		memcpy(color,  col, sizeof(unsigned char) * 3);
	}*/

	Entity() {
		dim[0] = 8;
		dim[1] = 8;
		pos[0] = g.xres / 2;
		pos[1] = g.yres / 2;
		vel[0] = -1;
		vel[1] = -1;
        curve[0] = 0;
        curve[1] = 0;
	}

	Entity(float wid, float hgt, float posX, float posY, float v0, float v1, 
            float curveX, float curveY) {
		dim[0] = wid;
		dim[1] = hgt;
		pos[0] = posX;
		pos[1] = posY;
		vel[0] = v0;
		vel[1] = v1;
        curve[0] = curveX;
        curve[1] = curveY;
	}
} entity[MAX_ENTITIES];

class EntitySpawn {
    public:
    int enterloc, chainLen, spawnSpeed, numEnt;
    float spawnX, spawnY;
    float spawnVelX, spawnVelY;
    float curveRandX, curveRandY;
    EntitySpawn() {
        chainLen = 0;
        spawnSpeed = 0;
        curveRandX = 0;
        curveRandY = 0;
    }
} e;