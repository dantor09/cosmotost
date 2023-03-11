#include "Global.h"

// class EntitySpawn;
// inline EntitySpawn e;

Global::Global()
{
		xres = 1280;
		yres = 720;
	  show_help_menu = false;
		levelchance = 30;
}


Global::~Global()
{

}

void Global::GameReset()
{
		n_Bullet = 0;
		n_Bread = 0;
		e.numEnt = 0;
		record.GetR();
		for(int i = 0; i <10; i++)
				record.gamer[i]='_';
		tos.posReset();
}
