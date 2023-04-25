/* Ailand Parriott
 * 23.02.17
 * 3350
 * cosmotost
 * entity.cpp
 */

#include <iostream>
#include <GL/glx.h>
#include <curl/curl.h>

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
	setHP(2);
	setDamage(10);
	point = 10;
}

Entity::Entity(float wid, float hgt, float pos_x, float pos_y, float v_x, 
        float v_y, float curve_x, float curve_y) {
	dim[0] = wid;
	dim[1] = hgt;
	pos[0] = pos_x;
	pos[1] = pos_y;
	vel[0] = v_x;
	vel[1] = v_y;
    curve[0] = curve_x;
    curve[1] = curve_y;
	setHP(2);
	setDamage(10);
	point = 10;
}

// FUNCTIONS
#define rnd() ((float)rand() / (float)RAND_MAX)
int EntitySpawn::randNum(int min, int max) {
    return min + rand() % ((max + 1) - min);
}

void EntitySpawn::makeEntity(float pos_x, float pos_y, float init_vel_x, 
        float init_vel_y, float curve_x, float curve_y) {
	if (e.num_ent < MAX_ENTITIES) {
		entity[e.num_ent].dim[0] = 8;
		entity[e.num_ent].dim[1] = 8;
		entity[e.num_ent].pos[0] = pos_x;
		entity[e.num_ent].pos[1] = pos_y;
		entity[e.num_ent].vel[0] = init_vel_x;
		entity[e.num_ent].vel[1] = init_vel_y;
		entity[e.num_ent].color[0] = e.randNum(80, 120);
		entity[e.num_ent].color[1] = e.randNum(100, 180);
		entity[e.num_ent].color[2] = e.randNum(200, 255);
        entity[e.num_ent].curve[0] = curve_x;
        entity[e.num_ent].curve[1] = curve_y;
		entity[e.num_ent].setHP(2);
		e.num_ent++;
	}	
}

void EntitySpawn::ResetEntity() {
    while (e.num_ent != 0) {
        entity[0] = entity[--e.num_ent];
    }
}

void Entity::entityPhysics() {
    // makeEntity(int pos_x, int pos_y,  int init_vel_x, int init_vel_y) 

    // Spawn_speed determines how many ticks until spawning another entity
    if (e.spawn_speed == 0) {
        e.spawn_speed = 6;
        if (e.chain_len == 0) {
            e.chain_len = e.randNum(4, 12);
            e.curve_rand_x = e.randNum(-4, 0);
            e.curve_rand_y = e.randNum(-4, 4);

            e.enter_loc = e.randNum(0, 3);
            if (e.enter_loc == 0) {
                // makeEntity SPAWN FROM TOP, MOVES LEFT DOWNWARD
                e.spawn_x = e.randNum(g.xres / 2, g.xres);
                e.spawn_y = g.yres - 5;
                e.spawn_vel_x = e.randNum(-8, -4);
                e.spawn_vel_y = e.randNum(-8, 0);
            } else if (e.enter_loc <= 2) {
                // makeEntity SPAWN FROM RIGHT, MOVES LEFT, RANDUM UP AND DOWN
                e.spawn_x = g.xres;
                e.spawn_y = e.randNum(0, g.yres);
                e.spawn_vel_x = e.randNum(-8, -4);
                e.spawn_vel_y = e.randNum(-8, 8);
            } else if (e.enter_loc == 3) {
                // makeEntity SPAWN FROM BOTTOM, MOVES LEFT AND UP
                e.spawn_x = e.randNum(g.xres / 2, g.xres);
                e.spawn_y = 5;
                e.spawn_vel_x = e.randNum(-8, -4);
                e.spawn_vel_y = e.randNum(0, 8);
            }
        }
        e.makeEntity(e.spawn_x, e.spawn_y, e.spawn_vel_x, e.spawn_vel_y, 
                e.curve_rand_x, e.curve_rand_y);

        e.chain_len--;
    }
    e.spawn_speed--;

	 for (int i = 0; i < e.num_ent; i++) {
        entity[i].pos[0] += entity[i].vel[0]/2;
		entity[i].pos[1] += entity[i].vel[1]/2;

        entity[i].vel[0] += entity[i].curve[0] / 32;
        entity[i].vel[1] += entity[i].curve[1] / 32;

        // DESPAWN
        if (entity[i].pos[1] < -4 || entity[i].pos[1] > g.yres + 4 ||
                entity[i].pos[0] < -4) {	
            entity[i] = entity[--e.num_ent];
        }

        // BOUNCE
		if (entity[i].pos[1] <= 4 || entity[i].pos[1] >= g.yres - 4) {			
			entity[i].vel[1] = -entity[i].vel[1];
        }
    }
 
 }

//  ENTITYSPAWN CONSTRUCTOR
EntitySpawn::EntitySpawn() {
    chain_len = 0;
    spawn_speed = 0;
    curve_rand_x = 0;
    curve_rand_y = 0;
}
