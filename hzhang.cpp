//Huaiyu Zhang
//Feb 24
#include <iostream>
#include <fstream>
// #include <string>
#include <cmath>
#include "hzhang.h"
#include "Global.h"
#include "mkausch.h"
#include "fonts.h"
#include <new>
#include <algorithm>
#include <cstring>

using namespace std;

void makeBullet(float x, float y,float z, bool tb, int bullet_type) {
		switch(bullet_type){
			case 1:
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].setBullet(x, y, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
					}
				break;
			case 2:
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].setBullet(x, y+5, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
					}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].setBullet(x, y-5, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
					}
				break;
			case 3:
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].setBullet(x, y, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].setBullet(x, y+5, z, tb, 2);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].setBullet(x, y-5, z, tb, 3);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				break;
			case 4:
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].setBullet(x, y-5, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].setBullet(x, y+5, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].setBullet(x, y+8, z, tb, 2);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].setBullet(x, y-8, z, tb, 3);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				break;
			case 5:
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].setBullet(x, y, z, tb, 1);
						++g.n_Bullet;
				}
		}

}
void makeBread(float x, float y,float z, int Bread_t, int type) {
  if (g.n_Bread < MAX_bread){
      bread[g.n_Bread].setBread(x, y, z, Bread_t, type);
      ++g.n_Bread;
      // cout<<"make Bread!!!"<<endl;
    }
}


//==================================================================================
Item::Item(){
	tex = nullptr;	// assign texture to null in the base class
					// this var should be overridden in classes with textures
}
Item::~Item(){
}

void Item::setHP(float life) {
    hp = life;
}
void Item::setCD(int x) {
    cd = x;
}
void Item::setDamage(float x) {
    damage = x;
}

void Item::setVel(float x, float y, float z) {
    //set the velocity of item
    vel[0] = x;
    vel[1] = y;
    vel[2] = z;
}

void Item::setAcc(float x, float y, float z) {
    //set the acceleration of item
    acc[0] = x;
    acc[1] = y;
    acc[2] = z;
}

