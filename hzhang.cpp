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

void makeSpear(float x, float y,float z, int type) 
{
	cerr << "a spear make" << endl;
	if(g.n_Spear < 10) {
		spear[g.n_Spear].setSpear(x,y,z,1);
		++g.n_Spear;
	}
}

//==================================================================================
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
	setVel(-50*dcos,-50*dsin,0.0);
}

// to check toaster hp, if <=0 then dead
bool Item::hpCheck() 
{
    // if no hp return true
    return (hp < 0.01);
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
    // std::cout << "hp 1 :" << hp << "Damage :" << a.damage<< std::endl;
    hp = hp - a.damage;
    // std::cout << "hp 2 :" << hp << std::endl;
}

bool Item::screenIn() 
{
    //check if item reach the edge of screen
    bool a = (pos[0] <= g.xres-w+1);
    bool b = (pos[0] >= w-1);
    bool c = (pos[1] <= g.yres-h+1);
	bool d = (pos[1] >= ((3 * g.yres / 40) + h - 1));
	if(g.donut_active) {
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
void Item::draw(Item tos)
{
	// if(trace)
	setTrace(tos);
    // draw item
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
	int offset_x = -125;
	for(int i = 0; i < lives; i++) {
		plive[i].setColor(255, 255, 255);
		plive[i].w = 5;
		plive[i].h = 10;
		// Position to the left of the health bar
		plive[i].setPos(info_board_1.pos[0] + offset_x, 40, 0);
		offset_x += 20;
	}
	// turn dim and pos to Vertex
	setVertex();
	// energy bar and recover speed
	energy = 100.0f;
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

// laserCollison is a function test are object and toaster at same area on yres
bool Toaster::laserCollision(Item a)
{
	return (pos[1] > a.pos[1]-a.h && pos[1] < a.pos[1]+a.h && pos[0] < a.pos[0] - a.w - w);
}
bool Toaster::laserCollision(Entity a)
{
	return (pos[1] > a.pos[1]-a.dim[1] && pos[1] < a.pos[1]+a.dim[1] && pos[0] < a.pos[0] - a.dim[0] - w);
}

void Toaster::setDistance(float val)
{
	distance = val;
}

void Toaster::laserDamage(Item& a)
{
	cerr << " a.HP " << a.hp <<endl;
	a.hp = a.hp - laser_damage[bullet_type_prime-5];
	cerr << "make damage " << laser_damage[bullet_type_prime-5] << " a.HP " << a.hp <<endl;
}

void Toaster::laserDamage(Entity& a)
{
	cerr << " a.HP " << a.hp <<endl;
	a.hp = a.hp - laser_damage[bullet_type_prime-5];
	cerr << "make damage " << laser_damage[bullet_type_prime-5] << " a.HP " << a.hp <<endl;
}

void Toaster::posReset()
{
	setPos(g.xres/4, g.yres/2, 0.0);
	score = 0;
	setHP(starting_hp);
	bullet_type_prime = 1;
	bullet_type_minor = 5;
	energy = 100.0f;
}
void Toaster::moveToster()
{
// how toaster move by keys and shoot bullet
	if(screenIn() && !disable_keys) {
		// if the toaster still in the screen then move
		if (g.keys[XK_w]) {
			pos[1] += 4;
		}
		if (g.keys[XK_s]) {
			pos[1] -= 4;
		}
		if (g.keys[XK_a]) {
			pos[0] -= 4;
		}
		if (g.keys[XK_d]) {
			pos[0] += 4;
		}
	}
	else {
		// to keep toaster in the screen
		if(pos[0] > g.xres-w+1) 
			pos[0] = g.xres-w+1;
		if(g.donut_active && pos[0] > (0.7*g.xres)-w+1) 
			pos[0] = (0.7*g.xres)-w-1;
		if(pos[0] < w-1) 
			pos[0] = w-1;
		if(pos[1] > g.yres-h+1) 
			pos[1] = g.yres-h+1;
		if(pos[1] < (3*g.yres/40) + h - 1) 
			pos[1] = (3*g.yres/40) + h-1;
	}
	if (g.keys[XK_space] && bullet_type_prime <= 4) {
		//shoot bullet if not in CD
		if (bullet_type_prime < 5 && g.BulletCD==5) {
			makeBullet(pos[0]+w,pos[1],pos[2],bullet_type_prime);
		}
	}
	if(bullet_type_prime > 4 && bullet_type_prime <= 8) {
		if(g.keys[XK_space] && energy > 5) {
			laserOn = true;
			energy -= 0.2;

		} else {
			laserOn = false;
		}
	}
	if(energy < 100) {
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
	if(laserOn && energy > 5) {
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

	//D.T Draw the lives boxes on the information board
	for(int i = 0; i < lives; i++) {
		glColor3ubv(plive[i].color);
		glPushMatrix();
		glTranslatef(plive[i].pos[0], plive[i].pos[1], plive[i].pos[2]);
		glBegin(GL_QUADS);
			glVertex2f(-plive[i].w, -plive[i].h);
			glVertex2f(-plive[i].w,  plive[i].h);
			glVertex2f( plive[i].w,  plive[i].h);
			glVertex2f( plive[i].w, -plive[i].h);
		glEnd();
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
			setDim (4.0,4.0);
			setColor(240,100,100);
			setDamage(1);
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
			setDamage(1);
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
			setDamage(1);
			setHP(1);
			item_type = 43;
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
	float ya = (2*(pos[1]-tos.pos[1])*(vel[0])*(vel[0]))/((pos[0]-tos.pos[0])*(pos[0]-tos.pos[0]));
    switch (Bread_t) {
	case 1:
		// trace green box
		setDim(15.0,10.0);
		setVel(-4.0, 0.0, 0.0);
		setAcc (0.0,-ya,0.0);
		setColor(100,240,100);
		setDamage(10);
		setHP(2);
		trace = false;
		item_type = 11;
		bullet_type_prime = 1;
		point = 10;
		break;
	case 2:
		// level up box
		setDim(15.0,10.0);
		setVel(-4.0, 0.0, 0.0);
		setAcc(0.0,-ya,0.0);
		setColor(156,25,226);
		setDamage(0);
		setHP(1);
		trace = false;
		bullet_type_prime = 1;
		item_type = 12;
		point = 0;
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
		break;
	case 4:
		// normal green box
		setDim(15.0,10.0);
		setVel(-4.0, 0.0, 0.0);
		setAcc (0.0, 0.0, 0.0);
		setColor(100,240,100);
		setDamage(10);
		setHP(2);
		trace = false;
		item_type = 14;
		bullet_type_prime = 1;
		point = 10;
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

Spear::Spear(){}
Spear::~Spear(){}

void Spear::setSpear(float x, float y,float z, int spear_type) 
{
	setPos(x,y,z);
	setDim(50.0,5.0);
	setVel(-50.0,0.0,0.0);
	setAcc(0.0,0.0,0.0);
	setColor(250, 238, 2);
	setDamage(50);
	setHP(100);
	setCD(100);
	trace = true;
	item_type = 13;
}

void Spear::moveSpear()
{
	if(!trace) {
		pos[0] += vel[0];
		pos[1] += vel[1];
		pos[2] += vel[2];
		vel[0] += acc[0];
		vel[1] += acc[1];
		vel[2] += acc[2];
	}
}

Donut::Donut() 
{
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

void Donut::moveDonut() 
{
	// cerr << weapon << "  " << count_down << endl;
	int val = rand()%7 +1;
 	val = 5;
	if(!weapon) {
		if(count_down == 0) {
			atteckMove(val);
		} else {
			count_down--;
		}
	} else {
		atteckMove(val);
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
		if (hp < 0) {
				return true;
		}
		setCD();
		return false;
}

bool Donut::collision(Item itm) 
{
	float dx = pos[0] - itm.pos[0] - itm.w;
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

void Donut::draw() 
{
	int n = 20;
	float anglein = 3.141592 / n;
	float x0,x1,y0,y1;
	glPushMatrix();
	for(int i = 0; i < n; i++) {
		x0= pos[0]+ out_radius*cos(i*anglein);
		y0= pos[1]+ out_radius*sin(i*anglein);
		x1= pos[0]+ out_radius*cos((i+1)*anglein);
		y1= pos[1]+ out_radius*sin((i+1)*anglein);
		glBegin(GL_TRIANGLES);
		glColor4ub(100, 100, 255, 1);
			glVertex3f(pos[0],pos[1],0);
		glColor4ub(100, 100, 255, 1);
			glVertex3f(x0,y0,0);
			glVertex3f(x1,y1,0);
		glEnd();
		x0= pos[0]+ out_radius*cos(-i*anglein);
		y0= pos[1]+ out_radius*sin(-i*anglein);
		x1= pos[0]+ out_radius*cos(-(i+1)*anglein);
		y1= pos[1]+ out_radius*sin(-(i+1)*anglein);
		glBegin(GL_TRIANGLES);
		glColor4ub(100, 100, 255, 1);
			glVertex3f(pos[0],pos[1],0);
		glColor4ub(100, 100, 255, 1);
			glVertex3f(x0,y0,0);
			glVertex3f(x1,y1,0);
		glEnd();
	}
	if(shelled_on) {
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

 void Donut::atteckMove(int num) 
 {
	if(!weapon) {
		weapon_id = num;
		if (num == 1 || num == 2){
			weapon_outer_count = 10;
			weapon_inner_count = 20;
		}
		if (num == 3) {
			weapon_outer_count = 30;
			weapon_inner_count = 2;
		} 
		if (num == 4) {
			weapon_outer_count = 15;
			weapon_inner_count = 4;
		}
		if (num == 5) {
			weapon_outer_count = 1;
			weapon_inner_count = 20;
		}
		weapon = true;
	} else {
		if(weapon_outer_count == 0) {
			weapon = false;
			count_down = cd;
			return;
		}
		else if (weapon_inner_count == 0) {
			float randangle;
			int rand_pos_neg;
			float accangle;
			float dx;
			float dy;
			float ya;
			int one = 1;
			int negone = -1;
			rand_pos_neg = (rand()%2) * 2 - 1; 			
			switch (weapon_id) {
			case 1:
				accangle = 3.1415926/8;
				for (int i = 0; i < 8; i++) {
					dx = out_radius * cos(1.57 + (i * accangle));
					dy = out_radius * sin(1.57 + (i * accangle));
					do_bul[g.n_donut_bullet].setPos(pos[0]+dx,pos[1]+dy,0);
					do_bul[g.n_donut_bullet].setDim(4.0,4.0);
					do_bul[g.n_donut_bullet].setVel(-5.0, 0.0, 0.0);
					do_bul[g.n_donut_bullet].setAcc (0.0, 0.0, 0.0);
					do_bul[g.n_donut_bullet].setColor(255,240,255);
					do_bul[g.n_donut_bullet].setDamage(10);
					do_bul[g.n_donut_bullet].trace = false;
					do_bul[g.n_donut_bullet].item_type = 16;
					do_bul[g.n_donut_bullet].setVertex();
					g.n_donut_bullet++;
				}
				break;
			case 2:
				accangle = 3.1415926/20;
				for (int i = 0; i < 20; i++) {
					dx = cos(1.57 + (i * accangle));
					dy = sin(1.57 + (i * accangle));
					do_bul[g.n_donut_bullet].setPos(pos[0]+(out_radius * dx),pos[1]+(out_radius * dy),0);
					do_bul[g.n_donut_bullet].setDim(4.0,4.0);
					do_bul[g.n_donut_bullet].setVel(5.0 * dx, 5 * dy, 0.0);
					do_bul[g.n_donut_bullet].setAcc (0.0, 0.0, 0.0);
					do_bul[g.n_donut_bullet].setColor(255,240,255);
					do_bul[g.n_donut_bullet].setDamage(10);
					do_bul[g.n_donut_bullet].trace = false;
					do_bul[g.n_donut_bullet].item_type = 16;
					do_bul[g.n_donut_bullet].setVertex();
					g.n_donut_bullet++;
				}
				break;	
			case 3:
				randangle =(((float)rand()) / (float)RAND_MAX);
				for (int i = 0; i < 2; i++) {
					dx = out_radius * cos(one * (1.57 + 0.5*randangle));
					dy = out_radius * sin(one * (1.57 + 0.5*randangle));
					do_bul[g.n_donut_bullet].setPos(pos[0]+dx,pos[1]+dy,0);
					do_bul[g.n_donut_bullet].setDim(4.0,4.0);
					do_bul[g.n_donut_bullet].setVel(-5.0 , 0.0, 0.0);
					ya = (2*(do_bul[g.n_donut_bullet].pos[1]-tos.pos[1])*(do_bul[g.n_donut_bullet].vel[0])*(do_bul[g.n_donut_bullet].vel[0]))/((do_bul[g.n_donut_bullet].pos[0]-tos.pos[0])*(do_bul[g.n_donut_bullet].pos[0]-tos.pos[0]));
					do_bul[g.n_donut_bullet].setAcc (0.0, -ya, 0.0);
					do_bul[g.n_donut_bullet].setColor(255,240,255);
					do_bul[g.n_donut_bullet].setDamage(10);
					do_bul[g.n_donut_bullet].trace = false;
					do_bul[g.n_donut_bullet].item_type = 16;
					do_bul[g.n_donut_bullet].setVertex();
					g.n_donut_bullet++;
					one *= negone;
				}
				break;
			case 4:
				accangle = 3.1415926/20;
				for (int i = 1; i < 3; i++) {
					dx = out_radius * cos(3.14 - i*accangle);
					dy = out_radius * sin(3.14 - i*accangle);
					do_bul[g.n_donut_bullet].setPos(pos[0]+dx,pos[1]+dy,0);
					do_bul[g.n_donut_bullet].setDim(4.0,4.0);
					do_bul[g.n_donut_bullet].setVel(-5.0 - i, 0.0, 0.0);
					ya = (2*(do_bul[g.n_donut_bullet].pos[1]-tos.pos[1])*(do_bul[g.n_donut_bullet].vel[0])*(do_bul[g.n_donut_bullet].vel[0]))/((do_bul[g.n_donut_bullet].pos[0]-tos.pos[0])*(do_bul[g.n_donut_bullet].pos[0]-tos.pos[0]));
					do_bul[g.n_donut_bullet].setAcc (0.0, -ya, 0.0);
					do_bul[g.n_donut_bullet].setColor(255,240,255);
					do_bul[g.n_donut_bullet].setDamage(10);
					do_bul[g.n_donut_bullet].trace = false;
					do_bul[g.n_donut_bullet].item_type = 16;
					do_bul[g.n_donut_bullet].setVertex();
					g.n_donut_bullet++;
					dx = out_radius * cos(3.14 + i*accangle);
					dy = out_radius * sin(3.14 + i*accangle);
					do_bul[g.n_donut_bullet].setPos(pos[0]+dx,pos[1]+dy,0);
					do_bul[g.n_donut_bullet].setDim(4.0,4.0);
					do_bul[g.n_donut_bullet].setVel(-5.0 - i, 0.0, 0.0);
					ya = (2*(do_bul[g.n_donut_bullet].pos[1]-tos.pos[1])*(do_bul[g.n_donut_bullet].vel[0])*(do_bul[g.n_donut_bullet].vel[0]))/((do_bul[g.n_donut_bullet].pos[0]-tos.pos[0])*(do_bul[g.n_donut_bullet].pos[0]-tos.pos[0]));
					do_bul[g.n_donut_bullet].setAcc (0.0, -ya, 0.0);
					do_bul[g.n_donut_bullet].setColor(255,240,255);
					do_bul[g.n_donut_bullet].setDamage(10);
					do_bul[g.n_donut_bullet].trace = false;
					do_bul[g.n_donut_bullet].item_type = 16;
					do_bul[g.n_donut_bullet].setVertex();
					g.n_donut_bullet++;
				}
				break;
			case 5:
				d_laser[g.n_laser].setDonutLaser(donut.pos[0], donut.pos[1], donut.out_radius,135,225,0.5);
				d_laser[g.n_laser].setCD(100,50,1,1);
				g.n_laser++;
				// d_laser[g.n_laser].setDonutLaser(0,10,6,'h');
				// d_laser[g.n_laser].setCD(10,50,1,1);
				// g.n_laser++;
				break;

			case 6:
				cerr << "in case 6"<<endl;
				d_laser[g.n_laser].setDonutLaser(g.yres_start,6,'h');
				d_laser[g.n_laser].setCD(10,50,1,1);
				g.n_laser++;
				break;

			case 7:
				cerr << "in case 7" <<endl;
				d_laser[g.n_laser].setDonutLaser(donut.pos[0],donut.pos[1], donut.out_radius,90,270,2);
				d_laser[g.n_laser].setCD(100,50,1,1);
				g.n_laser++;
				break;
			
			
			}
			weapon_outer_count--;
			weapon_inner_count = 20;
		} else {
			weapon_inner_count--;
		}
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
	if(laser_type == 2 || laser_type == 3) {
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
	else if(laser_type == 1 || laser_type == 4) {
	// horizental
		vertex[0] = coor_one[0];
		vertex[1] = coor_one[1] + dim;
		vertex[2] =	coor_two[0];
		vertex[3] = coor_two[1] + dim;
		vertex[4] = coor_two[0];
		vertex[5] = coor_two[1] - dim;
		vertex[6] = coor_one[0];;
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
		else {
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
// charge is cd_charge
// lag is cd_lag
// h is the laser hide or not in lag stage
// s decide if Laser can move or not (moveble)
void DonutLaser::setCD(int charge, int lag, bool h, bool s)
{
	colli_tos = false;
	charge_on = true;
	lag_on = false;
	cd_charge = charge;
	cd_lag = lag;
	cd_stay = 100;
	if(charge != 0){
		alpha = 0;
		alpha_inc = 255 / charge;
	} else {
		alpha = 255;
		alpha_inc = 0;
	}
	hide = h;
	moveble = s;
	dim = 4;
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

// slop laser, move horizental or vertical
void DonutLaser::setDonutLaser(float pos, float angle, float speed, char hor_or_ver){
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
void DonutLaser::setDonutLaser(float xcenter, float ycenter, float r, float anglestart, float angleend, float anglespeed){
	cout << "make 5 laser" << endl;
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
	limit_angle[1] = atan( (g.yres - center[1]) / (0 - center[0])) + 3.1415926;
	limit_angle[2] = atan( (g.yres_start - center[1]) / (0 - center[0])) + 3.1415926;
	limit_angle[3] = atan( (g.yres - center[1]) / (g.xres - center[0])) + 6.2831853;
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
	cerr << slop << endl;
	if(charge_on){
	// charge and change alpha
		if(cd_charge > 0) {
			cd_charge--;
			alpha += alpha_inc;
		} else {
			charge_on = false;
			lag_on = true;
		}
	} else if(lag_on) {
	// lag give player time to react 
	// should not have collision test when lag
		if(cd_lag > 0) {
			cd_lag--;
		} else {
			lag_on = false;
		}
	} else {
	// move laser if is moveble 
	// countdown if not moveble
		if(moveble) {
			// can move
			if(laser_type != 5) {
				// not rotate
				coor_one[0] += vel_one[0];
				coor_one[1] += vel_one[1];
				coor_two[0] += vel_two[0];
				coor_two[1] += vel_two[1];
			} else {
				// rotate laser
				slop += angleacc;
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
					coor_two[0] = center[0] - ((g.yres-center[1])/ tan(slop));
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
			if(cd_stay > 0) {
				cd_stay--;
			}
		}
	}
}

bool DonutLaser::collision(Item itm){
	if(!(charge_on||lag_on)) {
		if (laser_type == 1) {
			// horizental line move up or down
			return !((coor_one[1] - dim - itm.h - itm.pos[1] > 0)||(itm.pos[1] - coor_one[1] - dim - itm.h > 0));
		}
		else if (laser_type == 2) {
			// horizental line move up or down
			return !((coor_one[0] - dim - itm.w - itm.pos[0] > 0)||(itm.pos[0] - coor_one[0] - dim - itm.w > 0));
		}
		else if (laser_type == 3) {
			if(slop > 0) {
				return !((itm.pos[1] - itm.h) > slop * (itm.pos[0] + itm.w - coor_one[0] - dim)
						|| (itm.pos[1] + itm.h) < slop * (itm.pos[0] - itm.h - coor_one[1] + dim));
			} else {
				return !((itm.pos[0] + itm.w) < slop * (itm.pos[1] - itm.h - coor_one[1] - dim)
						|| (itm.pos[0] - itm.w) > slop * (itm.pos[1] + itm.h - coor_one[1] + dim));
			}
		}
		else if (laser_type == 4) {
			//slop and move up down
			if (slop > 0) {
				return !((itm.pos[0] + itm.w) < slop * (itm.pos[1] - itm.h - coor_one[1] - dim)
						|| (itm.pos[0] - itm.w) > slop * (itm.pos[1] + itm.h - coor_one[1] + dim));
			} else {
				return !((itm.pos[0] + itm.w) > slop * (itm.pos[1] - itm.h - coor_one[1] - dim)
						|| (itm.pos[0] - itm.w) < slop * (itm.pos[1] + itm.h - coor_one[1] + dim));
			}
		}
		else if (laser_type == 5) {
			if (slop > 0 && slop < 1.570796327) {
				if((itm.pos[0] - itm.w) > coor_one[0] && (itm.pos[1] - itm.h) > coor_one[1]) {
					return (((itm.pos[1] - itm.h - coor_one[1]) / (itm.pos[0] + itm.w - coor_one[0]) < tan(slop)) 
						&& ((itm.pos[1] + itm.h - coor_one[1]) / (itm.pos[0] - itm.w - coor_one[0]) > tan(slop)));
				}
				return false;
			}
			else if (slop > 1.570796327 && slop < 3.1415927) {
				if((itm.pos[0] + itm.w) < coor_one[0] && (itm.pos[1] - itm.h) > coor_one[1]) {
					return (((itm.pos[1] - itm.h - coor_one[1]) / (coor_one[0] - itm.pos[0] + itm.w) < -tan(slop)) 
						&& ((itm.pos[1] + itm.h - coor_one[1]) / (coor_one[0] - itm.pos[0] - itm.w) > -tan(slop)));
				}
				return false;			
			}
			else if (slop > 3.1415926 && slop < 4.71238898) {
				if((itm.pos[0] + itm.w) < coor_one[0] && (itm.pos[1] + itm.h) < coor_one[1]) {
					return (((coor_one[1] - itm.pos[1] - itm.h) / (coor_one[0] - itm.pos[0] + itm.w) < tan(slop)) 
						&& ((coor_one[1] - itm.pos[1] + itm.h) / (coor_one[0] - itm.pos[0] - itm.w) > tan(slop)));
				}
				return false;	
			}
		} else {
			if((itm.pos[0] - itm.w) > coor_one[0] && (itm.pos[1] + itm.h) < coor_one[1]) {
				return (((itm.pos[1] - itm.h - coor_one[1]) / (coor_one[0] - itm.pos[0] + itm.w) < -tan(slop)) 
					&& ((itm.pos[1] + itm.h - coor_one[1]) / (coor_one[0] - itm.pos[0] - itm.w) > -tan(slop)));
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
	if(moveble) {
		if(laser_type == 1) {
			return (coor_one[1] < g.yres_start || coor_one[1] > g.yres);
		}
		else if(laser_type == 2) {
			return (coor_one[0] < 0 || coor_one[0] > g.xres);
		}
		else if(laser_type == 3) {
			return ((coor_one[0] < 0 && coor_two[0] < 0)
					|| (coor_one[0] > g.xres && coor_two[0] > g.xres));
		}
		else if(laser_type == 4) {
			return ((coor_one[1] < g.yres_start && coor_two[1] < g.yres_start)
					|| (coor_one[1] < g.yres && coor_two[1] < g.yres));
		}
		else if(laser_type == 5) {
			if(angleacc > 0) {
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
	if(charge_on) {
		glPushMatrix();
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(alpha,0,0,alpha);
		glBegin(GL_QUADS);
			glVertex2f(vertex[0],vertex[1]);
			glVertex2f(vertex[2],vertex[3]);
			glVertex2f(vertex[4],vertex[5]);
			glVertex2f(vertex[6],vertex[7]);
		glEnd();
		glPopMatrix();
	} else if (lag_on) {
		if(!hide) {
			glPushMatrix();
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(alpha,0,0,alpha);
			glBegin(GL_QUADS);
				glVertex2f(vertex[0],vertex[1]);
				glVertex2f(vertex[2],vertex[3]);
				glVertex2f(vertex[4],vertex[5]);
				glVertex2f(vertex[6],vertex[7]);
			glEnd();
			glPopMatrix();
		}
	} else {

		glPushMatrix();
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(100,0,0,alpha);
		glBegin(GL_QUADS);
			glVertex2f(vertex[0],vertex[1]);
		glColor4ub(255,100,100,255);
			glVertex2f(vertex[2],vertex[3]);
			glVertex2f(coor_two[0],coor_two[1]);
			glVertex2f(coor_one[0],coor_one[1]);
		glEnd();
		glBegin(GL_QUADS);
			glVertex2f(coor_one[0],coor_one[1]);
			glVertex2f(coor_two[0],coor_two[1]);
		glColor4ub(20,0,0,alpha);
			glVertex2f(vertex[4],vertex[5]);
			glVertex2f(vertex[6],vertex[7]);
		glEnd();		
		glPopMatrix();
	}
}


ChargeBread::ChargeBread()
{
	charge_on = true;
	charge_dim[0] = 0;
	charge_dim[1] = 0;
	charge_dim_acc[0] = 0;
	charge_dim_acc[1] = 0;
    charge_need = 0;
    charge_num_now = 0;
	
}

ChargeBread::~ChargeBread() 
{
}

void ChargeBread::setDimAcc()
{

}

void ChargeBread::charge() {

}
void ChargeBread::moveChargeBread() {

}













//=======================================================================================//
//math functions//
bool pointIn(float x0, float y0, float * arr, int n) 
{
	float point[n/2];
	for(int i = 0; i < (n/2); i++)
	{
		point[i] = crossX(x0,y0,(arr[2*i]),(arr[2*i+1]),(arr[(2*i+2)%n]),(arr[(2*i+3)%n]));
	}
	float large=0;
	float small=0;
	for(int i = 0; i < (n/2); i++)
	{
		if(large < point[i])
			small = large;
			large = point[i];
		if(large > point[i] && small < point[i])
			small = point[i];
	}
	cout << large << " " << small <<endl;
	if (x0 > large || x0 < small)
		return false;
	else
		return true;
}

float crossX(float x0,float y0,float xa,float ya,float xb,float yb) 
{
	float result = (ya-((ya-yb)*xa/(xa-xb)))/((y0/x0)-((ya-yb)/(xa-xb)));
	if(result < fmin(xa,xb)|| result > fmax(xa,xb))
		return 0;
	else
		return result;
}

float maxRadius(float *arr,int n) 
{
	float result = 0.0f;
	for(int i = 0; i < (n/2); i++) {
		if(result < ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1])))
			result = ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1]));
	}
	return result;
}

float minRadius(float *arr,int n) 
{
	float result = ((arr[0]*arr[0])+(arr[1]*arr[1]));
	for(int i = 0; i < (n/2); i++) {
		if(result > ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1])))
			result = ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1]));
	}
	return result;
}

float maxTan(float *arr,int n) 
{
	float result = 0.0f;
	for(int i = 0; i < (n/2); i++) {
		if(result < (arr[2 * i] / arr[2 * i + 1]))
			result = (arr[2 * i] / arr[2 * i + 1]);
	}
	return result;
}
float minTan(float *arr,int n) 
{
	float result = (arr[0] / arr[1]);
	for(int i = 0; i < (n/2); i++) {
		if(result > (arr[2 * i] / arr[2 * i + 1]))
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
// 	if(R0 < r1 || R1 < r0 || T0 < t1 || T1 < t0)
// 		return false;
// 	else {
// 		bool delta = false;
// 		for(int i = 0; i < 4; i++) {
// 			if(!delta)
// 				delta = pointIn(pos[0]+vertex[i*2],pos[1]+vertex[i*2+1],aArr,8);
// 		}
// 		for(int i = 0; i < 4; i++) {
// 			if(!delta)
// 				delta = pointIn(a.pos[0]+a.vertex[i*2],a.pos[1]+a.vertex[i*2+1],arr,8);
// 		}
// 		return delta;
// 	}
// }

///====================================Draw shape Functions=======================
//===============================================================================
