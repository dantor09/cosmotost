//Huaiyu Zhang
//Feb 24
#include <iostream>
#include <string>
#include <cmath>
#include "hzhang.h"
#include "Global.h"
#include <fstream>
using namespace std;


void make_Bullet(float x, float y,float z, bool tb, int type) {
		switch(type){
			case 1:
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].set_Bullet(x, y, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
					}
				break;
			case 2:
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].set_Bullet(x, y+5, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
					}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].set_Bullet(x, y-5, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
					}
				break;
			case 3:
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].set_Bullet(x, y, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].set_Bullet(x, y+5, z, tb, 2);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].set_Bullet(x, y-5, z, tb, 3);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				break;
			case 4:
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].set_Bullet(x, y-5, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].set_Bullet(x, y+5, z, tb, 1);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].set_Bullet(x, y+8, z, tb, 2);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
				if (g.n_Bullet < MAX_bullet){
						bul[g.n_Bullet].set_Bullet(x, y-8, z, tb, 3);
						++g.n_Bullet;
						// cout<<"make bullet"<<endl;
				}
		}

}
void make_Bread(float x, float y,float z, int Bread_t, int type) {
  if (g.n_Bread < MAX_bread){
      bread[g.n_Bread].set_Bread(x, y, z, Bread_t, type);
      ++g.n_Bread;
      // cout<<"make Bread!!!"<<endl;
    }
}


//==================================================================================
Item::Item(){
}
Item::~Item(){
}

void Item::set_HP(int life) {
    HP = life;
}
void Item::set_CD(int x) {
    CD = x;
}
void Item::set_damage(int x) {
    damage = x;
}

void Item::set_vel(float x, float y, float z) {
    //set the velocity of item
    vel[0] = x;
    vel[1] = y;
    vel[2] = z;
}

void Item::set_acc(float x, float y, float z) {
    //set the acceleration of item
    acc[0] = x;
    acc[1] = y;
    acc[2] = z;
}
// to check toaster HP, if <=0 then dead
bool Item::HP_check() {
    // if no HP return true
    return (HP <= 0);
}

bool Item::Collison(Item a) {
    // for (x0,y0,x1,y1)and(x2,y2,x3,y3) squares
    // if collison -->(x0-x3)*(x1-x2)<0
    // same for y
    bool x = (((pos[0]+w)-(a.pos[0]-a.w))*((pos[0]-w)-(a.pos[0]+a.w))) < 0;
  	bool y = (((pos[1]+h)-(a.pos[1]-a.h))*((pos[1]-h)-(a.pos[1]+a.h))) < 0;
  	return x&&y;
}

void Item::HPdamage(Item a) {
    // std::cout << "HP 1 :" << HP << "Damage :" << a.damage<< std::endl;
    HP = HP - a.damage;
    // std::cout << "HP 2 :" << HP << std::endl;
}

bool Item::ScreenIn() {
    //check if item reach the edge of screen
    bool a = (pos[0] <= g.xres-w+1);
    bool b = (pos[0] >= w-1);
    bool c = (pos[1] <= g.yres-h+1);
    bool d = (pos[1] >= h-1);
    return a&&b&&c&&d;
}

bool Item::ScreenOut() {
    //check if item out of screen
    bool a = (pos[0] >= g.xres+w);
    bool b = (pos[0] <= -w);
    bool c = (pos[1] >= g.yres+h);
    bool d = (pos[1] <= -h);
    return a||b||c||d;
}

void Item::draw()
{
    // draw item
    
    glPushMatrix();
  	glColor3ub(color[0], color[1], color[2]);
  	glTranslatef(pos[0], pos[1], pos[2]);
  	glBegin(GL_QUADS);
  			glVertex2f(-w, -h);
  			glVertex2f(-w,  h);
  			glVertex2f( w,  h);
  			glVertex2f( w, -h);
  	glEnd();
  	glPopMatrix();

}

