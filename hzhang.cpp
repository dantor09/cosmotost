//Huaiyu Zhang
//Feb 24
#include <iostream>
#include <fstream>
// #include <string>
#include <cmath>
#include "hzhang.h"
#include "Global.h"
#include "mkausch.h"
#include "aparriott.h"
#include "fonts.h"
#include <new>
#include <algorithm>
#include <cstring>

using namespace std;
// x,y,z are pos[]. bullet_type is connect with bullet_type_prime
void makeBullet(float x, float y,float z, int bullet_type) 
{
	stats.UpdateShots(bullet_type);

	switch(bullet_type){
	case 1:
		if (g.n_Bullet < MAX_bullet){
			bul[g.n_Bullet].setBullet(x, y, z, 1);
			++g.n_Bullet;
		}
		break;
	case 2:
		if (g.n_Bullet < MAX_bullet){
			bul[g.n_Bullet].setBullet(x, y+5, z, 1);
			++g.n_Bullet;
		}
		if (g.n_Bullet < MAX_bullet){
			bul[g.n_Bullet].setBullet(x, y-5, z, 1);
			++g.n_Bullet;
		}
		break;
	case 3:
		if (g.n_Bullet < MAX_bullet){
			bul[g.n_Bullet].setBullet(x, y, z, 1);
			++g.n_Bullet;
		}
		if (g.n_Bullet < MAX_bullet){
			bul[g.n_Bullet].setBullet(x, y+5, z, 2);
			++g.n_Bullet;
		}
		if (g.n_Bullet < MAX_bullet){
			bul[g.n_Bullet].setBullet(x, y-5, z, 3);
			++g.n_Bullet;
		}
		break;
	case 4:
		if (g.n_Bullet < MAX_bullet){
			bul[g.n_Bullet].setBullet(x, y-5, z, 1);
			++g.n_Bullet;
		}
		if (g.n_Bullet < MAX_bullet){
			bul[g.n_Bullet].setBullet(x, y+5, z, 1);
			++g.n_Bullet;
		}
		if (g.n_Bullet < MAX_bullet){
			bul[g.n_Bullet].setBullet(x, y+8, z, 2);
			++g.n_Bullet;
		}
		if (g.n_Bullet < MAX_bullet){
			bul[g.n_Bullet].setBullet(x, y-8, z, 3);
			++g.n_Bullet;
		}
		break;
	case 5:
		if (g.n_Bullet < MAX_bullet){
			bul[g.n_Bullet].setBullet(x, y, z, 1);
			++g.n_Bullet;
		}
	}
}

void makeBread(float x, float y,float z, int Bread_t, int type) 
{
  	if (g.n_Bread < MAX_bread){
		bread[g.n_Bread].setBread(x, y, z, Bread_t, type);
		++g.n_Bread;
    }
}

void makeSpear(float x, float y,float z, int cd) 
{
	g.log << "a spear make" << endl;
	if (g.n_Spear < 10) {
		spear[g.n_Spear].setSpear(x,y,z,cd);
		++g.n_Spear;
	}
}

//=============================================================================
Item::Item()
{
	tex = nullptr;	// assign texture to null in the base class
					// this var should be overridden in classes with textures
}
Item::~Item()
{
}

void Item::setHP(float life) 
{
    hp = life;
}
void Item::setCD(int x) 
{
    cd = x;
}
void Item::setDamage(float x) 
{
    damage = x;
}

void Item::setVel(float x, float y, float z) 
{
    //set the velocity of item
    vel[0] = x;
    vel[1] = y;
    vel[2] = z;
}

void Item::setAcc(float x, float y, float z) 
{
    //set the acceleration of item
    acc[0] = x;
    acc[1] = y;
    acc[2] = z;
}

void Item::setVertex() 
{
    //set the acceleration of item
	vertex[0] = -w;
	vertex[1] = h;
	vertex[2] = w;
	vertex[3] = h;
	vertex[4] = w;
	vertex[5] = -h;
	vertex[6] = -w;
	vertex[7] = -h;
}

// trace toster
void Item::setTrace(Item tos) 
{
	float dx = pos[0] - tos.pos[0];
	float dy = pos[1] - tos.pos[1];
	float dcos = dx/sqrt((dx*dx)+(dy*dy));
	float dsin = dy/sqrt((dx*dx)+(dy*dy));
	vertex[0] = -(w*dcos) - (h*dsin);
	vertex[1] = -(w*dsin) + (h*dcos);
	vertex[2] =  (w*dcos) - (h*dsin);
	vertex[3] =  (w*dsin) + (h*dcos);
	vertex[4] =  (w*dcos) + (h*dsin);
	vertex[5] =  (w*dsin) - (h*dcos);
	vertex[6] = -(w*dcos) + (h*dsin);
	vertex[7] = -(w*dsin) - (h*dcos);
	setVel(velocity*dcos,velocity*dsin,0.0);
}

// to check toaster hp, if <=0 then dead
bool Item::hpCheck() 
{
    // if no hp return true
    return (hp < 0.01);
	// stats.UpdateKills(); // declared in aparriott.cpp

}

bool Item::collision(Item a) 
{
    // for (x0,y0,x1,y1)and(x2,y2,x3,y3) squares
    // if collison -->(x0-x3)*(x1-x2)<0
    // same for y
    bool x = (((pos[0]+w)-(a.pos[0]-a.w))*((pos[0]-w)-(a.pos[0]+a.w))) < 0;
  	bool y = (((pos[1]+h)-(a.pos[1]-a.h))*((pos[1]-h)-(a.pos[1]+a.h))) < 0;
  	return x&&y;
}

void Item::hpDamage(float val) 
{
	hp -= val;
}
void Item::hpDamage(Item a) 
{
    // std::g.log << "hp 1 :" << hp << "Damage :" << a.damage<< std::endl;
    hp = hp - a.damage;
    // std::g.log << "hp 2 :" << hp << std::endl;
}

bool Item::screenIn() 
{
    //check if item reach the edge of screen
    bool a = (pos[0] <= g.xres-w+1);
    bool b = (pos[0] >= w-1);
    bool c = (pos[1] <= g.yres-h+1);
	bool d = (pos[1] >= ((3 * g.yres / 40) + h - 1));
	if (g.donut_active) {
		a = (pos[0] <= 0.7 * g.xres - w + 1);
	}
    return a&&b&&c&&d;
}

bool Item::screenOut() 
{
    //check if item out of screen
    bool a = (pos[0] >= g.xres+w);
    bool b = (pos[0] <= -w);
    bool c = (pos[1] >= g.yres+h);
    bool d = (pos[1] <= ((3*g.yres/40)-h));
    return a||b||c||d;
}

void Item::draw()
{
		// draw item
	if (!tex) {
		glPushMatrix();
		glColor3ub(color[0], color[1], color[2]);
		glTranslatef(pos[0], pos[1], pos[2]);
		glBegin(GL_QUADS);
			glVertex2f(vertex[0],vertex[1]);
			glVertex2f(vertex[2],vertex[3]);
			glVertex2f(vertex[4],vertex[5]);
			glVertex2f(vertex[6],vertex[7]);
		glEnd();
		glPopMatrix();

	} else {
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, *tex);
		// glColor3ub(color[0], color[1], color[2]);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
		glTranslatef(pos[0], pos[1], pos[2]);
		glBegin(GL_QUADS);

			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(vertex[0],vertex[1]);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(vertex[2],vertex[3]);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(vertex[4],vertex[5]);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(vertex[6],vertex[7]);


		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();
	}

}

