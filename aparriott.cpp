/* Ailand Parriott
 * 23.02.17
 * 3350
 * cosmotost
 * entity.cpp
 */

#include <iostream>
#include <GL/glx.h>

#include "Global.h"
#include "aparriott.h"

using namespace std;

//  ENTITY CONSTRUCTORS
Entity::Entity() {
	dim[0] = 8;
	dim[1] = 8;
	pos[0] = g.xres / 2;
	pos[1] = g.yres / 2;
	vel[0] = -1;
	vel[1] = -1;
    curve[0] = 0;
    curve[1] = 0;
}

Entity::Entity(float wid, float hgt, float posX, float posY, float v0, 
        float v1, float curveX, float curveY) {
	dim[0] = wid;
	dim[1] = hgt;
	pos[0] = posX;
	pos[1] = posY;
	vel[0] = v0;
	vel[1] = v1;
    curve[0] = curveX;
    curve[1] = curveY;
}

// FUNCTIONS
#define rnd() ((float)rand() / (float)RAND_MAX)
int EntitySpawn::randnum(int min, int max) {
    return min + rand() % ((max + 1) - min);
}

void EntitySpawn::makeEntity(float posX, float posY, float initVelX, float initVelY, 
                float curveX, float curveY) {
	if (e.numEnt < MAX_ENTITIES) {
		entity[e.numEnt].dim[0] = 8;
		entity[e.numEnt].dim[1] = 8;
		entity[e.numEnt].pos[0] = posX;
		entity[e.numEnt].pos[1] = posY;
		entity[e.numEnt].vel[0] = initVelX;
		entity[e.numEnt].vel[1] = initVelY;
		entity[e.numEnt].color[0] = e.randnum(80, 120);
		entity[e.numEnt].color[1] = e.randnum(100, 180);
		entity[e.numEnt].color[2] = e.randnum(200, 255);
        entity[e.numEnt].curve[0] = curveX;
        entity[e.numEnt].curve[1] = curveY;
		e.numEnt++;
	}	
}

// void Entity::entityPhysics() {
//     // makeEntity(int posX, int posY,  int initVelX, int initVelY) 
// 
//     // Spawnspeed determines how many ticks until spawning another entity
//     if (e.spawnSpeed == 0) {
//         e.spawnSpeed = 6;
//         if (e.chainLen == 0) {
//             e.chainLen = e.randnum(4, 12);
//             e.curveRandX = e.randnum(-4, 0);
//             e.curveRandY = e.randnum(-4, 4);
// 
//             e.enterloc = e.randnum(0, 3);
//             if (e.enterloc == 0) {
//                 // makeEntity SPAWN FROM TOP, MOVES LEFT DOWNWARD
//                 e.spawnX = e.randnum(g.xres / 2, g.xres);
//                 e.spawnY = g.yres - 5;
//                 e.spawnVelX = e.randnum(-8, -4);
//                 e.spawnVelY = e.randnum(-8, 0);
//             } else if (e.enterloc <= 2) {
//                 // makeEntity SPAWN FROM RIGHT, MOVES LEFT, RANDUM UP AND DOWN
//                 e.spawnX = g.xres;
//                 e.spawnY = e.randnum(0, g.yres);
//                 e.spawnVelX = e.randnum(-8, -4);
//                 e.spawnVelY = e.randnum(-8, 8);
//             } else if (e.enterloc == 3) {
//                 // makeEntity SPAWN FROM BOTTOM, MOVES LEFT AND UP
//                 e.spawnX = e.randnum(g.xres / 2, g.xres);
//                 e.spawnY = 5;
//                 e.spawnVelX = e.randnum(-8, -4);
//                 e.spawnVelY = e.randnum(0, 8);
//             }
//         }
//         e.makeEntity(e.spawnX, e.spawnY, e.spawnVelX, e.spawnVelY, e.curveRandX, 
//                     e.curveRandY);
// 
//         e.chainLen--;
//     }
//     e.spawnSpeed--;
// 
// 	for (int i = 0; i < e.numEnt; i++) {
//         entity[i].pos[0] += entity[i].vel[0]/2;
// 		entity[i].pos[1] += entity[i].vel[1]/2;
// 
//         entity[i].vel[0] += entity[i].curve[0] / 32;
//         entity[i].vel[1] += entity[i].curve[1] / 32;
// 
//         // DESPAWN
//         if (entity[i].pos[1] < -4 || 
//                 entity[i].pos[1] > g.yres + 4 ||
//                 entity[i].pos[0] < -4) {	
//             entity[i] = entity[--e.numEnt];
//         }
// 
//         // BOUNCE
// 		if (entity[i].pos[1] <= 4 ||
// 				entity[i].pos[1] >= g.yres - 4) {			
// 			entity[i].vel[1] = -entity[i].vel[1];
//         }
//     }
// 
// }

// void Entity::entityRender() {
// 	//Draw entity.
// 	glPushMatrix();
// 	glColor3ubv(entity[i].color);
// 	glTranslatef(entity[i].pos[0], entity[i].pos[1], 0.0f);
// 	glBegin(GL_QUADS);
// 		glVertex2f(-entity[i].dim[0], -entity[i].dim[1]);
// 		glVertex2f(-entity[i].dim[0],  entity[i].dim[1]);
// 		glVertex2f( entity[i].dim[0],  entity[i].dim[1]);
// 		glVertex2f( entity[i].dim[0], -entity[i].dim[1]);
// 	glEnd();
// 	glPopMatrix();
// }


//  ENTITYSPAWN CONSTRUCTOR
EntitySpawn::EntitySpawn() {
    chainLen = 0;
    spawnSpeed = 0;
    curveRandX = 0;
    curveRandY = 0;
}