Toaster::Toaster()
{
    // To set item_type 0
    // the origen bullet type will be 1
    score = 0;
    b_type = 1;
    item_type = 0;
    set_pos(g.xres/4, g.yres/2, 0.0);
    set_color(188, 226, 232);
    set_dim(20,15);
    starting_hp = 80;
		set_HP(starting_hp);
		set_damage(100);
    lives = 1;

}

Toaster::~Toaster()
{
}

void Toaster::posReset()
{
		set_pos(g.xres/4, g.yres/2, 0.0);
		score = 0;
		set_HP(starting_hp);
}
void Toaster::MoveToster()
{
  // how toaster move by keys and shoot bullet
  if(ScreenIn()) {
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
      if(pos[1] < h-1) pos[1] = h-1;
  }
  if (g.keys[XK_space]) {
      //shoot bullet if not in CD
			if (g.BulletCD==5) {
					make_Bullet(pos[0]+w,pos[1],pos[2],1,b_type);
					// std::cout << "shoot"<<std::endl;
			}
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

void Bullet::set_Bullet(float x, float y, float z, bool tb, int type) {
    set_pos (x,y,z);
    if (tb) {
      //Toaster bullet
        switch (type) {
          case 1:
            // bullet type 1
            set_vel (10.0,0.0,0.0);
            set_dim (4.0,4.0);
            set_color(240,100,100);
            set_damage(1);
            set_HP(1);
            break;
          case 2:
            // bullet type 2
            // up 10degree
            set_vel (10.0,1.76,0.0);
            set_dim (4.0,4.0);
            set_color(240,100,100);
            set_damage(1);
            set_HP(1);
            break;
          case 3:
            // bullet type 3
            // down 10degree
            set_vel (10.0,-1.76,0.0);
            set_dim (4.0,4.0);
            set_color(240,100,100);
            set_damage(1);
            set_HP(1);
            break;

        }
    }
    else {
      //Bread bullet
        switch (type) {
          case 1:
            // bullet type 1
            // straight line
            set_vel (-6.0,0.0,0.0);
            set_dim (4.0,4.0);
            break;
          case 2:
            // bullet type 2
            // up 10degree
            set_vel (-6.0,1.05,0.0);
            set_dim (4.0,4.0);
            break;
          case 3:
            // bullet type 3
            // down 10degree
            set_vel (-6.0,-1.05,0.0);
            set_dim (4.0,4.0);
            break;
          case 4:
            // bullet type 4
            // track the origen point of Toaster
            set_dim (4.0,4.0);
            set_vel (-6.0,0.0,0.0);
            float ya = (2*(pos[1]-tos.pos[1])*(vel[0])*(vel[0]))/((pos[0]-tos.pos[0])*(pos[0]-tos.pos[0]));
            set_acc (0.0,ya,0.0);
        }
    }
}

void Bullet::MoveBullet() {
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

void Bread::set_Bread(float x, float y,float z, int Bread_t, int type) {
    set_pos (x,y,z);
		float ya = (2*(pos[1]-tos.pos[1])*(vel[0])*(vel[0]))/((pos[0]-tos.pos[0])*(pos[0]-tos.pos[0]));
    switch (Bread_t) {
      case 1:
          set_dim(15.0,10.0);
          set_vel(-4.0, 0.0, 0.0);
          set_acc (0.0,-ya,0.0);
          set_color(100,240,100);
          set_damage(10);
          set_HP(2);
					item_type = 11;
          b_type = 1;
          point = 10;
          break;
			case 2:
          set_dim(15.0,10.0);
          set_vel(-4.0, 0.0, 0.0);
          set_acc (0.0,-ya,0.0);
          set_color(156,25,226);
          set_damage(0);
          set_HP(0);
          b_type = 1;
					item_type = 12;
          point = 0;
          break;
    }
}

void Bread::MoveBread() {
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
void Gamerecord::GetR()
{
	ifstream fin("Highscore.txt");
	if (fin.fail()) {
		getline(fin,reName);
		fin.getline(gamer,10);
		if(atoi(gamer) > highscore)
				highscore = atoi(gamer);
	}

}
void Gamerecord::ChangeR(int s)
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
		g.state = MAINMENU;
}
Gamerecord::~Gamerecord(){}
