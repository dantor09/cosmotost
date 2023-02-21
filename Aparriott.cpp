/* Ailand Parriott
 * 23.02.17
 * 3350
 * Project Group 1
 * Entity files
 */

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

#include "cosmotost.cpp"

using namespace std;

// ============
// = ENTITY S =
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

void makeEntity(float posX, float posY, float initVelX, float initVelY, 
                float curveX, float curveY) {
	if (g.numEnt < MAX_ENTITIES) {
		entity[g.numEnt].dim[0] = 8;
		entity[g.numEnt].dim[1] = 8;
		entity[g.numEnt].pos[0] = posX;
		entity[g.numEnt].pos[1] = posY;
		entity[g.numEnt].vel[0] = initVelX;
		entity[g.numEnt].vel[1] = initVelY;
		entity[g.numEnt].color[0] = randnum(80, 120);
		entity[g.numEnt].color[1] = randnum(100, 180);
		entity[g.numEnt].color[2] = randnum(200, 255);
        entity[g.numEnt].curve[0] = curveX;
        entity[g.numEnt].curve[1] = curveY;
		g.numEnt++;
	}	
}

void entityPhysics() {
    // makeEntity(int posX, int posY,  int initVelX, int initVelY) 

    // Spawnspeed determines how many ticks until spawning another entity
    if (e.spawnSpeed == 0) {
        e.spawnSpeed = 6;
        if (e.chainLen == 0) {
            e.chainLen = randnum(4, 12);
            e.curveRandX = randnum(-4, 0);
            e.curveRandY = randnum(-4, 4);

            e.enterloc = randnum(0, 3);
            if (e.enterloc == 0) {
                // makeEntity SPAWN FROM TOP, MOVES LEFT DOWNWARD
                e.spawnX = randnum(g.xres / 2, g.xres);
                e.spawnY = g.yres - 5;
                e.spawnVelX = randnum(-8, -4);
                e.spawnVelY = randnum(-8, 0);
            } else if (e.enterloc <= 2) {
                // makeEntity SPAWN FROM RIGHT, MOVES LEFT, RANDUM UP AND DOWN
                e.spawnX = g.xres;
                e.spawnY = randnum(0, g.yres);
                e.spawnVelX = randnum(-8, -4);
                e.spawnVelY = randnum(-8, 8);
            } else if (e.enterloc == 3) {
                // makeEntity SPAWN FROM BOTTOM, MOVES LEFT AND UP
                e.spawnX = randnum(g.xres / 2, g.xres);
                e.spawnY = 5;
                e.spawnVelX = randnum(-8, -4);
                e.spawnVelY = randnum(0, 8);
            }
        }
        makeEntity(e.spawnX, e.spawnY, e.spawnVelX, e.spawnVelY, e.curveRandX, 
                    e.curveRandY);

        e.chainLen--;
    }
    e.spawnSpeed--;

	for (int i = 0; i < g.numEnt; i++) {
        entity[i].pos[0] += entity[i].vel[0]/2;
		entity[i].pos[1] += entity[i].vel[1]/2;

        entity[i].vel[0] += entity[i].curve[0] / 32;
        entity[i].vel[1] += entity[i].curve[1] / 32;

        // DESPAWN
        if (entity[i].pos[1] < -4 || 
                entity[i].pos[1] > g.yres + 4 ||
                entity[i].pos[0] < -4) {	
            entity[i] = entity[--g.numEnt];
        }

        // BOUNCE
		if (entity[i].pos[1] <= 4 ||
				entity[i].pos[1] >= g.yres - 4) {			
			entity[i].vel[1] = -entity[i].vel[1];
        }
    }

}

void entityRender() {
	//Draw entity.
	for (int i = 0; i < g.numEnt; i++) {
		glPushMatrix();
		glColor3ubv(entity[i].color);
		glTranslatef(entity[i].pos[0], entity[i].pos[1], 0.0f);
		glBegin(GL_QUADS);
			glVertex2f(-entity[i].dim[0], -entity[i].dim[1]);
			glVertex2f(-entity[i].dim[0],  entity[i].dim[1]);
			glVertex2f( entity[i].dim[0],  entity[i].dim[1]);
			glVertex2f( entity[i].dim[0], -entity[i].dim[1]);
		glEnd();
		glPopMatrix();
	}
}






