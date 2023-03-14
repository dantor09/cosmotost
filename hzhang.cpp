//Huaiyu Zhang
//Feb 24
#include <iostream>
#include <string>
#include <cmath>
#include "hzhang.h"
#include "Global.h"
#include <fstream>
using namespace std;


void makeBullet(float x, float y,float z, bool tb, int type) {
		switch(type){
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
}
Item::~Item(){
}

void Item::setHP(int life) {
    hp = life;
}
void Item::setCD(int x) {
    cd = x;
}
void Item::setDamage(int x) {
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
    return (hp <= 0);
}

bool Item::collision(Item a) {
    // for (x0,y0,x1,y1)and(x2,y2,x3,y3) squares
    // if collison -->(x0-x3)*(x1-x2)<0
    // same for y
    bool x = (((pos[0]+w)-(a.pos[0]-a.w))*((pos[0]-w)-(a.pos[0]+a.w))) < 0;
  	bool y = (((pos[1]+h)-(a.pos[1]-a.h))*((pos[1]-h)-(a.pos[1]+a.h))) < 0;
  	return x&&y;
}

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
    b_type = 1;
    item_type = 0;
    setPos(g.xres/4, g.yres/2, 0.0);
    setColor(188, 226, 232);
    setDim(20,15);
    starting_hp = 80;
		setHP(starting_hp);
		setDamage(100);
    lives = 1;
		setVertex();
		energy = 100.0f;
		energy_recover = 0.01f;
		disable_keys = false;
}

Toaster::~Toaster()
{
}

void Toaster::posReset()
{
		setPos(g.xres/4, g.yres/2, 0.0);
		score = 0;
		setHP(starting_hp);
		b_type = 1;
		energy = 100.0f;
}
void Toaster::moveToster()
{
  // how toaster move by keys and shoot bullet
   if(ScreenIn() && !disable_keys) {
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
  if (g.keys[XK_space]) {
      //shoot bullet if not in CD
			if (g.BulletCD==5) {
					makeBullet(pos[0]+w,pos[1],pos[2],1,b_type);
					// std::cout << "shoot"<<std::endl;
			}
  }
	if(energy < 100)
	{
			energy += energy_recover;
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
          b_type = 1;
          point = 10;
          break;
			case 2:
          setDim(15.0,10.0);
          setVel(-4.0, 0.0, 0.0);
          setAcc(0.0,-ya,0.0);
          setColor(156,25,226);
          setDamage(0);
          setHP(0);
					trace = false;
          b_type = 1;
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
					b_type = 1;
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
					b_type = 1;
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


Gamerecord::Gamerecord()
{
		n = 0;
		ifstream fin("Highscore.txt");
		if (fin.fail()) {
				ofstream outfile("Highscore.txt");
				outfile.close();
				delt = true;
		} else {
				getline(fin,reName);
				if(!fin.eof()) {
						fin.getline(gamer,10);
						if(atoi(gamer) > highscore)
								highscore = atoi(gamer);
						delt = false;
				}
				delt = true;
		}
		for(int i = 0; i <10; i++)
				gamer[i]='_';
}
void Gamerecord::getRecord()
{
	ifstream fin("Highscore.txt");
	if (fin.fail()) {
		getline(fin,reName);
		fin.getline(gamer,10);
		if(atoi(gamer) > highscore)
				highscore = atoi(gamer);
	}

}
void Gamerecord::changeRecord(int s)
{
		if (record.delt) {
				record.str = record.gamer;
				ofstream outfile("Highscore.txt");
				outfile << record.str << endl;
				outfile << s << endl;
				outfile.close();
		} else {
				ofstream outfile;
				outfile.open("Highscore.txt", ios::out | ios::trunc);
				outfile << record.str << endl;
				outfile << s << endl;
				outfile.close();
		}
}
Gamerecord::~Gamerecord(){}
