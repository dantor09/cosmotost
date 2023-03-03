#include "Global.h"

Global::Global()
{
		xres = 1280;
		yres = 720;
	  show_help_menu = false;
		levelchance = 30;
}


Global::~Global()
{
	// if (gameTimer != nullptr) {
	// 	delete gameTimer;
	// 	gameTimer = nullptr;
	// }
}

Global::GameReset()
{
		n_Bullet = 0;
		n_Bread = 0;
		tos.posReset();
}
