/* Ailand Parriott
 * 23.02.21
 * 3350
 * Cosmotost
 * aparriott.h
 */

#pragma once

// libraries
#include <iostream>

// local
#include "hzhang.h"

using namespace std;

const int MAX_ENTITIES = 128;

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// ENTITY

class Entity {
	public:
	float dim[2];
	float vel[2];
	float pos[2];
    float curve[2];
	unsigned char color[3];
	float hp;	// how much hp he starts out with
	float damage;	// how much damage he does if collides with toaster
	int point;
	GLuint * tex;
	float tex_color[3];

	/*void set_color(unsigned char col[3]) {
		memcpy(color,  col, sizeof(unsigned char) * 3);
	}*/

	// CONSTRUCTORS
	Entity();
	Entity(float wid, float hgt, float pos_x, float pos_y, float v_x, 
            float v_y, float curve_x, float curve_y) ;

	// SETTERS
	void entityPhysics();
	void entityRender();
	bool collision(Item & a);	// defined in mkausch.cpp
	void setHP(float life);	// defined in mkausch.cpp
	void setDamage(float x);	// defined in mkausch.cpp
	void hpDamage(Item & a);	// defined in mkausch.cpp
	bool hpCheck();			// defined in mkausch.cpp
	Entity & operator = (const Entity &e);
};


class EntitySpawn {
	public:
	int enter_loc, chain_len, spawn_speed, num_ent;
	float spawn_x, spawn_y;
	float spawn_vel_x, spawn_vel_y;
	float curve_rand_x, curve_rand_y;
	EntitySpawn();
	int randNum(int min, int max);
	void makeEntity(float pos_x, float pos_y, float init_vel_x, 
			float init_vel_y, float curve_x, float curve_y);
	void ResetEntity();

};
// ENTITY
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// STATISTICS
class Statistics {
	public:
	float accuracy;
	float timeSurvived;
	int blockyCollateral;
	int beamKills;
	int bombKills;
	int bombsThrown;
	int damageTaken; 
	int kills;
	int dashes;
	int powerUpsGained;
	int shots;
	int shotsHit;
	int shotsMissed;

	Statistics();

	void DisplayStats();
	void UpdateKills();
	void UpdateShots(int numBullets);

};
// STATISTICS
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
// TEXTURES
//class TextureLoad {
//	public:
//	int a;
//
//	TextureLoad();
//};
// TEXTURES
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 


inline Entity entity[MAX_ENTITIES];
inline EntitySpawn e;

inline Statistics stats;

// inline TextureLoad texture;