void Item::draw(Item tos)
{
	// if (trace)
	setTrace(tos);
    // draw item
    // glPushMatrix();
  	// glColor3ub(color[0], color[1], color[2]);
  	// glTranslatef(pos[0], pos[1], pos[2]);
  	// glBegin(GL_QUADS);
	// 	glVertex2f(vertex[0],vertex[1]);
	// 	glVertex2f(vertex[2],vertex[3]);
	// 	glVertex2f(vertex[4],vertex[5]);
	// 	glVertex2f(vertex[6],vertex[7]);
  	// glEnd();
  	// glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, *tex);
	// glColor3ub(color[0], color[1], color[2]);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glTranslatef(pos[0], pos[1], pos[2]);
	glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(vertex[0],vertex[1]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(vertex[2],vertex[3]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(vertex[4],vertex[5]);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(vertex[6],vertex[7]);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}

Item & Item::operator = (const Item &a)
{
	this->w = a.w;
	this->h = a.h;
	for (int i = 0; i < 3; i++)
		this->pos[i] = a.pos[i];
  	for (int i = 0; i < 3; i++)
		this->color[i] = a.color[i];
	this->item_type = a.item_type;
	this->trace = a.trace;
	for (int i = 0; i < 3; i++)
		this->vel[i] = a.vel[i];
	for (int i = 0; i < 3; i++)
		this->acc[i] = a.acc[i];
	this->starting_hp = a.starting_hp;
	this->hp = a.hp;
	this->lives = a.lives;
	this->damage = a.damage;
	for (int i = 0; i < 8; i++)
		this->vertex[i] = a.vertex[i];
	this->cd = a.cd;
	this->tex = a.tex;
 	return *this;
}


Toaster::Toaster()
{
	// To set item_type 0
	// the origen bullet type will be 1
	// set player score t0 0
	score = 0;
	// prime is the corrent weapon, minor is a int store another weapon
	// 1~4 are bullet,   5~8 are laser
	bullet_type_prime = 1;
	bullet_type_minor = 5;
	// for database
	item_type = 0;
	// start pos for toaster
	setPos(g.xres/4, g.yres/2, 0.0);
	setColor(188, 226, 232);
	setDim(30,25);
	// set hp
	starting_hp = 80;
	setHP(starting_hp);
	// dont have to have this
	setDamage(100);
	// how many live toaster have
	lives = 3;
	// for live bar
	plive = new Box[lives];
	int offset_x = -165;
	for (int i = 0; i < lives; i++) {
		plive[i].setColor(255, 255, 255);
		plive[i].w = 8;
		plive[i].h = 16;
		// Position to the left of the health bar
		plive[i].setPos(info_board_1.pos[0] + offset_x, 40, 0);
		offset_x += 28;
	}
	// turn dim and pos to Vertex
	setVertex();
	// energy bar and recover speed
	max_energy = 100.0f;
	energy = max_energy;
	energy_recover = 0.1f;
	// for freeze block
	disable_keys = false;
	// laser off 
	laserOn = false;
	tex = &g.toaster_silhouette;
}

Toaster::~Toaster()
{
	delete [] plive;
}
//=========================================================================
// laserCollison is a function test are object and toaster at same area on yres
bool Toaster::laserCollision(Item a)
{
	return (pos[1] > a.pos[1]-a.h && 
					pos[1] < a.pos[1]+a.h && 
						pos[0] < a.pos[0] - a.w - w);
}
bool Toaster::laserCollision(Entity a)
{
	return (pos[1] > a.pos[1]-a.dim[1] && 
					pos[1] < a.pos[1]+a.dim[1] && 
						pos[0] < a.pos[0] - a.dim[0] - w);
}

void Toaster::setDistance(float val)
{
	distance = val;
}

void Toaster::laserDamage(Item& a)
{
	a.hp = a.hp - laser_damage[bullet_type_prime-5];
}

void Toaster::laserDamage(Entity& a)
{
	a.hp = a.hp - laser_damage[bullet_type_prime-5];
}

void Toaster::posReset()
{
	setPos(g.xres/4, g.yres/2, 0.0);
	score = 0;
	setHP(starting_hp);
	bullet_type_prime = 1;
	bullet_type_minor = 5;
	energy = 100.0f;
	freeze = false;
}
void Toaster::moveToster()
{
// how toaster move by keys and shoot bullet
	if (screenIn() && !disable_keys) {
		// if the toaster still in the screen then move
		if (g.keys[XK_w]) {
			vel[1] = 4;
			// pos[1] += 4;
		} else if (g.keys[XK_s]) {
			// pos[1] -= 4;
			vel[1] = -4;
		} else {
			vel[1] = 0;
		}
		//if (g.keys[XK_s]) {

		// }
		if (g.keys[XK_a]) {
			vel[0] = -4;
			// pos[0] -= 4;
		} else if (g.keys[XK_d]) {
			vel[0] = 4;
		} else {
			vel[0] = 0;
		}

		pos[0] += vel[0];
		pos[1] += vel[1];

	} else {
		// to keep toaster in the screen
		if (pos[0] > g.xres-w+1) 
			pos[0] = g.xres-w+1;
		if (g.donut_active && pos[0] > (0.7*g.xres)-w+1) 
			pos[0] = (0.7*g.xres)-w-1;
		if (pos[0] < w-1) 
			pos[0] = w-1;
		if (pos[1] > g.yres-h+1) 
			pos[1] = g.yres-h+1;
		if (pos[1] < (3*g.yres/40) + h - 1) 
			pos[1] = (3*g.yres/40) + h-1;
	}
	if (!freeze) {
		if (g.keys[XK_space] && bullet_type_prime <= 4) {
			//shoot bullet if not in CD
			if (bullet_type_prime < 5 && g.BulletCD==5) {
				makeBullet(pos[0]+w,pos[1],pos[2],bullet_type_prime);
			}
		}
		if (bullet_type_prime > 4 && bullet_type_prime <= 8) {
			if (g.keys[XK_space] && energy > 5) {
				laserOn = true;
				energy -= 0.2;

			} else {
				laserOn = false;
			}
		}
	}
	if (energy < 100) {
		energy += energy_recover;
	}
}

void Toaster::bulletReload() {
	int temp;
	temp = bullet_type_prime;
	bullet_type_prime = bullet_type_minor;
	bullet_type_minor = temp;
	laserOn = false;
}

void Toaster::tdraw() {
	int colorrand = rand()%200 + 55;
	if (laserOn && energy > 5) {
		glPushMatrix();
		glColor3ub(20,0,0);
		glTranslatef(pos[0]+w, pos[1], pos[2]);
		glBegin(GL_QUADS);
			glVertex2f(0,laser_h[bullet_type_prime - 5]);
			glColor3ub(colorrand, 0, 50);
			glVertex2f(0,0);
			glColor3ub(colorrand, 0, 50);
			glVertex2f(distance,0);
			glColor3ub(20,0,0);
			glVertex2f(distance,laser_h[bullet_type_prime - 5]);
		glEnd();
		glBegin(GL_QUADS);
			glColor3ub(colorrand, 0, 50);
			glVertex2f(0,0);
			glColor3ub(20,0,0);
			glVertex2f(0,-laser_h[bullet_type_prime - 5]);
			glColor3ub(20,0,0);
			glVertex2f(distance,-laser_h[bullet_type_prime - 5]);
			glColor3ub(colorrand, 0, 50);
			glVertex2f(distance,0);
		glEnd();
		glPopMatrix();
	}

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, *(tos.tex));
  	// glColor3ub(color[0], color[1], color[2]);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
  	glTranslatef(pos[0], pos[1], pos[2]);
  	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(vertex[0],vertex[1]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(vertex[2],vertex[3]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(vertex[4],vertex[5]);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(vertex[6],vertex[7]);
  	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
  	glPopMatrix();

	//D.T Draw the lives boxes on the information board
	for (int i = 0; i < lives; i++) {
		glColor3ubv(plive[i].color);

		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, g.lives_silhouette);
		// glColor3ub(color[0], color[1], color[2]);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		// glTranslatef(pos[0], pos[1], pos[2]);
		glTranslatef(plive[i].pos[0], plive[i].pos[1], plive[i].pos[2]);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(-plive[i].w, -plive[i].h);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(-plive[i].w,  plive[i].h);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f( plive[i].w,  plive[i].h);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f( plive[i].w, -plive[i].h);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();

	}

}
// string Toaster::PrintScore() {
//     ostringstream temp;
//     temp << "Score : " << score << endl;
//   return temp.str();
// }

Bullet::Bullet() 
{
    item_type = 41;
	tex = &g.elec_bul_silhouette;
}
Bullet::~Bullet() {}

void Bullet::setBullet(float x, float y, float z, int type) 
{
    setPos (x,y,z);
      //Toaster bullet
        switch (type) {
		case 1:
		// bullet type 1
			setVel (10.0,0.0,0.0);
			setDim (10.0,10.0);
			setColor(240,100,100);
			setDamage(1-(tos.lives-3));
			setHP(1);
			item_type = 41;
			break;
		case 2:
			// bullet type 2
			// item type 42
			// up 10degree
			setVel (10.0,1.76,0.0);
			setDim (4.0,4.0);
			setColor(240,100,100);
			setDamage(1-(tos.lives-3));
			setHP(1);
			item_type = 42;
			break;
		case 3:
			// bullet type 3
			// item type 43
			// down 10degree
			setVel (10.0,-1.76,0.0);
			setDim (4.0,4.0);
			setColor(240,100,100);
			setDamage(1-(tos.lives-3));
			setHP(1);
			item_type = 43;
			break;
		case 4:
			g.log << "make 4 bullet" << endl;
			float r;
			float dx;
			float dy;	
			dx = pos[0]-tos.pos[0];
    		dy = pos[1]-tos.pos[1];
			r = (float)sqrt((dx*dx)+(dy*dy));
			setVel (-10.0 * ((dx/r)),-10 *(dy/r),0.0);
			setDim (4.0,4.0);
			setColor(240,100,100);
			setDamage(10);
			setHP(1);
			break;
		
    	}
	setVertex();
}

void Bullet::moveBullet() 
{
    pos[0] += vel[0];
    pos[1] += vel[1];
    pos[2] += vel[2];
    vel[0] += acc[0];
    vel[1] += acc[1];
    vel[2] += acc[2];
}

Bread::Bread() 
{
    item_type = 11;
	
}
Bread::~Bread() {}

void Bread::setBread(float x, float y,float z, int Bread_t, int type) 
{
    setPos (x,y,z);
	float ya = (2*(pos[1]-tos.pos[1])*(vel[0])*(vel[0]))/
						((pos[0]-tos.pos[0])*(pos[0]-tos.pos[0]));
    switch (Bread_t) {
	case 1:
		// trace green box
		setDim(12.0,12.0);
		setVel(-4.0, 0.0, 0.0);
		setAcc (0.0,-ya,0.0);
		setColor(100,240,100);
		setDamage(10);
		setHP(2);
		trace = false;
		item_type = 11;
		bullet_type_prime = 1;
		point = 10;
		tex = &g.bread_silhouette;
		break;
	case 2:
		// gun level up box
		setDim(20.0,25.0);
		setVel(-4.0, 0.0, 0.0);
		setAcc(0.0,-ya,0.0);
		setColor(156,25,226);
		setDamage(0);
		setHP(1);
		trace = false;
		bullet_type_prime = 1;
		item_type = 12;
		point = 0;
		tex = &g.powerup_silhouette;
		break;
	case 3:
		// forky
		setDim(50.0,5.0);
		setVel(-50.0,0.0,0.0);
		setAcc(0.0,0.0,0.0);
		setColor(250, 238, 2);
		setDamage(50);
		setHP(100);
		setCD(100);
		trace = true;
		bullet_type_prime = 1;
		item_type = 13;
		point = 0;
		tex = &g.fork_silhouette;
		break;
	case 4:
		// normal green box
		setDim(12.0,12.0);
		setVel(-4.0, 0.0, 0.0);
		setAcc (0.0, 0.0, 0.0);
		setColor(100,240,100);
		setDamage(10);
		setHP(2);
		trace = false;
		item_type = 14;
		bullet_type_prime = 1;
		point = 10;
		tex = &g.bread_silhouette;
		break;
	case 5:
		// full power potion 
		setDim(20.0,15.0);
		setVel(-4.0, 0.0, 0.0);
		setAcc(0.0,-ya,0.0);
		setColor(214,213,142);
		setDamage(0);
		setHP(1);
		trace = false;
		bullet_type_prime = 1;
		item_type = 15;
		point = 0;
		tex = &g.energy_silhouette;
		break;
	case 6:
		// full health potion 
		setDim(24.0,16.0);
		setVel(-4.0, 0.0, 0.0);
		setAcc(0.0,-ya,0.0);
		setColor(125,249,255);
		setDamage(0);
		setHP(1);
		trace = false;
		bullet_type_prime = 1;
		item_type = 18;
		point = 0;
		tex = &g.health_silhouette;
		break;
	case 7:
		// extra life
		setDim(27.0,20.0);
		setVel(-4.0, 0.0, 0.0);
		setAcc(0.0,-ya,0.0);
		setColor(255,193,204);
		setDamage(0);
		setHP(1);
		trace = false;
		bullet_type_prime = 1;
		item_type = 17;
		point = 0;
		tex = &g.lives_silhouette;
		break;
    }
	setVertex();
}

void Bread::moveBread() 
{
    pos[0] += vel[0];
    pos[1] += vel[1];
    pos[2] += vel[2];
    vel[0] += acc[0];
    vel[1] += acc[1];
    vel[2] += acc[2];
}

Spear::Spear(){tex = &g.fork_silhouette;}
Spear::~Spear(){}

void Spear::setSpear(float x, float y,float z, int cd) 
{
	setPos(x,y,z);
	setDim(50.0,20.0);
	velocity = -35;
	setVel(-50.0,0.0,0.0);
	setAcc(0.0,0.0,0.0);
	setColor(250, 238, 2);
	setDamage(50);
	setHP(100);
	setCD(cd);
	point = 30;
	trace = true;
	item_type = 13;
}

void Spear::moveSpear()
{
	if (!trace) {
		pos[0] += vel[0];
		pos[1] += vel[1];
		pos[2] += vel[2];
		vel[0] += acc[0];
		vel[1] += acc[1];
		vel[2] += acc[2];
	}
}

//=========================================================================

DonutLaser::DonutLaser()
{
}

DonutLaser::~DonutLaser()
{
}

void DonutLaser::setVertex()
{
	if (laser_type == 2 || laser_type == 3) {
	// vertical
		vertex[0] = coor_one[0] - dim;
		vertex[1] = coor_one[1];
		vertex[2] =	coor_two[0] - dim;
		vertex[3] = coor_two[1];
		vertex[4] = coor_two[0] + dim;
		vertex[5] = coor_two[1];
		vertex[6] = coor_one[0] + dim;
		vertex[7] = coor_one[1];
	}
	else if (laser_type == 1 || laser_type == 4) {
	// horizental
		vertex[0] = coor_one[0];
		vertex[1] = coor_one[1] + dim;
		vertex[2] =	coor_two[0];
		vertex[3] = coor_two[1] + dim;
		vertex[4] = coor_two[0];
		vertex[5] = coor_two[1] - dim;
		vertex[6] = coor_one[0];
		vertex[7] = coor_one[1] -dim;
	} 
	else if (laser_type == 5) {
		float s = sin(slop);
		float c = cos(slop);
		vertex[0] = coor_one[0] + dim * s;
		vertex[1] = coor_one[1] -dim * c;
		if (slop > limit_angle[0] && slop < limit_angle[1]) {
			// up
			vertex[2] = coor_two[0] + dim;
			vertex[3] = coor_two[1];
			vertex[4] = coor_two[0] - dim;
			vertex[5] = coor_two[1];
		}
		else if (slop > limit_angle[1] && slop < limit_angle[2]) {
			// left
			vertex[2] = coor_two[0];
			vertex[3] = coor_two[1] + dim;
			vertex[4] = coor_two[0];
			vertex[5] = coor_two[1] - dim;
		}
		else if (slop > limit_angle[2] && slop < limit_angle[3]) {
			// down
			vertex[2] = coor_two[0] - dim;
			vertex[3] = coor_two[1];
			vertex[4] = coor_two[0] + dim;
			vertex[5] = coor_two[1];
		}
		else if (slop < limit_angle[0] || slop > limit_angle[3]){
			// right
			vertex[2] = coor_two[0];
			vertex[3] = coor_two[1] - dim;
			vertex[4] = coor_two[0];
			vertex[5] = coor_two[1] + dim;
		}
		vertex[6] = coor_one[0] - dim * s;
		vertex[7] = coor_one[1] + dim * c;
	}
}
// chargeCD , lagCD, hide, moveble, dim, stay_cd
void DonutLaser::setCD(int charge, int lag, bool h, bool s, int d, int stay)
{
	colli_tos = false;
	charge_on = true;
	lag_on = false;
	cd_charge = charge;
	cd_lag = lag;
	cd_stay = stay;
	if (charge != 0) {
		alpha = 0;
		alpha_inc = 255 / charge;
	} else {
		alpha = 255;
		alpha_inc = 0;
	}
	hide = h;
	moveble = s;
	dim = d;
	setVertex();
}

// horizental or vertical laser,  pos is x0 or y0
void DonutLaser::setDonutLaser(float pos, float speed, char hor_or_ver)
{
	switch (hor_or_ver)
	{
	case 'h':
		laser_type = 1;
		coor_one[0] = 0;
		coor_two[0] = g.xres;
		coor_one[1] = pos;
		coor_two[1] = pos;
		vel_one[0] = 0;
		vel_two[0] = 0;
		vel_one[1] = speed;
		vel_two[1] = speed;
		break;
	
	case 'v':
		laser_type = 2;
		coor_one[0] = pos;
		coor_two[0] = pos;
		coor_one[1] = g.yres_start;
		coor_two[1] = g.yres;
		vel_one[0] = speed;
		vel_two[0] = speed;
		vel_one[1] = 0;
		vel_two[1] = 0;
		break;
	}
}
//=========================================================================
// slop laser, move horizental or vertical
void DonutLaser::setDonutLaser(float pos, float angle, 
											float speed, char hor_or_ver)
{
	target_angle = (3.1415926 * angle)/ 180;
	slop = tan(target_angle);
	switch (hor_or_ver)
	{
	case 'h':
		laser_type = 3;
		coor_one[0] = pos;
		coor_two[0] = pos + ((g.yres - g.yres_start) * tan(target_angle));
		coor_one[1] = g.yres_start;
		coor_two[1] = g.yres;
		vel_one[0] = speed;
		vel_two[0] = speed;
		vel_one[1] = 0;
		vel_two[1] = 0;
		break;
	
	case 'v':
		laser_type = 4;
		coor_one[0] = 0;
		coor_two[0] = g.xres;
		coor_one[1] = pos;
		coor_two[1] = pos + (g.xres * tan(target_angle));
		vel_one[0] = 0;
		vel_two[0] = 0;
		vel_one[1] = speed;
		vel_two[1] = speed;
		break;
	}
}

// laser move by angle
// 						||
// 						90	
// 						||
// ===============180=== =======0========
// 						||
// 						270
// 						||
// 						||
void DonutLaser::setDonutLaser(float xcenter, float ycenter, float r, 
						float anglestart, float angleend, float anglespeed)
{
	g.log << "make 5 laser" << endl;
	laser_type = 5;
	slop = anglestart * 3.1415926 / 180;
	angleacc = anglespeed * 3.1415926 / 180;
	target_angle = angleend * 3.1415926 / 180;
	radius = r;
	center[0] = xcenter;
	center[1] = ycenter;
	coor_one[0] = center[0] + (radius * cos(slop));
	coor_one[1] = center[1] + (radius * sin(slop));
	limit_angle[0] = atan( (g.yres - center[1]) / (g.xres - center[0]));
	limit_angle[1] = atan( (g.yres - center[1]) / 
											(0 - center[0])) + 3.1415926;
	limit_angle[2] = atan( (g.yres_start - center[1]) / 
											(0 - center[0])) + 3.1415926;
	limit_angle[3] = atan( (g.yres_start - center[1]) / 
											(g.xres - center[0])) + 6.2831853;
	if (slop > limit_angle[0] && slop < limit_angle[1]) {
		// up
		coor_two[0] = center[0] + ((g.yres-center[1])/ tan(slop));
		coor_two[1] = g.yres;
	}
	else if (slop > limit_angle[1] && slop < limit_angle[2]) {
		// left
		coor_two[0] = 0;
		coor_two[1] = center[1] + ((-center[0]) * tan(slop));
	}
	else if (slop > limit_angle[2] && slop < limit_angle[3]) {
		// down
		coor_two[0] = center[0] - ((g.yres-center[1])/ tan(slop));
		coor_two[1] = g.yres_start;
	}
	else {
		// right
		coor_two[0] = g.xres;
		coor_two[1] = center[1] - ((-center[0]) * tan(slop));
	}
}

void DonutLaser::moveLaser() {
	// g.log << slop << endl;
	if (charge_on) {
	// charge and chclearange alpha
		if (cd_charge > 0) {
			cd_charge--;
			alpha += alpha_inc;
			// g.log << alpha << endl;
		} else {
			charge_on = false;
			lag_on = true;
		}
	} else if (lag_on) {
	// lag give player time to react 
	// should not have collision test when lag
		if (cd_lag > 0) {
			cd_lag--;
		} else {
			lag_on = false;
		}
	} else {
	// move laser if is moveble 
	// countdown if not moveble
		if (moveble) {
			// can move
			if (laser_type != 5) {
				// not rotate
				coor_one[0] += vel_one[0];
				coor_one[1] += vel_one[1];
				coor_two[0] += vel_two[0];
				coor_two[1] += vel_two[1];
			} else {
				// rotate laser
				slop += angleacc;
				if (slop < 0) {
					slop = 2 * 3.1415926;
				}
				else if (slop > 2 * 3.1415926) {
					slop = 0;
				}
				coor_one[0] = center[0] + (radius * cos(slop));
				coor_one[1] = center[1] + (radius * sin(slop));
				if (slop > limit_angle[0] && slop < limit_angle[1]) {
					// up
					coor_two[0] = center[0] + ((g.yres-center[1])/ tan(slop));
					coor_two[1] = g.yres;
				}
				else if (slop > limit_angle[1] && slop < limit_angle[2]) {
					// left
					coor_two[0] = 0;
					coor_two[1] = center[1] + ((-center[0]) * tan(slop));
				}
				else if (slop > limit_angle[2] && slop < limit_angle[3]) {
					// down
					coor_two[0] = center[0] - 
									((center[1]-g.yres_start)/ tan(slop));
					coor_two[1] = g.yres_start;
				}
				else {
					// right
					coor_two[0] = g.xres;
					coor_two[1] = center[1] - ((-center[0]) * tan(slop));
				}
			}
			setVertex();
		} else {
			if (cd_stay > 0) {
				cd_stay--;
			}
		}
	}
}

bool DonutLaser::collision(Item itm){
	if (!(charge_on||lag_on)) {
		if (laser_type == 1) {
			// horizental line move up or down
			return !((coor_one[1] - dim - itm.h - itm.pos[1] > 0)||
					(itm.pos[1] - coor_one[1] - dim - itm.h > 0));
		}
		else if (laser_type == 2) {
			// horizental line move up or down
			return !((coor_one[0] - dim - itm.w - itm.pos[0] > 0)||
					(itm.pos[0] - coor_one[0] - dim - itm.w > 0));
		}
		else if (laser_type == 3) {
			if (slop > 0) {
				return !((itm.pos[1] - itm.h) > 
						slop * (itm.pos[0] + itm.w - coor_one[0] - dim)
						|| (itm.pos[1] + itm.h) < 
						slop * (itm.pos[0] - itm.h - coor_one[1] + dim));
			} else {
				return !((itm.pos[0] + itm.w) < 
						slop * (itm.pos[1] - itm.h - coor_one[1] - dim)
						|| (itm.pos[0] - itm.w) > 
						slop * (itm.pos[1] + itm.h - coor_one[1] + dim));
			}
		}
		else if (laser_type == 4) {
			//slop and move up down
			if (slop > 0) {
				return !((itm.pos[0] + itm.w) < 
						slop * (itm.pos[1] - itm.h - coor_one[1] - dim)
						|| (itm.pos[0] - itm.w) > 
						slop * (itm.pos[1] + itm.h - coor_one[1] + dim));
			} else {
				return !((itm.pos[0] + itm.w) > 
						slop * (itm.pos[1] - itm.h - coor_one[1] - dim)
						|| (itm.pos[0] - itm.w) < 
						slop * (itm.pos[1] + itm.h - coor_one[1] + dim));
			}
		}
		else if (laser_type == 5) {
			if (slop > 0 && slop < 1.570796327) {
				if ((itm.pos[0] - itm.w) > coor_one[0] && 
						(itm.pos[1] - itm.h) > coor_one[1]) {
					return (((itm.pos[1] - itm.h - coor_one[1]) / 
							(itm.pos[0] + itm.w - coor_one[0]) < tan(slop)) 
						&& ((itm.pos[1] + itm.h - coor_one[1]) / 
							(itm.pos[0] - itm.w - coor_one[0]) > tan(slop)));
				}
				return false;
			}
			else if (slop > 1.570796327 && slop < 3.1415927) {
				if ((itm.pos[0] + itm.w) < coor_one[0] && 
							(itm.pos[1] - itm.h) > coor_one[1]) {
					return (((itm.pos[1] - itm.h - coor_one[1]) / 
							(coor_one[0] - itm.pos[0] + itm.w) < -tan(slop)) 
						&& ((itm.pos[1] + itm.h - coor_one[1]) / 
							(coor_one[0] - itm.pos[0] - itm.w) > -tan(slop)));
				}
				return false;			
			}
			else if (slop > 3.1415926 && slop < 4.71238898) {
				if ((itm.pos[0] + itm.w) < coor_one[0] && 
									(itm.pos[1] + itm.h) < coor_one[1]) {
					return (((coor_one[1] - itm.pos[1] - itm.h) / 
							(coor_one[0] - itm.pos[0] + itm.w) < tan(slop)) 
						&& ((coor_one[1] - itm.pos[1] + itm.h) / 
							(coor_one[0] - itm.pos[0] - itm.w) > tan(slop)));
				}
				return false;	
			}
		} else {
			if ((itm.pos[0] - itm.w) > coor_one[0] && 
									(itm.pos[1] + itm.h) < coor_one[1]) {
				return (((itm.pos[1] - itm.h - coor_one[1]) / 
							(coor_one[0] - itm.pos[0] + itm.w) < -tan(slop)) 
					&& ((itm.pos[1] + itm.h - coor_one[1]) / 
							(coor_one[0] - itm.pos[0] - itm.w) > -tan(slop)));
			}
			return false;			
		}
	} else {
		return false;
	}
	return false;
}

void DonutLaser::hpDamage(Item &a) 
{
	a.hp -= damage;
}

bool DonutLaser::deleteLaser() 
{
	if (moveble) {
		if (laser_type == 1) {
			return (coor_one[1] < g.yres_start || coor_one[1] > g.yres);
		}
		else if (laser_type == 2) {
			return (coor_one[0] < 0 || coor_one[0] > g.xres);
		}
		else if (laser_type == 3) {
			return ((coor_one[0] < 0 && coor_two[0] < 0)
					|| (coor_one[0] > g.xres && coor_two[0] > g.xres));
		}
//=========================================================================
		else if (laser_type == 4) {
			return ((coor_one[1] < g.yres_start && 
											coor_two[1] < g.yres_start)
					|| (coor_one[1] < g.yres && 
												coor_two[1] < g.yres));
		}
		else if (laser_type == 5) {
			if (angleacc > 0) {
				return slop > target_angle;
			} else {
				return slop < target_angle;
			}
		}
		
	} else {
		return cd_stay <= 0;
	}

	return false;
}

void DonutLaser::draw()
{
	if (charge_on) {
		glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub((int)alpha,100,100,(int)alpha);
		glBegin(GL_QUADS);
			glVertex2f(vertex[0],vertex[1]);
			glVertex2f(vertex[2],vertex[3]);
			glVertex2f(vertex[4],vertex[5]);
			glVertex2f(vertex[6],vertex[7]);
		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
		
	} else if (lag_on) {
		if (!hide) {
			glPushMatrix();
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub((int)alpha,0,0,(int)alpha);
			glBegin(GL_QUADS);
				glVertex2f(vertex[0],vertex[1]);
				glVertex2f(vertex[2],vertex[3]);
				glVertex2f(vertex[4],vertex[5]);
				glVertex2f(vertex[6],vertex[7]);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
			
		}
	} else {

		glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glAlphaFunc(GL_GREATER, 0.0f);
		int n = rand()%155;
		glColor4ub(100,0,0,n);
		glBegin(GL_QUADS);
			glVertex2f(vertex[0],vertex[1]);
			glVertex2f(vertex[2],vertex[3]);
		glColor4ub(255,192,203,255);
			glVertex2f(coor_two[0],coor_two[1]);
			glVertex2f(coor_one[0],coor_one[1]);
		glEnd();
		glBegin(GL_QUADS);
			glVertex2f(coor_one[0],coor_one[1]);
			glVertex2f(coor_two[0],coor_two[1]);
		glColor4ub(20,0,0,n);
			glVertex2f(vertex[4],vertex[5]);
			glVertex2f(vertex[6],vertex[7]);
		glEnd();	
		glDisable(GL_BLEND);
		glPopMatrix();
	}
}
//=============================================================================
EffectBox::EffectBox()
{
}

EffectBox::~EffectBox()
{
}

void EffectBox::setXY()
{
	xy_pos[0] = (pos[0] - tpos[0]) > 0;
	xy_pos[1] = (pos[1] - tpos[1]) > 0;
}

void EffectBox::setTpos(float a,float b)
{
	tpos[0] = a;
	tpos[1] = b;
}
void EffectBox::setTcolor(int a, int b, int c)
{
	t_color[0] = a;
	t_color[1] = b;
	t_color[2] = c;
}

void EffectBox::setEffectVel(float angle, float racc) 
{
	a_vel = angle;
	r_acc = racc;
}

void EffectBox::setBools(bool b)
{
	zhixian_or_zhuan = b;
}

void EffectBox::moveEffect()
{
	float rgrav;
	float r;
  	float dx;
  	float dy;
	if (zhixian_or_zhuan) {
		//zhixian
		pos[0] += vel[0];
		pos[1] += vel[1];
		vel[0] += acc[0];
		vel[1] += acc[1];
		dx = tpos[0]-pos[0];
		dy = tpos[1]-pos[1];
		r = (float)sqrt((dx*dx)+(dy*dy));
		setAcc(2500*(dx/(r*r*r)),2500*(dy/(r*r*r)), 0);
		// acc[0] += 0.1*(dx/r);
		// acc[1] += 0.1*(dy/r);
	} else {
        dx = pos[0]-tpos[0];
        dy = pos[1]-tpos[1];
		vel[0] = dy * a_vel;
		vel[1] = -dx * a_vel;
		if (split_cd > 0) {
			split_cd--;
			if (split_cd == 0) {
				r_acc == 0;
			}
		} else {
			if (stay_cd > 0) {
				stay_cd--;
			} else {
				r_acc -= 0.1;
			}
		}
		rgrav = (float)sqrt((dx*dx)+(dy*dy));
		vel[0] += r_acc * (dx/rgrav);
		vel[1] += r_acc * (dy/rgrav);
		pos[0] += vel[0];
		pos[1] += vel[1];
	}
}

bool EffectBox::deleteEffect()
{
	bool x;
	bool y;
	x = xy_pos[0] != (pos[0] - tpos[0]) > 0;
	y = xy_pos[1] != (pos[1] - tpos[1]) > 0;
	return x&&y;
}

bool EffectBox::deleteEffect(float dis)
{
	float rgrav;
  	float dx;
  	float dy;
	dx = pos[0]-tpos[0];
    dy = pos[1]-tpos[1];
	rgrav = (float)sqrt((dx*dx)+(dy*dy));
	return rgrav < dis;
}

//=============================================================================
ChargeBread::ChargeBread()
{
	g.log<<"make a charge bread" <<endl;
	charge_on = true;
	charge_dim[0] = 0;
	charge_dim[1] = 0;
	charge_dim_acc[0] = 0;
	charge_dim_acc[1] = 0;
    charge_need = 1;	
}

ChargeBread::~ChargeBread() 
{
}

void ChargeBread::setDimAcc(int n)
{
	charge_need = n;
	charge_dim_acc[0] = w/charge_need;
	charge_dim_acc[1] = h/charge_need;
}
void ChargeBread::setBulCD(int a,int b,int t)
{
	inner_cd = a;
	bul_count = b;
	b_type = t;
	shoot = true;
}
void ChargeBread::charge() 
{
	float r;
  	float dx;
  	float dy;
	int deltax, deltay;	
	EffectBox temp;
	if (charge_need > 0) {
		for (int i = 0; i < rand()%2+1; i++) {
			g.log << "make partical" << endl;
			deltax = rand()%300-150;
			deltay = rand()%300-150;
			if (deltax >=0) {
				deltax += 350;
			} else {
				deltax -= 350;
			}
			if (deltay >=0) {
				deltay += 350;
			} else {
				deltay -= 350;
			}

			temp.setPos(pos[0]+deltax,pos[1]+deltay,0);
			temp.setTpos(pos[0],pos[1]);
			temp.setBools(1);
			temp.setDim(2,2);
			temp.setColor(255,255,255);
			temp.setVel(0,0,0);
			dx = pos[0]-temp.pos[0];
			dy = pos[1]-temp.pos[1];
			r = (float)sqrt((dx*dx)+(dy*dy));
			temp.setAcc(-0.1*(dx/r),-0.1*(dy/r), 0);
			temp.setVertex();
			effect.push_front(temp);
			charge_need--;
			if (charge_need == 0) {
				break;
			}
		}
	} else {
		if (effect.empty()) {
			charge_on = false;
		}
	}
}
void ChargeBread::moveChargeBread() {
	int c_count = inner_cd;
	Bullet temp;
	DonutLaser ltemp;
	float rad;
  	float dx;
  	float dy;	
	if (charge_on) {
		charge();
	} 
	else if (working) {
		if (c_count == 0) {
			if (las.empty()) {
				if (shoot) {
					switch (b_type)
					{
					case 1:
						temp.setBullet(pos[0]-w-10,pos[1],0,4);
						cbul.push_front(temp);
						shoot = false;
						break;
					case 2:
						temp.setBullet(pos[0]-w-10,pos[1],0,4);
						cbul.push_front(temp);
						temp.setBullet(pos[0]-w-10,pos[1],0,4);
						temp.setPos(pos[0]-w-10,pos[1]+8,0);
						cbul.push_front(temp);
						temp.setBullet(pos[0]-w-10,pos[1],0,4);
						temp.setPos(pos[0]-w-10,pos[1]-8,0);
						cbul.push_front(temp);
						shoot = false;
						break;
					case 3:
						dx = pos[0]-w-10 -tos.pos[0];
						dy = pos[1]-tos.pos[1];
						rad = (atan(dx/dy)*180/3.1415926) + 180; 
						ltemp.setDonutLaser(pos[0]-w-10,pos[1],1,rad,rad+30,0);
						ltemp.setCD(100,20,0,0,4,200);
						las.push_front(ltemp);
						shoot = false;
						break;
					}
				} else {
					c_count = inner_cd;
					bul_count--;
				}
			}

		} else {
			c_count--;
		}
	}
	if (bul_count == 0) {
		working = false;
	} else {
		shoot = true;
	}
	for (auto la = effect.begin(); la != effect.end(); ) {
		la->moveEffect();
		if (la->deleteEffect()) {
			charge_dim[0] += charge_dim_acc[0];
			charge_dim[1] += charge_dim_acc[1];
			if (next(la) != effect.end()) {
				la = effect.erase(la);
				g.log << "erase eff" << endl;
			} else {
				la = effect.erase(la);
				break;
			}
		} else {
			++la;
		}			
	}
}

void ChargeBread::draw() 
{
	if (charge_on) {
		glPushMatrix();
		glColor3ub(color[0], color[1], color[2]);
		glTranslatef(pos[0], pos[1], pos[2]);
		glBegin(GL_QUADS);
			glVertex2f(-charge_dim[0],charge_dim[1]);
			glVertex2f(charge_dim[0],charge_dim[1]);
			glVertex2f(charge_dim[0],-charge_dim[1]);
			glVertex2f(-charge_dim[0],-charge_dim[1]);
		glEnd();
		glPopMatrix();
	} else {
		setVertex();
		glPushMatrix();
		glColor3ub(color[0], color[1], color[2]);
		glTranslatef(pos[0], pos[1], pos[2]);
		glBegin(GL_QUADS);
			glVertex2f(vertex[0],vertex[1]);
			glVertex2f(vertex[2],vertex[3]);
			glVertex2f(vertex[4],vertex[5]);
			glVertex2f(vertex[6],vertex[7]);
		glEnd();
		glPopMatrix();
	}
}

//=============================================================================
Donut::Donut() 
{
	dtex = &g.donut_texture;
	pos[0] = g.xres * 0.9;
	pos[1] = g.yres / 2;
	pos[2] = 0;
	out_radius = 200.0;
	shelled_radius = 210.0;
	hp = 10000;
	cd = 200;
	count_down = cd;
	weapon = false;
	shelled_on = false;
	setCD();
}
Donut::~Donut() {}


void Donut::donutReset() 
{
	dtex = &g.donut_texture;
	pos[0] = g.xres * 0.9;
	pos[1] = g.yres / 2;
	pos[2] = 0;
	out_radius = 200.0;
	inner_radius = 0;
	shelled_radius = 210.0;
	charge_on = true;
	charge_need = 2000;
	hp = 0;
	cd = 200;
	count_down = cd;
	weapon = false;
	shelled_on = false;
	eff.clear();
	dfork.clear();
	donutlasers.clear();
	dbready.clear();

}

void Donut::moveDonut() 
{
	float r;
  	float dx;
  	float dy;	
	EffectBox temp;
	if (charge_on) {
		tos.freeze = true;
		for (auto ef = eff.begin(); ef != eff.end();) {
			ef->moveEffect();
			if (ef->deleteEffect()||ef->deleteEffect(inner_radius)) {
				inner_radius += 0.1;
				hp += 5;
				if (next(ef) != eff.end()) {
					ef = eff.erase(ef);
				} else {
					ef = eff.erase(ef);
					break;
				}
			} else {
				++ef;
			}
		}
		if (charge_need > 0) {
			for (int i = 0; i < rand()%5+1; i++) {
				dx=(((float)rand()) / (float)RAND_MAX);
				dy=(((float)rand()) / (float)RAND_MAX);
				temp.setPos(dx*g.xres,dy*g.yres,0);
				temp.setTpos(pos[0],pos[1]);
				temp.setXY();
				temp.setBools(1);
				temp.setDim(2,2);
				temp.setColor(255,192,203);
				dx = pos[0]-temp.pos[0];
				dy = pos[1]-temp.pos[1];
				r = (float)sqrt((dx*dx)+(dy*dy));
				temp.setVel(2*dx/r,2*dy/r,0);
				temp.setAcc(150*(dx/(r*r*r)),150*(dy/(r*r*r)), 0);
				temp.setVertex();
				eff.push_front(temp);
				g.log << "make partical" << charge_need << " " << temp.acc[0] <<
							temp.acc[1]<<endl;
				charge_need--;
				if (charge_need == 0) {
					break;
				}
		}
	} else {
		if (eff.empty()) {
			charge_on = false;
		}
	}

	} else {
		tos.freeze = false;
		// g.log << weapon << "  " << count_down << endl;
		int arr[] = {1,2,5,8,3,4, 6 ,7, 9, 10,11,12,13,14};
		int val = rand()%4 + bonus;
		val = arr[val];
		if (!weapon) {
			if (count_down == 0) {
				atteckMove(val);
			} else {
				count_down--;
				d_rotate += d_rotate_acc;
			}
		} else {
			atteckMove(val);
		}
	}
}

void Donut::setCD()
{
	int temp = 10000 - (int)hp;
	temp = temp / 1000;
	cd = 200 - temp*20;
}

bool Donut::hpCheck() 
{
	stats.UpdateKills(); // declared in aparriott.cpp
	
	if (hp < 0) {
			return true;
	}
	if (hp > 7500) {
		bonus = 0;
	} else if (hp > 5000) {
		bonus = 4;
	} else if (hp > 2500) {
		bonus = 8;
	} else {
		bonus = 10;
	}
	setCD();
	return false;
}

bool Donut::collision(Item itm) 
{
	float dx = pos[0] - itm.pos[0] - itm.w;
	if (charge_on) {
		return false;
	}
	if (dx > out_radius)
		return false;
	float dy;
	if ((pos[1] - itm.pos[1] - itm.h) > 0) {
		dy = pos[1] - itm.pos[1] - itm.h;
	}
	else if ((pos[1] - itm.pos[1] + itm.h) < 0) {
		dy = itm.pos[1] - itm.h - pos[1];
	}
	else {
		dy = 0;
	}
	float distance = sqrt((dx*dx)+(dy*dy));
	return out_radius > distance;
}

void Donut::hpDemageDonut(Item itm) 
{
	hp -= itm.damage;
}

void Donut::makeChargeBread(int )
{

}

void Donut::draw() 
{
	for (auto dlaser = donutlasers.begin(); 
									dlaser != donutlasers.end(); dlaser++) {
		dlaser->draw();
	}
	for (auto dbread = dbready.begin(); 
									dbread != dbready.end(); dbread++) {
		dbread->draw();
		g.log << "draw charge" << dbread->charge_on << dbread->charge_dim[0] <<  endl;
		for (auto ef = dbread->effect.begin(); 
									ef != dbread->effect.end(); ef++) {
			ef->draw();
			g.log << "draw ef" << ef->pos[0] << ef->pos[1] << endl;
		}
	}
	int n = 40;
	float anglein = 2*3.141592 / n;
	float x0,x1,y0,y1;
	if (charge_on) {
		for (auto ef = eff.begin();
								ef != eff.end(); ef++) {
			ef->draw();
		}
		glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, *dtex);
		// glBindTexture(GL_TEXTURE_2D, *dtex);
		glColor4ub(255,255,255,(int)inner_radius);
		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < n; i++) {
			x0= pos[0] + inner_radius *cos(i*anglein +d_rotate);
			y0= pos[1] + inner_radius *sin(i*anglein +d_rotate);
			x1= 0.5 + 0.5 *cos(i*anglein+3.14);
			y1= 0.5 + 0.5 *sin(i*anglein+3.14);
			glTexCoord2f(x1 ,y1);
			glVertex2f(x0, y0);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);

		for (int i = 0; i < n; i++) {
				x0=pos[0]+inner_radius*cos(i*anglein);
				y0=pos[1]+inner_radius*sin(i*anglein);
				x1=pos[0]+inner_radius*cos((i+1)*anglein);
				y1=pos[1]+inner_radius*sin((i+1)*anglein);
				// glBegin(GL_TRIANGLES);
				// glColor4ub(0,0,0,0);
				// // glTexCoord3f(1,1,0);
				// glVertex3f(pos[0],pos[1],0);
				// glColor4ub(255, 143, 196,255);
				// // glTexCoord3f(tx1,ty1,0);
				// glVertex3f(x0,y0,0);
				// // glTexCoord3f(tx2,ty2,0);
				// glVertex3f(x1,y1,0);
				// glEnd();
				glBegin(GL_QUADS);
				glColor4ub(255, 143, 196,255);
				glVertex3f(x0,y0,0);
				// glTexCoord3f(tx2,ty2,0);
				glVertex3f(x1,y1,0);
				x0=pos[0]+1.5*inner_radius*cos(i*anglein);
				y0=pos[1]+1.5*inner_radius*sin(i*anglein);
				x1=pos[0]+1.5*inner_radius*cos((i+1)*anglein);
				y1=pos[1]+1.5*inner_radius*sin((i+1)*anglein);
				int n = 255-(int)inner_radius;
				glColor4ub(n,n,n,0);
				// glTexCoord3f(tx2,ty2,0);
				glVertex3f(x1,y1,0);
				glVertex3f(x0,y0,0);
				glEnd();

		}
		glDisable(GL_BLEND);

	} else {
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, *dtex);
		glColor4ub(255,255,255,255);
		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < n; i++) {
			x0= pos[0] + out_radius *cos(i*anglein +d_rotate);
			y0= pos[1] + out_radius *sin(i*anglein +d_rotate);
			x1= 0.5 + 0.5 *cos(i*anglein+3.14);
			y1= 0.5 + 0.5 *sin(i*anglein+3.14);
			glTexCoord2f(x1 ,y1);
			glVertex2f(x0, y0);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		if (shelled_on) {
			float anglein = (2*3.1415926)/40;
			float x0,x1,y0,y1;
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			for(int i=0; i<20; i++){
				x0= pos[0] + shelled_radius *cos(i*anglein);
				y0= pos[1] + shelled_radius *sin(i*anglein);
				x1= pos[0] + shelled_radius *cos((i+1)*anglein);
				y1= pos[1] + shelled_radius *sin((i+1)*anglein);
				glBegin(GL_TRIANGLES);
				glColor4ub(255, 255, 255, 20);
					glVertex3f(pos[0],pos[1],0);
				glColor4ub(255, 255, 255, 200);
					glVertex3f(x0,y0,0);
					glVertex3f(x1,y1,0);
				glEnd();
				x0= pos[0] + shelled_radius *cos(-i*anglein);
				y0= pos[1] + shelled_radius *sin(-i*anglein);
				x1= pos[0] + shelled_radius *cos(-(i+1)*anglein);
				y1= pos[1] + shelled_radius *sin(-(i+1)*anglein);
				glBegin(GL_TRIANGLES);
				glColor4ub(255, 255, 255, 20);
					glVertex3f(pos[0],pos[1],0);
				glColor4ub(255, 255, 255, 200);
					glVertex3f(x0,y0,0);
					glVertex3f(x1,y1,0);
				glEnd();
			}
			glDisable(GL_BLEND);
		}
	}
}

 void Donut::atteckMove(int num) 
 {
	if (!weapon) {
		weapon_id = num;
		if (num == 1 || num == 2 || num == 9){
			weapon_outer_count = 10;
			weapon_inner_count = 20;
			donut_count = weapon_inner_count;
		}
		if (num == 3 ) {
			weapon_outer_count = 30;
			weapon_inner_count = 5;
			donut_count = weapon_inner_count;
		} 
		if (num == 4) {
			weapon_outer_count = 15;
			weapon_inner_count = 4;
			donut_count = weapon_inner_count;
		}
		if (num >= 5 && num <= 8 || num == 12) {
			weapon_outer_count = 1;
			weapon_inner_count = 200;
			donut_count = weapon_inner_count;
		}
		if (num == 10) {
			weapon_outer_count = 100;
			weapon_inner_count = 2;
			donut_count = weapon_inner_count;
		}
		if (num == 11) {
			weapon_outer_count = 2;
			weapon_inner_count = 100;
			donut_count = weapon_inner_count;
		}
		if (num == 13) {
			weapon_outer_count = 6;
			weapon_inner_count = 30;
			donut_count = weapon_inner_count;
		}
		if (num == 14) {
			weapon_outer_count = 5;
			weapon_inner_count = 50;
			donut_count = weapon_inner_count;
		}		
		weapon = true;
	} else {
		// g.log << donutlasers.empty() << dfork.empty() << dbready.empty() <<endl;
		if (weapon_outer_count == 0) {
			if (donutlasers.empty()) {
				d_rotate = 0;
				d_rotate_acc = 6.283 / (cd+1);
				weapon = false;
				count_down = cd;
				return;
			}
		}
		else if (weapon_inner_count == 0) {
			float randangle;
			int rand_pos_neg;
			float accangle;
			float dx;
			float dy;
			float ya;
			int gb;
			int one = 1;
			int negone = -1;
			rand_pos_neg = (rand()%2) * 2 - 1; 	
			DonutLaser temp;
			Spear stemp;
			ChargeBread btemp;
			float alp;
			switch (weapon_id) {
			case 1:
				accangle = 3.1415926/8;
				for (int i = 0; i < 8; i++) {
					dx = out_radius * cos(1.57 + (i * accangle));
					dy = out_radius * sin(1.57 + (i * accangle));
					gb = g.n_donut_bullet;
					do_bul[gb].setPos(pos[0]+dx,pos[1]+dy,0);
					do_bul[gb].setDim(4.0,4.0);
					do_bul[gb].setVel(-5.0, 0.0, 0.0);
					do_bul[gb].setAcc (0.0, 0.0, 0.0);
					do_bul[gb].setColor(255,192,203);
					do_bul[gb].setDamage(10);
					do_bul[gb].trace = false;
					do_bul[gb].item_type = 16;
					do_bul[gb].setVertex();
					g.n_donut_bullet++;
				}
				break;
			case 2:
				accangle = 3.1415926/20;
				for (int i = 0; i < 20; i++) {
					dx = cos(1.57 + (i * accangle));
					dy = sin(1.57 + (i * accangle));
					gb = g.n_donut_bullet;
					do_bul[gb].setPos(pos[0]+(out_radius * dx),
												pos[1]+(out_radius * dy),0);
					do_bul[gb].setDim(4.0,4.0);
					do_bul[gb].setVel(5.0 * dx, 5 * dy, 0.0);
					do_bul[gb].setAcc (0.0, 0.0, 0.0);
					do_bul[gb].setColor(255,192,203);
					do_bul[gb].setDamage(10);
					do_bul[gb].trace = false;
					do_bul[gb].item_type = 16;
					do_bul[gb].setVertex();
					g.n_donut_bullet++;
				}
				break;	
			case 3:
				randangle =(((float)rand()) / (float)RAND_MAX);
				for (int i = 0; i < 2; i++) {
					dx = out_radius * cos(one * (1.57 + 0.5*randangle));
					dy = out_radius * sin(one * (1.57 + 0.5*randangle));
					gb = g.n_donut_bullet;
					do_bul[gb].setPos(pos[0]+dx,pos[1]+dy,0);
					do_bul[gb].setDim(4.0,4.0);
					do_bul[gb].setVel(-5.0 , 0.0, 0.0);
					ya = (2*(do_bul[gb].pos[1]-tos.pos[1])*
									(do_bul[gb].vel[0])*(do_bul[gb].vel[0]))/
									((do_bul[gb].pos[0]-tos.pos[0])*
									(do_bul[gb].pos[0]-tos.pos[0]));
					do_bul[gb].setAcc (0.0, -ya, 0.0);
					do_bul[gb].setColor(255,192,203);
					do_bul[gb].setDamage(10);
					do_bul[gb].trace = false;
					do_bul[gb].item_type = 16;
					do_bul[gb].setVertex();
					g.n_donut_bullet++;
					one *= negone;
				}
				break;
			case 4:
				accangle = 3.1415926/20;
				for (int i = 1; i < 3; i++) {
					dx = out_radius * cos(3.14 - i*accangle);
					dy = out_radius * sin(3.14 - i*accangle);
					gb = g.n_donut_bullet;
					do_bul[gb].setPos(pos[0]+dx,pos[1]+dy,0);
					do_bul[gb].setDim(4.0,4.0);
					do_bul[gb].setVel(-5.0 - i, 0.0, 0.0);
					ya = (2*(do_bul[gb].pos[1]-tos.pos[1]) *
									(do_bul[gb].vel[0])*(do_bul[gb].vel[0])) /
								((do_bul[gb].pos[0]-tos.pos[0]) *
									(do_bul[gb].pos[0]-tos.pos[0]));
					do_bul[gb].setAcc (0.0, -ya, 0.0);
					do_bul[gb].setColor(255,192,203);
					do_bul[gb].setDamage(10);
					do_bul[gb].trace = false;
					do_bul[gb].item_type = 16;
					do_bul[gb].setVertex();
					g.n_donut_bullet++;
					dx = out_radius * cos(3.14 + i*accangle);
					dy = out_radius * sin(3.14 + i*accangle);
					gb = g.n_donut_bullet;
					do_bul[gb].setPos(pos[0]+dx,pos[1]+dy,0);
					do_bul[gb].setDim(4.0,4.0);
					do_bul[gb].setVel(-5.0 - i, 0.0, 0.0);
					ya = (2*(do_bul[gb].pos[1]-tos.pos[1]) *
									(do_bul[gb].vel[0])*(do_bul[gb].vel[0])) /
								((do_bul[gb].pos[0]-tos.pos[0]) *
									(do_bul[gb].pos[0]-tos.pos[0]));
					do_bul[gb].setAcc (0.0, -ya, 0.0);
					do_bul[gb].setColor(255,192,203);
					do_bul[gb].setDamage(10);
					do_bul[gb].trace = false;
					do_bul[gb].item_type = 16;
					do_bul[gb].setVertex();
					g.n_donut_bullet++;
				}
				break;
			case 5:
				temp.setDonutLaser(donut.pos[0],donut.pos[1], 
										donut.out_radius,90.01,269.9,1);
				temp.setCD(100,50,1,1,4,0);
				donutlasers.push_front(temp);
				break;

			case 6:
				g.log << "in case 6"<<endl;
				temp.setDonutLaser(g.yres_start+100,6,'h');
				temp.setCD(100,50,0,1,4,0);
				donutlasers.push_front(temp);
				temp.setDonutLaser(g.xres,-8,'v');
				temp.setCD(100,50,0,1,4,0);
				donutlasers.push_front(temp);
				break;

			case 7:
				g.log << "in case 7" <<endl;
				temp.setDonutLaser(0.5*g.xres,g.yres_start, 
														1,90.01,180.1,0.5);
				temp.setCD(100,50,1,1,4,0);
				donutlasers.push_front(temp);
				temp.setDonutLaser(0.5*g.xres,g.yres, 1,269.99,180.1,-0.5);
				temp.setCD(100,50,1,1,4,0);
				donutlasers.push_front(temp);
				break;
			case 8:
				g.log << "in case 8" <<endl;
				alp=(((float)rand()) / (float)RAND_MAX);
				alp = g.yres - (alp *(g.yres-g.yres_start) * 0.33);
				temp.setDonutLaser(alp, 0,'h');
				temp.setCD(50 ,50 + rand()%50,0,0,5+rand()%20,100);
				donutlasers.push_front(temp);
				alp=(((float)rand()) / (float)RAND_MAX);
				alp = g.yres - ((alp+1) *(g.yres-g.yres_start) * 0.33);
				temp.setDonutLaser(alp, 0,'h');
				temp.setCD(50,50+rand()%100,0,0,5+rand()%20,100);
				donutlasers.push_front(temp);
				alp=(((float)rand()) / (float)RAND_MAX);
				alp = g.yres_start + (alp *(g.yres-g.yres_start) * 0.33);
				temp.setDonutLaser(alp, 0,'h');
				temp.setCD(50 + rand()%50,50 + rand()%50,0,0,5+rand()%20,100);
				donutlasers.push_front(temp);
				break;
			case 9:
				accangle = 3.1415926/8;
				for (int i = 0; i < 9; i++) {
					dx = out_radius * cos(1.57 + (i * accangle));
					dy = out_radius * sin(1.57 + (i * accangle));
					gb = g.n_donut_bullet;
					do_bul[gb].setPos(pos[0]+dx,pos[1]+dy,0);
					do_bul[gb].setDim(4.0,4.0);
					do_bul[gb].setVel(-5.0, 0.0, 0.0);
					do_bul[gb].setAcc (0.0, 0.0, 0.0);
					do_bul[gb].setColor(255,240,255);
					do_bul[gb].setDamage(10);
					do_bul[gb].trace = false;
					do_bul[gb].item_type = 16;
					do_bul[gb].setVertex();
					g.n_donut_bullet++;
				}
				break;
			case 10:
				randangle = 3.1415/25;
				if (weapon_outer_count > 75) {
					ya = 1.57 + (weapon_outer_count - 76) * randangle;
				} else if (weapon_outer_count > 50) {
					ya = 1.57 + (negone*(weapon_outer_count - 75)) * randangle;
				} else if (weapon_outer_count > 25) {
					ya = 1.57 + (weapon_outer_count - 26) * randangle;
				} else {
					ya = 1.57 + (negone*(weapon_outer_count - 26)) * randangle;
				}
				dx = out_radius * cos(ya);
				dy = out_radius * sin(ya);
				gb = g.n_donut_bullet;
				do_bul[gb].setPos(pos[0]+dx,pos[1]+dy,0);
				do_bul[gb].setDim(4.0,4.0);
				do_bul[gb].velocity = -10;
				do_bul[gb].setTrace(tos);
				do_bul[gb].setAcc (0.0, 0.0, 0.0);
				do_bul[gb].setColor(255,240,255);
				do_bul[gb].setDamage(10);
				do_bul[gb].trace = false;
				do_bul[gb].item_type = 16;
				g.n_donut_bullet++;
				break;
			case 11:
				g.log << "in case 11"<<endl;
				if (rand()%2) {
					temp.setDonutLaser(g.yres-10,-6,'h');
					temp.setCD(50,50,0,1,4,0);
				donutlasers.push_front(temp);
				} else {
					temp.setDonutLaser(g.yres_start+10,6,'h');
					temp.setCD(50,50,0,1,4,0);
					donutlasers.push_front(temp);
				}
				temp.setDonutLaser(g.xres,-8,'v');
				temp.setCD(50,50,0,1,4,0);
				donutlasers.push_front(temp);
				break;
			case 12:
				g.log << "in case 12" <<endl;
				alp=(((float)rand()) / (float)RAND_MAX);
				alp = g.yres - (alp *(g.yres-g.yres_start) * 0.33);
				temp.setDonutLaser(alp, 0,'h');
				temp.setCD(30 ,50 + rand()%50,1,0,5+rand()%20,100);
				donutlasers.push_front(temp);
				alp=(((float)rand()) / (float)RAND_MAX);
				alp = g.yres - ((alp+1) *(g.yres-g.yres_start) * 0.33);
				temp.setDonutLaser(alp, 0,'h');
				temp.setCD(30,50+rand()%100,1,0,5+rand()%20,100);
				donutlasers.push_front(temp);
				alp=(((float)rand()) / (float)RAND_MAX);
				alp = g.yres_start + (alp *(g.yres-g.yres_start) * 0.33);
				temp.setDonutLaser(alp, 0,'h');
				temp.setCD(30,50 + rand()%50,1,0,5+rand()%20,100);
				donutlasers.push_front(temp);
				alp = (((float)rand()) / (float)RAND_MAX);
				alp = 0.4*g.xres + (0.6 *alp *g.xres);
 				makeSpear(alp,g.yres-10,0, 100);
				makeSpear(alp,g.yres_start+10,0, 100);
				break;	
			case 13:
				g.log << "in case 13" <<endl;
				if (weapon_outer_count == 6) {
					temp.setDonutLaser(g.xres,-8,'v');
					temp.setCD(50,50,0,1,4,0);
					donutlasers.push_front(temp);
				}
				alp=(((float)rand()) / (float)RAND_MAX);
				alp = alp *(g.yres-g.yres_start);
				temp.setDonutLaser(alp, 0,'h');
				temp.setCD(20 ,50 + rand()%50,1,0,10,100);
				donutlasers.push_front(temp);
				break;		
			case 14:
				g.log << "in case 14" <<endl;
				if (rand()%2) {
				temp.setDonutLaser(donut.pos[0],donut.pos[1], 
										donut.out_radius,90.01,269.9,1.5);
				temp.setCD(20,100,1,1,4,0);
				donutlasers.push_front(temp);
				} else {
				temp.setDonutLaser(donut.pos[0],donut.pos[1], 
										donut.out_radius,269.9,90.01,-1.5);
				temp.setCD(20,100,1,1,4,0);
				donutlasers.push_front(temp);
				}
				break;														
			case 15:
				g.log << "in case 8" <<endl;
				
				break;														

			}
			weapon_outer_count--;
			weapon_inner_count = donut_count;
		} else {
			weapon_inner_count--;
		}
	}
}
//=============================================================================













