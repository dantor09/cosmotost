#include "Global.h"

// class EntitySpawn;
// inline EntitySpawn e;

Global::Global()
{
	xres = 1280;
	yres = 720;
	yres_start = 3*yres/40;
	show_help_menu = false;
	levelchance = 15;
	sfx_vol = 1.0f;
	m_vol = 1.0f;
}


Global::~Global()
{

}

void Global::gameReset()
{
	n_Bullet = 0;
	n_Bread = 0;
	n_Spear = 0;
	n_donut_bullet = 0;
	n_laser = 0;
	e.num_ent = 0;
	// record.getRecord();
	// for(int i = 0; i <10; i++)
	// 	record.gamer[i]=' ';
	tos.posReset();
	vblocky.gamereset();
}
