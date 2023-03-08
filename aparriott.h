/* Ailand Parriott
 * 23.02.21
 * 3350
 * Cosmotost
 * hostile.h
 */

#pragma once
#include <iostream>
#include "hzhang.h"  
//#include <stdio.h>
//#include <unistd.h>
//#include <cstdlib>
//#include <ctime>
//#include <cstring>
//#include <cmath>
#include <GL/glx.h>

using namespace std;

const int MAX_ENTITIES = 128;

class Entity {
	public:
	float dim[2];
	float vel[2];
	float pos[2];
    float curve[2];
	unsigned char color[3];
	int HP;	// how much hp he starts out with
	int damage;	// how much damage he does if collides with toaster
	int point;
	/*void set_color(unsigned char col[3]) {
		memcpy(color,  col, sizeof(unsigned char) * 3);
	}*/

	// CONSTRUCTORS
	Entity();
	Entity(float wid, float hgt, float posX, float posY, float v0, float v1, 
            float curveX, float curveY);

	// SETTERS
	void entityPhysics();
	void entityRender();
	bool collision(Item & a);	// defined in mkausch.cpp
	void set_HP(int life);	// defined in mkausch.cpp
	void set_damage(int x);	// defined in mkausch.cpp
	void HPdamage(Item & a);	// defined in mkausch.cpp
	bool HP_check();			// defined in mkausch.cpp

};

class EntitySpawn {
    public:
    int enterloc, chainLen, spawnSpeed, numEnt;
    float spawnX, spawnY;
    float spawnVelX, spawnVelY;
    float curveRandX, curveRandY;

    EntitySpawn();

	int randnum(int min, int max);
	void makeEntity(float posX, float posY, float initVelX, float initVelY, 
            float curveX, float curveY);
};

inline Entity entity[MAX_ENTITIES];
inline EntitySpawn e;