//===========================================================================//
//math functions//
bool pointIn(float x0, float y0, float * arr, int n) 
{
	float point[n/2];
	for (int i = 0; i < (n/2); i++) {
		point[i] = crossX(x0,y0,(arr[2*i]),(arr[2*i+1]),
											(arr[(2*i+2)%n]),(arr[(2*i+3)%n]));
	}
	float large=0;
	float small=0;
	for (int i = 0; i < (n/2); i++) {
		if (large < point[i]) {
			small = large;
			large = point[i];
		}
		if (large > point[i] && small < point[i])
			small = point[i];
	}
	g.log << large << " " << small <<endl;
	if (x0 > large || x0 < small)
		return false;
	else
		return true;
}

float crossX(float x0,float y0,float xa,float ya,float xb,float yb) 
{
	float result = (ya-((ya-yb)*xa/(xa-xb)))/((y0/x0)-((ya-yb)/(xa-xb)));
	if (result < fmin(xa,xb)|| result > fmax(xa,xb))
		return 0;
	else
		return result;
}

float maxRadius(float *arr,int n) 
{
	float result = 0.0f;
	for (int i = 0; i < (n/2); i++) {
		if (result < ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1])))
			result = ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1]));
	}
	return result;
}

float minRadius(float *arr,int n) 
{
	float result = ((arr[0]*arr[0])+(arr[1]*arr[1]));
	for (int i = 0; i < (n/2); i++) {
		if (result > ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1])))
			result = ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1]));
	}
	return result;
}

