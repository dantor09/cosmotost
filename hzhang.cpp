//Huaiyu Zhang
//Feb 24
#include <iostream>
#include <string>
#include "hzhang.h"
#include "Global.h"

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
    HP = x;
}

void Item::set_vel(float x, float y, float z) {
    //set the velocity of item
    vel[0] = x;
    vel[1] = y;
    vel[2] = z;
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

}

Toaster::~Toaster()
{
}


void Toaster::MoveToster()
{
  // how toaster move by keys and shoot bullet
  if(ScreenIn()) {
      // if the toaster still in the screen then move
      if (g.keys[XK_w]){
    			pos[1] += 4;
    			std::cout << "move w"<<pos[1]<<std::endl;
    	}
    	if (g.keys[XK_s]){
    			pos[1] -= 4;
    			std::cout << "move s"<<pos[1]<<std::endl;
    	}
    	if (g.keys[XK_a]){
    			pos[0] -= 4;
    			std::cout << "move a"<<pos[0]<<std::endl;
    	}
    	if (g.keys[XK_d]){
    			pos[0] += 4;
    			std::cout << "move d"<<pos[0]<<std::endl;
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
					std::cout << "shoot"<<std::endl;
			}
  }
}
string Toaster::PrintScore() {
    ostringstream temp;
    temp << "Score : " << score << endl;
  return temp.str();
}

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
            break;

        }
    }
    else {
      //Bread bullet
        switch (type) {
          case 1:
            // bullet type 1
            set_vel (-6.0,0.0,0.0);
            set_dim (4.0,4.0);
            break;

        }
    }
}

void Bullet::MoveBullet() {
    pos[0] += vel[0];
    pos[1] += vel[1];
    pos[2] += vel[2];
}

Bread::Bread() {
    item_type = 11;
}
Bread::~Bread() {}

void Bread::set_Bread(float x, float y,float z, int Bread_t, int type) {
    set_pos (x,y,z);
    switch (Bread_t) {
      case 1:
          set_dim(15.0,10.0);
          set_vel(-4.0, 0.0, 0.0);
          set_color(100,240,100);
          b_type = 1;
          point = 10;
          break;
    }
}

void Bread::MoveBread() {
    pos[0] += vel[0];
    pos[1] += vel[1];
    pos[2] += vel[2];
}
