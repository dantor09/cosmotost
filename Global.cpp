#include "Global.h"


// Global Objects defined here:
// Note: do not use the extern keyword to declare them here
//         ~~ This is their definition ~~
//              - This is how you make them accessable in other files without
//                      causing them to be defined twice.

Global g;
Box s_menu_bg;

Global::Global()
{
	xres = 640;
	yres = 480;
}