float maxTan(float *arr,int n) 
{
	float result = 0.0f;
	for (int i = 0; i < (n/2); i++) {
		if (result < (arr[2 * i] / arr[2 * i + 1]))
			result = (arr[2 * i] / arr[2 * i + 1]);
	}
	return result;
}
float minTan(float *arr,int n) 
{
	float result = (arr[0] / arr[1]);
	for (int i = 0; i < (n/2); i++) {
		if (result > (arr[2 * i] / arr[2 * i + 1]))
			result = (arr[2 * i] / arr[2 * i + 1]);
	}
	return result;
}

float getAngle(float x0, float y0, float x1, float y1)
{
	return atan((x1 - x0)/(y1 - y0));
}

// bool Item::collision(Item a) {
// 	float arr[8];
// 	float aArr[8];
// 	for(int i = 0; i < 4; i++)
// 	{
// 		arr[i*2] = pos[0] + vertex[i*2];
// 		arr[i*2+1] = pos[1] + vertex[i*2];
// 		aArr[i*2] = a.pos[0] + a.vertex[i*2];
// 		aArr[i*2+1] = a.pos[0] + a.vertex[i*2+1];
// 	}
// 	float R0 = maxRadius(arr,8);
// 	float r0 = minRadius(arr,8);
// 	float R1 = maxRadius(aArr,8);
// 	float r1 = minRadius(aArr,8);
// 	float T0 = maxTan(arr,8);
// 	float t0 = minTan(arr,8);
// 	float T1 = maxTan(aArr,8);
// 	float t1 = minTan(aArr,8);
// 	if (R0 < r1 || R1 < r0 || T0 < t1 || T1 < t0)
// 		return false;
// 	else {
// 		bool delta = false;
// 		for(int i = 0; i < 4; i++) {
// 			if (!delta)
// 				delta = pointIn(pos[0]+vertex[i*2],
										// pos[1]+vertex[i*2+1],aArr,8);
// 		}
// 		for(int i = 0; i < 4; i++) {
// 			if (!delta)
// 				delta = pointIn(a.pos[0]+a.vertex[i*2],
										// a.pos[1]+a.vertex[i*2+1],arr,8);
// 		}
// 		return delta;
// 	}
// }

///====================================Draw shape Functions=======================
//=============================================================================