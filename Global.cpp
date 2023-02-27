#include "Global.h"

Global::Global()
{
	xres = 1280;
	yres = 720;
  show_help_menu = false;
}


Global::~Global(){}


void make_Bullet(float x, float y,float z, bool tb, int type) {
  if (g.n_Bullet < MAX_bullet){
      bul[g.n_Bullet].set_Bullet(x, y, z, tb, type);
      ++g.n_Bullet;
      // cout<<"make bullet"<<endl;
    }

}
void make_Bread(float x, float y,float z, int Bread_t, int type) {
  if (g.n_Bread < MAX_bread){
      bread[g.n_Bread].set_Bread(x, y, z, Bread_t, type);
      ++g.n_Bread;
      // cout<<"make Bread!!!"<<endl;
    }
}