void Item::setVertex() {
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
void Item::setTrace(Item tos) {
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
bool Item::hpCheck() {
    // if no hp return true
    return (hp < 0.01);
}

bool Item::collision(Item a) {
    // for (x0,y0,x1,y1)and(x2,y2,x3,y3) squares
    // if collison -->(x0-x3)*(x1-x2)<0
    // same for y
    bool x = (((pos[0]+w)-(a.pos[0]-a.w))*((pos[0]-w)-(a.pos[0]+a.w))) < 0;
  	bool y = (((pos[1]+h)-(a.pos[1]-a.h))*((pos[1]-h)-(a.pos[1]+a.h))) < 0;
  	return x&&y;
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
void Item::hpDamage(Item a) {
    // std::cout << "hp 1 :" << hp << "Damage :" << a.damage<< std::endl;
    hp = hp - a.damage;
    // std::cout << "hp 2 :" << hp << std::endl;
}

bool Item::screenIn() {
    //check if item reach the edge of screen
    bool a = (pos[0] <= g.xres-w+1);
    bool b = (pos[0] >= w-1);
    bool c = (pos[1] <= g.yres-h+1);
		bool d = (pos[1] >= ((3*g.yres/40)+h-1));
    return a&&b&&c&&d;
}

bool Item::screenOut() {
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
		score = 0;
		bullet_type_prime = 1;
		bullet_type_minor = 5;
		item_type = 0;
		setPos(g.xres/4, g.yres/2, 0.0);
		setColor(188, 226, 232);
		setDim(30,25);
		starting_hp = 80;
		setHP(starting_hp);
		setDamage(100);
		lives = 3;
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
		setVertex();
		energy = 100.0f;
		energy_recover = 0.1f;
		disable_keys = false;
		laserOn = false;
		tex = &g.toaster_silhouette;
}

Toaster::~Toaster()
{
	delete [] plive;
}

bool Toaster::laserCollision(Item a){
		return (pos[1] > a.pos[1]-a.h && pos[1] < a.pos[1]+a.h && pos[0] < a.pos[0] - a.w - w);
}
void Toaster::setDistance(float val){
		distance = val;
}
void Toaster::laserDamage(Item& a){
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
		energy = 100.0f;
}
void Toaster::moveToster()
{
  // how toaster move by keys and shoot bullet
   if(screenIn() && !disable_keys) {
      // if the toaster still in the screen then move
      if (g.keys[XK_w]){
    			pos[1] += 4;
    			// std::cout << "move w"<<pos[1]<<std::endl;
    	}
    	if (g.keys[XK_s]){
    			pos[1] -= 4;
    			// std::cout << "move s"<<pos[1]<<std::endl;
    	}
    	if (g.keys[XK_a]){
    			pos[0] -= 4;
    			// std::cout << "move a"<<pos[0]<<std::endl;
    	}
    	if (g.keys[XK_d]){
    			pos[0] += 4;
    			// std::cout << "move d"<<pos[0]<<std::endl;
    	}
  }
  else {
		// to keep toaster in the screen
		if(pos[0] > g.xres-w+1) pos[0] = g.xres-w+1;
		if(pos[0] < w-1) pos[0] = w-1;
		if(pos[1] > g.yres-h+1) pos[1] = g.yres-h+1;
				if(pos[1] < (3*g.yres/40) + h - 1) pos[1] = (3*g.yres/40) + h-1;
  }
  if (g.keys[XK_space] && bullet_type_prime <= 4) {
      //shoot bullet if not in CD
		if (bullet_type_prime < 5 && g.BulletCD==5) {
				makeBullet(pos[0]+w,pos[1],pos[2],1,bullet_type_prime);
				// std::cout << "shoot"<<std::endl;
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

void Toaster::bulletReload(){
		int temp;
		temp = bullet_type_prime;
		bullet_type_prime = bullet_type_minor;
		bullet_type_minor = temp;
		laserOn = false;
}

void Toaster::tdraw(){
	if(laserOn && energy > 5) {
			glPushMatrix();
			glColor3ub(20,0,0);
			glTranslatef(pos[0]+w, pos[1], pos[2]);
			glBegin(GL_QUADS);
					glVertex2f(0,laser_h[bullet_type_prime - 5]);
					glColor3ub(20,0,0);
					glVertex2f(0,-laser_h[bullet_type_prime - 5]);
					glColor3ub(255, 0, 100);
					glVertex2f(distance,-laser_h[bullet_type_prime - 5]);
					glColor3ub(255, 100, 0);
					glVertex2f(distance,laser_h[bullet_type_prime - 5]);
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

Bullet::Bullet() {
    item_type = 41;
}
Bullet::~Bullet() {}

void Bullet::setBullet(float x, float y, float z, bool tb, int type) {
    setPos (x,y,z);
    if (tb) {
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
    }
    else {
      //Bread bullet
        switch (type) {
          case 1:
            // bullet type 1
            // straight line
            setVel (-6.0,0.0,0.0);
            setDim (4.0,4.0);
            break;
          case 2:
            // bullet type 2
            // up 10degree
            setVel (-6.0,1.05,0.0);
            setDim (4.0,4.0);
            break;
          case 3:
            // bullet type 3
            // down 10degree
            setVel (-6.0,-1.05,0.0);
            setDim (4.0,4.0);
            break;
          case 4:
            // bullet type 4
            // track the origen point of Toaster
            setDim (4.0,4.0);
            setVel (-6.0,0.0,0.0);
            float ya = (2*(pos[1]-tos.pos[1])*(vel[0])*(vel[0]))/((pos[0]-tos.pos[0])*(pos[0]-tos.pos[0]));
            setAcc (0.0,ya,0.0);
        }
    }
		setVertex();
}

void Bullet::moveBullet() {
    pos[0] += vel[0];
    pos[1] += vel[1];
    pos[2] += vel[2];
    vel[0] += acc[0];
    vel[1] += acc[1];
    vel[2] += acc[2];
}

Bread::Bread() {
    item_type = 11;
}
Bread::~Bread() {}

void Bread::setBread(float x, float y,float z, int Bread_t, int type) {
    setPos (x,y,z);
		float ya = (2*(pos[1]-tos.pos[1])*(vel[0])*(vel[0]))/((pos[0]-tos.pos[0])*(pos[0]-tos.pos[0]));
    switch (Bread_t) {
     	case 1:
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

void Bread::moveBread() {
    pos[0] += vel[0];
    pos[1] += vel[1];
    pos[2] += vel[2];
    vel[0] += acc[0];
    vel[1] += acc[1];
    vel[2] += acc[2];
}
//=========================================================================



//=======================================================================================//
//math functions//
bool pointIn(float x0, float y0, float * arr, int n) {
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

float crossX(float x0,float y0,float xa,float ya,float xb,float yb) {
		float result = (ya-((ya-yb)*xa/(xa-xb)))/((y0/x0)-((ya-yb)/(xa-xb)));
		if(result < fmin(xa,xb)|| result > fmax(xa,xb))
			return 0;
		else
			return result;
}

float maxRadius(float *arr,int n) {
		float result = 0.0f;
		for(int i = 0; i < (n/2); i++) {
				if(result < ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1])))
						result = ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1]));
		}
		return result;
}

float minRadius(float *arr,int n) {
		float result = ((arr[0]*arr[0])+(arr[1]*arr[1]));
		for(int i = 0; i < (n/2); i++) {
				if(result > ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1])))
						result = ((arr[i*2]*arr[i*2])+(arr[i*2+1]*arr[i*2+1]));
		}
		return result;
}

float maxTan(float *arr,int n) {
		float result = 0.0f;
		for(int i = 0; i < (n/2); i++) {
				if(result < (arr[2 * i] / arr[2 * i + 1]))
						result = (arr[2 * i] / arr[2 * i + 1]);
		}
		return result;
}
float minTan(float *arr,int n) {
		float result = (arr[0] / arr[1]);
		for(int i = 0; i < (n/2); i++) {
				if(result > (arr[2 * i] / arr[2 * i + 1]))
						result = (arr[2 * i] / arr[2 * i + 1]);
		}
		return result;
}
///====================================Draw shape Functions=======================
//===============================================================================
