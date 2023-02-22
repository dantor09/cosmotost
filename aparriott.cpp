/* Ailand Parriott
 * 23.02.17
 * 3350
 * Project Group 1
 * Entity.cpp
 */

#include "Global.h"
#include "aparriott.h"

using namespace std;

#define rnd() ((float)rand() / (float)RAND_MAX)
int randnum(int min, int max) {
    return min + rand() % ((max + 1) - min);
}

void makeEntity(float posX, float posY, float initVelX, float initVelY, 
                float curveX, float curveY) {
	if (e.numEnt < MAX_ENTITIES) {
		entity[e.numEnt].dim[0] = 8;
		entity[e.numEnt].dim[1] = 8;
		entity[e.numEnt].pos[0] = posX;
		entity[e.numEnt].pos[1] = posY;
		entity[e.numEnt].vel[0] = initVelX;
		entity[e.numEnt].vel[1] = initVelY;
		entity[e.numEnt].color[0] = randnum(80, 120);
		entity[e.numEnt].color[1] = randnum(100, 180);
		entity[e.numEnt].color[2] = randnum(200, 255);
        entity[e.numEnt].curve[0] = curveX;
        entity[e.numEnt].curve[1] = curveY;
		e.numEnt++;
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

	for (int i = 0; i < e.numEnt; i++) {
        entity[i].pos[0] += entity[i].vel[0]/2;
		entity[i].pos[1] += entity[i].vel[1]/2;

        entity[i].vel[0] += entity[i].curve[0] / 32;
        entity[i].vel[1] += entity[i].curve[1] / 32;

        // DESPAWN
        if (entity[i].pos[1] < -4 || 
                entity[i].pos[1] > g.yres + 4 ||
                entity[i].pos[0] < -4) {	
            entity[i] = entity[--e.numEnt];
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
	for (int i = 0; i < e.numEnt; i++) {
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






