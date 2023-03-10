/********************************************************************
*
*		Program: cosmotost
*		Developed by: DAHM
*   	Group 1: (D)aniel, (A)iland, (H)uaiyu, (M)ichael
*		Date Code Originated: 2/17/23
*		Base Code Provided by: Professor Gordon Griesel
*
**********************************************************************/

//                 		INCLUDES

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>


// 					LOCAL LIBRARIES
#include "fonts.h"	// used for fonts
#include "Box.h"	// base box class
#include "Global.h"	// global vars
#include "mkausch.h"	// menu
#include "aparriott.h"	// entity spawning
#include "hzhang.h"	// record
#include "dtorres.h"

using namespace std;


class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	~X11_wrapper();
	X11_wrapper();
	void set_title();
	bool getXPending();
	XEvent getXNextEvent();
	void swapBuffers();
	void reshape_window(int width, int height);
	void check_resize(XEvent *e);
	int check_mouse(XEvent *e);
	int check_keys(XEvent *e);
} x11;

//Function prototypes
void init_opengl(void);
void physics(void);
void render(void);

//=====================================
// MAIN FUNCTION IS HERE
//=====================================
int main()
{
	init_opengl();
	//Main loop
	int k_done = 0, m_done = 0;
	while (!k_done && !m_done) {
		//Process external events.
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			m_done = x11.check_mouse(&e);
			k_done = x11.check_keys(&e);
			if (m_done == 1 || k_done == 1) {
				break;
			}
		}
		physics();
		render();
		x11.swapBuffers();
		usleep(200);
	}
	return 0;
}

X11_wrapper::~X11_wrapper()
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

X11_wrapper::X11_wrapper()
{
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	int w = g.xres, h = g.yres;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		cout << "\n\tcannot connect to X server\n" << endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		cout << "\n\tno appropriate visual found\n" << endl;
		exit(EXIT_FAILURE);
	}
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask =
		ExposureMask | KeyPressMask | KeyReleaseMask |
		ButtonPress | ButtonReleaseMask |
		PointerMotionMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
		InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);


}

void X11_wrapper::set_title()
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "c0sm0t0asT");
}

bool X11_wrapper::getXPending()
{
	//See if there are pending events.
	return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent()
{
	//Get a pending event.
	XEvent e;
	XNextEvent(dpy, &e);
	return e;
}

void X11_wrapper::swapBuffers()
{
	glXSwapBuffers(dpy, win);
}

void X11_wrapper::reshape_window(int width, int height)
{
	//window has been resized.
	g.xres = width;
	g.yres = height;
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
}

void X11_wrapper::check_resize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != g.xres || xce.height != g.yres) {
		//Window size did change.

		reshape_window(xce.width, xce.height);
		// reshape_window(g.xres, g.yres);	// force game to be same size - test

	}
}
//-----------------------------------------------------------------------------

int X11_wrapper::check_mouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	static Box * selection = nullptr;
	static Box * prev_selection = nullptr;

	static unsigned char * prev_color;

	// do nothing with mouse at splash screen
	if (g.state == SPLASH) {

	} else if (g.state == MAINMENU) {
		//Weed out non-mouse events
		if (e->type != ButtonRelease &&
			e->type != ButtonPress &&
			e->type != MotionNotify) {
			//This is not a mouse event that we care about.
			return 0;
		}
		// leave for reference
		if (e->type == ButtonRelease) {
			return 0;
		}
		if (e->type == ButtonPress) {
			if (e->xbutton.button==1) {
				//Left button was pressed.
				//int y = g.yres - e->xbutton.y;
				if (savex != e->xbutton.x || savey != e->xbutton.y) {
					savex = e->xbutton.x;
					savey = e->xbutton.y;
				}

				// check and see if the user clicked on the Menu
				selection = mm.check_t_box(savex, g.yres - savey);
				if (selection && (mm.words[selection->id] == "Start Game")) {
					mm.set_orig_color();
					g.state = GAME;
					// if (g.gameTimer) {
					// 	delete g.gameTimer;
					// }
					g.gameTimer.reset();	// start the game timer
					selection = nullptr;
					prev_selection = nullptr;
					return 0;
				} else if (selection && (mm.words[selection->id] == "High Scores")) {
					mm.set_orig_color();
					g.substate = HIGH_SCORES;
					selection = nullptr;
					prev_selection = nullptr;
					return 0;
				} else if (selection && (mm.words[selection->id] == "Settings")) {
					mm.set_orig_color();
					g.substate = SETTINGS;
					selection = nullptr;
					prev_selection = nullptr;
					return 0;
				} else if (selection && (mm.words[selection->id] == "Quit")) {
					mm.set_orig_color();
					selection = nullptr;
					prev_selection = nullptr;

					return 1;
				}

				return 0;
			}
			// leave for reference
			if (e->xbutton.button==3) {
				//Right button was pressed.
				return 0;
			}
		}
		// look and see
		if (e->type == MotionNotify) {
			//The mouse moved!
			if (savex != e->xbutton.x || savey != e->xbutton.y) {
				savex = e->xbutton.x;
				savey = e->xbutton.y;
				//Code placed here will execute whenever the mouse moves.

				// need to send in flipped y coord because window and
				// mouse coords have different origins
				selection = mm.check_t_box(savex, g.yres - savey);

				if (selection) {
					// cout << "hovering over " << selection->text << endl;
					if (selection != prev_selection) {

						mm.set_highlight(selection);
						prev_selection = selection; // remember selection
						selection = nullptr; // reset selection ptr
					}
				} else {

					mm.set_orig_color();
					prev_selection = nullptr;
				}
			}
		}
	} else if (g.state == PAUSE) {
		if (e->type != ButtonRelease &&
			e->type != ButtonPress &&
			e->type != MotionNotify) {
			//This is not a mouse event that we care about.
			return 0;
		}
		// leave for reference - maybe do the action on the button release
		//          -- and make it blink on the button press?
		if (e->type == ButtonRelease) {
			return 0;
		}
		if (e->type == ButtonPress) {
			if (e->xbutton.button==1) {
				//Left button was pressed.
				//int y = g.yres - e->xbutton.y;
				if (savex != e->xbutton.x || savey != e->xbutton.y) {
					savex = e->xbutton.x;
					savey = e->xbutton.y;
				}

				selection = pause_menu.check_t_box(savex, g.yres - savey);
				if (selection && (pause_menu.words[selection->id] == "Main Menu")) {
					pause_menu.set_orig_color();
					g.state = MAINMENU;
					// if (g.gameTimer) {
					// 	cerr << "killing game clock\n";
					// 	delete g.gameTimer;	// kill game clock
					// }
					g.gameTimer.pause();
					selection = nullptr;
					prev_selection = nullptr;

				} else if (selection && (pause_menu.words[selection->id] == "Start Over")) {
					pause_menu.set_orig_color();
					g.state = MAINMENU;
					g.GameReset();
					g.state = GAME;
					cerr << "g.state was changed back to GAME (RESET SEQUENCE)"
							<< endl;
					// if (g.gameTimer) {
					// 	cerr << "killing game clock\n";
					// 	delete g.gameTimer;	// kill game clock
					// }
					g.gameTimer.reset();
					// g.gameTimer.unPause();
					// g.gameTimer = new Timer();
					selection = nullptr;
					prev_selection = nullptr;

				} else if (selection && (pause_menu.words[selection->id] == "Back to Game")) {
					pause_menu.set_orig_color();
					g.state = GAME;
					if (g.gameTimer.isPaused()) {
						g.gameTimer.unPause();	// unpause the game
					}
					cerr << "g.state was changed back to GAME" << endl;
					selection = nullptr;
					prev_selection = nullptr;

#ifdef USE_OPENAL_SOUND

					sounds.unpause();
					cerr << "unpausing song " << sounds.get_song_name() << endl;
					return 0;

#endif

				} else if (selection && (pause_menu.words[selection->id] == "Quit Game")) {
					pause_menu.set_orig_color();
					cerr << "g.state was changed to should be quitting..." <<
							endl;

					// if (g.gameTimer) {
					// 	cerr << "killing game clock\n";
					// 	delete g.gameTimer;	// kill game clock
					// }

					cerr << "killing game clock\n";
					g.gameTimer.pause();
					selection = nullptr;
					prev_selection = nullptr;
					return 1;
				}

				return 0;
			}
			if (e->xbutton.button==3) {
				//Right button was pressed.
				return 0;
			}
		}
		if (e->type == MotionNotify) {
			//The mouse moved!
			if (savex != e->xbutton.x || savey != e->xbutton.y) {
				savex = e->xbutton.x;
				savey = e->xbutton.y;
				//Code placed here will execute whenever the mouse moves.

				// need to send in flipped y coord because window and
				// mouse coords have different origins
				selection = pause_menu.check_t_box(savex, g.yres - savey);

				if (selection) {
					if (selection != prev_selection) {
						pause_menu.set_highlight(selection);
						prev_selection = selection; // remember selection
						selection = nullptr; // reset selection ptr
					}
				} else {
					// was previously on something and now it's not
					pause_menu.set_orig_color();
					prev_selection = nullptr;

				}
			}
		}

	}
	return 0;

}

int X11_wrapper::check_keys(XEvent *e)
{
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	int key1 = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	if (e->type == KeyRelease) {
		g.keys[key1] = 0;
		// if (key == XK_Shift_L || key == XK_Shift_R)
		// 	shift = 0;
	}
	if (e->type == KeyPress) {
		//std::cout << "press" << std::endl;
		g.keys[key1]=1;
		// if (key == XK_Shift_L || key == XK_Shift_R) {
		// 	shift = 1;
	}

	if (g.state == SPLASH) {

		// only functional keys are escape to quickly quit game and enter
		// to advance to main menu
		if (e->type == KeyPress) {
			switch (key) {
				case XK_Return:
					// Enter was pressed
					g.state = MAINMENU;
					cerr << "g.state was changed to MAINMENU" << endl;
					g.GameReset();
					g.substate = NONE;
					return 0;
				case XK_Escape:
					//Escape key was pressed
					return 1;
			}
		}
		// Game State
		// only functional keys are:
		//     	p: enter Ailand's Entity State
		// Escape: Pauses the game
	} else if ((g.state == MAINMENU) && ((g.substate == SETTINGS) || (g.substate == HIGH_SCORES))) {
		if (e->type == KeyPress) {
			switch (key) {
				case XK_Escape:	// go back to main menu
					//Escape key was pressed
					//Enter Pause State
					g.state = MAINMENU;
					g.GameReset();
					g.substate = NONE;
					cerr << "g.state was changed to MAINMENU" << endl;
					cerr << "g.substate was changed to NONE" << endl;
					cerr << "Back to the main menu" << endl;
					return 0;

#ifdef USE_OPENAL_SOUND
				case XK_u:	// go back to main menu
					//Escape key was pressed
					//Enter Pause State
					sounds.pause();
					cerr << "cycling song to " << sounds.get_song_name() << endl;
					return 0;
#endif

			}
		}
		// Pause Menu State:
		// Only valid key entries are:
		// 		Escape: Leave Pause Menu
		// *** Should be waiting for mouse input on the menu ***
	} else if (g.state == GAME) {
		if (e->type == KeyPress) {
			float dusha = tos.pos[0] + 100*(g.keys[XK_d]-g.keys[XK_a]);
			float dushb = tos.pos[1] + 100*(g.keys[XK_w]-g.keys[XK_s]);
			switch (key) {
				case XK_j:
						// int a = 350;
						tos.pos[0] = dusha;
						tos.pos[1] = dushb;
						// pos[0] = 350;
						// pos[0] += 50;
						// pos[1] = 350;  			// std::cout << "move w"<<pos[1]<<std::endl;
				// std::cout << "move w"<<pos[1]<<std::endl;
						return 0;
				case XK_p: // p was pressed - toggle Ailand's Entity State
					if (g.substate == NONE) {
						g.substate = ENTITY;
						cerr << "g.substate was changed to ENTITY\n";
					} else if (g.substate == ENTITY) {
						g.substate = NONE;
						cerr << "g.substate was changed back to NONE\n";
					}
					return 0;
				case XK_t: // t was pressed - toggle Dan's Feature Mode
					if (g.substate == NONE) {
						g.substate = DTORRES;
						cerr << "g.substate was changed to DTORRES\n";
					} else if (g.substate == DTORRES) {
						g.substate = NONE;
						cerr << "g.substate was changed back to NONE\n";
					}
					return 0;
				case XK_h: // h was pressed - toggle Huaiyu's Feature Mode
					if (g.substate == NONE) {
						g.substate = HUAIYU;
						cerr << "g.substate was changed to HUAIYU\n";
					} else if (g.substate == HUAIYU) {
						g.substate = NONE;
						cerr << "g.substate was changed back to NONE\n";
					}
					return 0;
				case XK_k: // t was pressed - toggle Dan's Feature Mode
					if (g.substate == NONE) {
						g.substate = MIKE;
						cerr << "g.substate was changed to MIKE\n";
					} else if (g.substate == MIKE) {
						g.substate = NONE;
						cerr << "g.substate was changed back to NONE\n";
					}
					return 0;

				case XK_Escape:	// pause the game
					//Escape key was pressed
					//Enter Pause State
					g.state = PAUSE;
					g.gameTimer.pause();
					cerr << "g.state was changed to PAUSE" << endl;

#ifdef USE_OPENAL_SOUND

					sounds.pause();
					cerr << "pausing song " << sounds.get_song_name() << endl;
#endif

					return 0;
				case XK_F1:	// Toggle Help Menu
					g.show_help_menu = (g.show_help_menu ? false : true);
					cerr << "g.show_help_menu was toggled" << endl;
					return 0;
				case XK_F2:	// Toggle Game Over
					g.state = GAMEOVER;
					cerr << "g.state was changed to GAMEOVER" << endl;
					return 0;

#ifdef USE_OPENAL_SOUND
				case XK_u:	// go back to main menu
					//Escape key was pressed
					//Enter Pause State
					sounds.toggle_user_pause();
					cerr << "cycling song to " << sounds.get_song_name() << endl;
					return 0;
#endif


			}
		}
		// Pause Menu State:
		// Only valid key entries are:
		// 		Escape: Leave Pause Menu
		// *** Should be waiting for mouse input on the menu ***
	} else if (g.state == PAUSE) {
		if (e->type == KeyPress) {
			switch (key) {
				case XK_Escape:
					//Escape key was pressed
					//Return to Game State
					g.state = GAME;
					g.gameTimer.unPause();
					cerr << "g.state was changed back to GAME" << endl;

#ifdef USE_OPENAL_SOUND
					sounds.unpause();
					cerr << "unpausing song " << sounds.get_song_name() << endl;
#endif

					return 0;
			}
		}
		return 0;

		// Game Over State:
		// Only valid key entries are:
		// 		Escape: Go back to the main menu
		//		Game Over text with credits rolling?
	} else if (g.state == GAMEOVER) {
		if (tos.score > record.highscore)
		{
				int key = XLookupKeysym(&e->xkey, 0);
				if (e->type == KeyPress) {
						// cout << key << endl;
						if(key >=97 && key <= 122) {
								record.gamer[record.n]= (char)key;
								record.str = record.gamer;
								cout << (char)key << endl;
								cout << record.str << endl;
								if (record.n < 9)
									record.n++;
						}
						if(key == XK_BackSpace) {
								cout << "de" <<endl;
								record.gamer[record.n] = '_';
								if(record.n > 0)
										record.n--;
						}
						if(key == XK_Return) {
							record.ChangeR(tos.score);
							g.state = MAINMENU;
							g.GameReset();
							g.substate = NONE;
							cerr << "g.state was changed to MAINMENU" << endl;
							return 0;
						}
			}
		}
		if (e->type == KeyPress) {
			switch (key) {
				case XK_Escape:
					// Escape key was pressed
					// Go back to the Main Menu
					g.state = MAINMENU;
					g.GameReset();
					g.substate = NONE;
					cerr << "g.state was changed to MAINMENU" << endl;
					return 0;
			}
		}
	}

	return 0;
}

// won't have to mess with this much in the project
void init_opengl(void)
{
	//OpenGL initialization
    // make the viewport the size of the whole window
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
    // most likely this will always be set this way in all our programs
    // vertices are drawn and then filled in
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//Set 2D mode (no perspective)
    // modifies the matrices to be in orthographic mode
    // think of it as a camera looking at things from so far away
    //     that it is in 2 dimensions
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//Set the screen background color
    // makes it almost black

    // initialize fonts

	glEnable(GL_TEXTURE_2D);
	initialize_fonts();

	// set initial game state
	g.state = SPLASH;
	g.substate = NONE;

#ifdef USE_OPENAL_SOUND
	sounds.cycle_songs();	// cycle will start the start song on first play
#endif

}

void physics()
{
	if (g.state == GAME) {
		// ENTITY PHYSICS
		if (g.substate == ENTITY) {
			// Spawnspeed determines how many ticks until spawning another
			// entity
			if (e.spawnSpeed == 0) {
				e.spawnSpeed = 6;
				if (e.chainLen == 0) {
					e.chainLen = e.randnum(4, 12);
					e.curveRandX = e.randnum(-4, 0);
					e.curveRandY = e.randnum(-4, 4);
					e.enterloc = e.randnum(0, 3);
					if (e.enterloc == 0) {
						// makeEntity SPAWN FROM TOP, MOVES LEFT DOWNWARD
						e.spawnX = e.randnum(g.xres / 2, g.xres);
						e.spawnY = g.yres - 5;
						e.spawnVelX = e.randnum(-8, -4);
						e.spawnVelY = e.randnum(-8, 0);
					} else if (e.enterloc <= 2) {
						// makeEntity SPAWN FROM RIGHT, MOVES LEFT, RANDUM UP
						// AND DOWN
						e.spawnX = g.xres;
						e.spawnY = e.randnum(0, g.yres);
						e.spawnVelX = e.randnum(-8, -4);
						e.spawnVelY = e.randnum(-8, 8);
					} else if (e.enterloc == 3) {
						// makeEntity SPAWN FROM BOTTOM, MOVES LEFT AND UP
						e.spawnX = e.randnum(g.xres / 2, g.xres);
						e.spawnY = 5;
						e.spawnVelX = e.randnum(-8, -4);
						e.spawnVelY = e.randnum(0, 8);
					}
				}
				e.makeEntity(e.spawnX, e.spawnY, e.spawnVelX, e.spawnVelY,
							e.curveRandX, e.curveRandY);
				e.chainLen--;
			}
			e.spawnSpeed--;

			for (int i = 0; i < e.numEnt; i++) {
				entity[i].pos[0] += entity[i].vel[0]/2;
				entity[i].pos[1] += entity[i].vel[1]/2;
				entity[i].vel[0] += entity[i].curve[0] / 32;
				entity[i].vel[1] += entity[i].curve[1] / 32;

				if (entity[i].collision(tos)) g.state = GAMEOVER;

				for (int j=0; j < g.n_Bullet; j++) {
					if (entity[i].collision(bul[j])) {
							entity[i].HPdamage(bul[j]);
							bul[j].HPdamage(entity[i]);
							if(entity[i].HP_check()) {
								tos.score += entity[i].point;
								entity[i] = entity[--e.numEnt];
							}
							bul[j] = bul[--g.n_Bullet];
					}
				}

				// DESPAWN
				if (entity[i].pos[1] < -4 ||
						entity[i].pos[1] > g.yres + 4 ||
						entity[i].pos[0] < -4) {
					entity[i] = entity[--e.numEnt];
				}
				// BOUNCE
				if (entity[i].pos[1] <= 4 ||
						entity[i].pos[1] >= g.yres - 4) {
					entity[i].vel[1] = -entity[i].vel[1];
				}
			}
		}

		if (g.substate == MIKE) {
			blocky.move();

			// check toaster collision with blockyforky
			if (blocky.Collison(tos)) {
				tos.HPdamage(blocky);
				blocky.reset();

				if (blocky.HP_check()) {
					blocky.reset();
				}
				if (tos.HP_check()) {
					g.state = GAMEOVER;
				}
			}

			// check blocky's collision with bullets
			for (int j=0; j < g.n_Bullet; j++) {
				if (blocky.Collison(bul[j])) {
						blocky.HPdamage(bul[j]);
						bul[j].HPdamage(blocky);
						if(blocky.HP_check()) {
							tos.score += blocky.point;
							blocky.reset();
						}
						bul[j] = bul[--g.n_Bullet];
				}
			}

			// reset blocky if he's out of screen
			if (blocky.ScreenOut()) {
				blocky.reset();
			}
		}
		if (g.substate == HUAIYU) {
				for (int i=0; i < g.n_Bread; i++) {
						if(bread[i].trace) {
								bread[i].CD--;
								if(0 == bread[i].CD)
										bread[i].trace = false;
						}
				}
				if (g.BreadCD == 0 && (int)rand()%3 == 0)
						make_Bread(g.xres-60.0,(((float)rand()) / (float)RAND_MAX)*g.yres,0.0,3,1);
		}
		// cout << tos.pos[0] << endl;
		// move of toaster
		tos.MoveToster();
		if (g.BulletCD > 0) g.BulletCD--;
		else g.BulletCD=5;
		// auto create bread
		if (g.BreadCD > 0) g.BreadCD--;
		else {
			g.BreadCD=30;
			float alp=(((float)rand()) / (float)RAND_MAX);
			int breadrand = (int)rand()%g.levelchance;
			if(!breadrand==0 && (int)rand()%3 != 0)
					make_Bread(g.xres,alp*g.yres,0.0,1,1);
			else
					make_Bread(g.xres,alp*g.yres,0.0,4,1);
			if(breadrand==0) make_Bread(g.xres,alp*g.yres,0.0,2,1);
		}

		// move of all bullet
		for (int i=0; i < g.n_Bullet; i++) {
			// testing to see if this fixes crash
				if (bul[i].ScreenOut()) bul[i] = bul[--g.n_Bullet];
				bul[i].MoveBullet();
			}
			//move of all bread and check collison with bullet and Toaster
			for (int i=0; i < g.n_Bread; i++) {
					if (bread[i].ScreenOut()) bread[i] = bread[--g.n_Bread];
					// ckeak if collison with toaster
					if (bread[i].Collison(tos)) {
							if (bread[i].item_type == 11 || bread[i].item_type == 13 || bread[i].item_type == 14)	{
									bread[i].HPdamage(tos);
									tos.HPdamage(bread[i]);
									if(bread[i].HP_check())
											bread[i] = bread[--g.n_Bread];
									if(tos.HP_check())
											g.state = GAMEOVER;
							}
							if (bread[i].item_type == 12)	{
								if (tos.b_type < 4) tos.b_type++;
									bread[i] = bread[--g.n_Bread];
							}
							break;
					}
					// ckeak if collison with bullet
					for (int j=0; j < g.n_Bullet; j++) {
							if (bread[i].Collison(bul[j])&&(bread[i].item_type == 11 || bread[i].item_type == 13 || bread[i].item_type == 14)) {
									bread[i].HPdamage(bul[j]);
									bul[j].HPdamage(bread[i]);
									if(bread[i].HP_check()) {
										tos.score += bread[i].point;
										bread[i] = bread[--g.n_Bread];
									}
									bul[j] = bul[--g.n_Bullet];
							}
					}
					if(!bread[i].trace)
							bread[i].MoveBread();
		// time stuff/ change when timer finish
		// for bullet
			}
	}
}

void render()
{
	glClearColor(24.0/255, 38.0/255, 37.0/255, 1.0);	// clear it to a bluish
	glClear(GL_COLOR_BUFFER_BIT);	// clear screen


	if (g.state == SPLASH) {
		Box splash_img;
		splash_img.set_color(61, 90, 115);
		glColor3ubv(splash_img.color);
		splash_img.set_dim(100.0f, 100.0f);
		splash_img.set_pos(g.xres/2.0f, g.yres * (2.0/3.0f), 0);


		/*******************   SPLASH IMAGE PLACEHOLDER   *******************/
		glPushMatrix();
		glTranslatef(splash_img.pos[0], splash_img.pos[1], splash_img.pos[2]);
		glBegin(GL_QUADS);
			glVertex2f(-splash_img.w, -splash_img.h);
			glVertex2f(-splash_img.w,  splash_img.h);
			glVertex2f( splash_img.w,  splash_img.h);
			glVertex2f( splash_img.w, -splash_img.h);
		glEnd();
		glPopMatrix();

		Rect splash_msg;
		splash_msg.bot = splash_img.pos[1];
        splash_msg.left = splash_img.pos[0];
        splash_msg.center = 1;

        ggprint8b(&splash_msg, 0, 0x00ffff00, "Splash Img Placeholder");

		/******************    END SPLASH IMAGE    ***************************/

		Rect game_msg;
		game_msg.bot = g.yres * (1/4.0f);
        game_msg.left = g.xres / 2.0f;
        game_msg.center = 1;

        ggprint8b(&game_msg, 0, 0x00ffffff, "Press Enter to Start the Game");


	} else if (g.state == MAINMENU) {

		// draw main menu
		if (g.substate == NONE)
		{
			mm.draw();
		} else if (g.substate == SETTINGS) {
			Box settings_b;
			settings_b.set_color(61, 90, 115);
			glColor3ubv(settings_b.color);
			settings_b.set_dim(100.0f, 100.0f);
			settings_b.set_pos(g.xres/2.0f, g.yres * (2.0/3.0f), 0);

			glPushMatrix();
			glTranslatef(settings_b.pos[0], settings_b.pos[1], settings_b.pos[2]);
			glBegin(GL_QUADS);
				glVertex2f(-settings_b.w, -settings_b.h);
				glVertex2f(-settings_b.w,  settings_b.h);
				glVertex2f( settings_b.w,  settings_b.h);
				glVertex2f( settings_b.w, -settings_b.h);
			glEnd();
			glPopMatrix();

			Rect settings_msg;
			settings_msg.bot = settings_b.pos[1];
			settings_msg.left = settings_b.pos[0];
			settings_msg.center = 1;

			ggprint8b(&settings_msg, 0, 0x00ffff00, "Settings Img Placeholder");


		}

	} else if (g.state == GAME || g.state == PAUSE ) {
		// State Message

		// Set up and display Information board on bottom of screen.
		// info_board_1 object in global.h ... InfoBoard class in dtorres.h
		info_board_1.set_color(0, 0, 0);
		glColor3ubv(info_board_1.color);
		info_board_1.set_dim(g.xres/2, g.yres/20);
		info_board_1.set_pos(g.xres/2, g.yres/40, 0);
		info_board_1.draw();


		// draw Toaster bullet and bread
		tos.draw();
		tos_health.draw();
		for (int i=0; i < g.n_Bullet; i++) {
				bul[i].draw();
		}
		for (int i=0; i < g.n_Bread; i++) {
			if(!bread[i].trace)
				bread[i].draw();
			else
				bread[i].draw(tos);
	  }
		// ENTITY RENDER
		if (g.substate == ENTITY || g.state == PAUSE) {

			for (int i = 0; i < e.numEnt; i++) {
				glPushMatrix();
				glColor3ubv(entity[i].color);
				glTranslatef(entity[i].pos[0], entity[i].pos[1], 0.0f);
				glBegin(GL_QUADS);
					glVertex2f(-entity[i].dim[0], -entity[i].dim[1]);
					glVertex2f(-entity[i].dim[0],  entity[i].dim[1]);
					glVertex2f( entity[i].dim[0],  entity[i].dim[1]);
					glVertex2f( entity[i].dim[0], -entity[i].dim[1]);
				glEnd();
				glPopMatrix();
			}
		}

		if ((g.substate == MIKE || g.state == PAUSE) && blocky.is_alive()) {
			blocky.draw();
			blocky_health.draw();
		}

		if (g.state == PAUSE) {
			pause_menu.draw();
		}

	} else if (g.state == GAMEOVER) {
		// draw score display
		Box end_img;
		end_img.set_color(61, 90, 115);
		glColor3ubv(end_img.color);
		end_img.set_dim(100.0f, 100.0f);
		end_img.set_pos(g.xres/2.0f, g.yres * (2.0/3.0f), 0);


		/*******************   SPLASH IMAGE PLACEHOLDER   *******************/
		glPushMatrix();
		glTranslatef(end_img.pos[0], end_img.pos[1], end_img.pos[2]);
		glBegin(GL_QUADS);
			glVertex2f(-end_img.w, -end_img.h);
			glVertex2f(-end_img.w,  end_img.h);
			glVertex2f( end_img.w,  end_img.h);
			glVertex2f( end_img.w, -end_img.h);
		glEnd();
		glPopMatrix();

		Rect end_msg;
		end_msg.bot = end_img.pos[1];
        end_msg.left = end_img.pos[0];
        end_msg.center = 1;

        ggprint8b(&end_msg, 0, 0x00ffff00, "End Game Image Placeholder");

		/******************    END SPLASH IMAGE    ***************************/

		Rect game_msg;
		Rect typename_m, hscore_msg;

		typename_m.bot = g.yres * (1/10.0f);
				typename_m.left = g.xres / 2.0f;
				typename_m.center = 1;
		hscore_msg.bot = g.yres * (3/10.0f);
        hscore_msg.left = g.xres / 2.0f;
        hscore_msg.center = 1;
		if(tos.score > record.highscore) {
				game_msg.bot = g.yres * (2/5.0f);
		        game_msg.left = g.xres / 2.0f;
		        game_msg.center = 1;
				ggprint16(&typename_m, 0, 0x00ffffff, "%s", record.gamer);
				ggprint16(&hscore_msg, 0, 0x00DC143C, "New Record: %i", tos.score);
		} else {
				game_msg.bot = g.yres * (1/4.0f);
						game_msg.left = g.xres / 2.0f;
						game_msg.center = 1;

		}
		ggprint16(&game_msg, 0, 0x00ffffff, "GAME OVER");

	}

	// (A) - REMOVED PAUSE FROM IF ELSE STATEMENTS TO ALLOW GAME TO RENDER
	// ENTITES. SEE ELSEIF GAME TO SEE WHAT I MEAN

	// (M) you good, do what you think is best 2.26.23


	/*********************************************************************
	 * 							Help Menu / Text						  *
	 * 							  Contributors: 						  *
	 * 								M. Kausch							  *
	 * 			   					H. Zhang	 						  *
	 * 								D Torres							  *
	 *********************************************************************/

	if (g.show_help_menu == false) {

		Rect help_msg, score, g_time, bullets;

#ifdef USE_OPENAL_SOUND
		Rect s_name;
		s_name.bot = 20;
		s_name.left = g.xres - 300;
		s_name.center = 0;

#endif


		// ***********Locations of all the text rectangles***************
		// 					Top Left side of the screen
		help_msg.bot = (g.yres - 20);
		help_msg.left = 20;
		help_msg.center = 0;
		// 					Bottom left of information board
		score.bot = info_board_1.pos[1] + 15;
		score.left = info_board_1.pos[0] - ((info_board_1.w)/2);
		score.center = 0;
		// 					Bottom left of information board
		//					Time display is relative to the position of the score above
		g_time.bot = score.bot-20;
		g_time.left = score.left;
		g_time.center = 0;

		bullets.bot = g_time.bot-20;
		bullets.left = g_time.left-80;
		bullets.center = 0;


		if (g.state == GAME || g.state == PAUSE) {

			ggprint8b(&help_msg, 0, 0x00ffff00, "Press <F1> for help");
			ggprint8b(&score, 0, 0x00DC143C, "Score : %i",tos.score);
			ggprint8b(&g_time, 0, 0x00DC143C, "Time : %i",(int)g.gameTimer.getTime());
			// ggprint8b(&bullets, 0, 0x00DC143C, "Active bullets : %i",g.n_Bullet);	// debug output

#ifdef USE_OPENAL_SOUND

			if (g.state == GAME)
				ggprint8b(&s_name, 0, 0x00DC143C, "Now Playing: %s",sounds.get_song_name().c_str());
			else
				ggprint8b(&s_name, 0, 0x00DC143C, "Music Paused");
#endif

		}

	} else if (g.show_help_menu == true) {
		const unsigned int KEY_MESSAGES = 11;
		Rect gamestate_msg, key_msg[KEY_MESSAGES], score, g_time, s_name;

		// ***********Locations of all the text rectangles******************
		// 					Top Left side of the screen					//
		gamestate_msg.bot = (g.yres - 20);				// 1st (top)
        gamestate_msg.left = 20;
        gamestate_msg.center = 0;

		// everything's based on where the gamestate message is to
		// easily line it all up
		for (size_t i = 0; i < KEY_MESSAGES; i++) {
			key_msg[i].bot = (gamestate_msg.bot - 40 - i*20);
			key_msg[i].left = gamestate_msg.left;
			key_msg[i].center = gamestate_msg.center;
		}


		// 					Top Right side of the screen				//
		score.bot = g.yres-20;
		score.left = g.xres - 80;
		score.center = 0;

		g_time.bot = score.bot-20;
		g_time.left = score.left;
		g_time.center = 0;

		// bottom right of the screen
#ifdef USE_OPENAL_SOUND
			s_name.bot = 20;
			s_name.left = g.xres - 300;
			s_name.center = 0;

#endif

// 				Record Middle of the screen				//
		Rect typename_m, hscore_msg;
		typename_m.bot = g.yres * (3/5.0f);
				typename_m.left = g.xres / 2.0f;
				typename_m.center = 1;
		hscore_msg.bot = g.yres * (2/5.0f);
        hscore_msg.left = g.xres / 2.0f;
        hscore_msg.center = 1;


		// 					Write Messages Based On State					//
		switch (g.state)
		{
			case SPLASH:
				ggprint8b(&gamestate_msg, 0, 0x00ffff00,
													"STATE - SPLASH SCREEN");
				ggprint8b(&key_msg[0], 0, 0x00ffff00,
														"<ESC> - Exit Game");
				ggprint8b(&key_msg[1], 0, 0x00ffff00,
												"<ENTER> - GO TO MAIN MENU");
#ifdef USE_OPENAL_SOUND
				ggprint8b(&s_name, 0, 0x00DC143C, "Now Playing: %s",sounds.get_song_name().c_str());
#endif
				break;
			case MAINMENU:
				if (g.substate == NONE) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00,
														"STATE - MAIN MENU");
#ifdef USE_OPENAL_SOUND
					ggprint8b(&s_name, 0, 0x00DC143C, "Now Playing: %s",sounds.get_song_name().c_str());
#endif
				} else if (g.substate == SETTINGS) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00,
															"STATE - SETTINGS");
					ggprint8b(&key_msg[0], 0, 0x00ffff00,
												"<ESC> - Back to Main Menu");
				} else if (g.substate == HIGH_SCORES) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00,
															"STATE - High Scores");
					ggprint8b(&key_msg[0], 0, 0x00ffff00,
												"<ESC> - Back to Main Menu");
					if(record.highscore == 0) {
							ggprint16(&hscore_msg, 0, 0x00DC143C,
								 						"No Record Yet!!");
					} else {
							ggprint16(&hscore_msg, 0, 0x00DC143C,
								 						"Score : %i",tos.score);
							ggprint16(&typename_m, 0, 0x00DC143C,
								 						"%s",record.reName);
					}
				}

				break;
			case GAME:
				if (g.substate == NONE) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00, "STATE - GAME");

					ggprint8b(&key_msg[6], 0, 0x00ffff00,
										"<p> - Go To AParriott Feature Mode");
					ggprint8b(&key_msg[7], 0, 0x00ffff00,
										"<h> - Go To HZhang Feature Mode");
					ggprint8b(&key_msg[8], 0, 0x00ffff00,
										"<k> - Go To MKausch Feature Mode");
					ggprint8b(&key_msg[9], 0, 0x00ffff00,
										"<t> - Go To DTorres Feature Mode");
					ggprint8b(&key_msg[10], 0, 0x00ffff00,
										"<u> - Cycle Music");

				} else if (g.substate == ENTITY) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00,
									"STATE - ENTITY - APARRIOTT FEATURE MODE");
					ggprint8b(&key_msg[6], 0, 0x00ffff00,
										"<p> - Go back to Game Mode");
				} else if (g.substate == DTORRES) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00,
									"STATE - DTORRES - DTORRES FEATURE MODE");
					ggprint8b(&key_msg[6], 0, 0x00ffff00,
										"<t> - Go back to Game Mode");
				} else if (g.substate == HUAIYU) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00,
									"STATE - HUAIYU - HZHANG FEATURE MODE");
					ggprint8b(&key_msg[6], 0, 0x00ffff00,
										"<h> - Go back to Game Mode");
				} else if (g.substate == MIKE) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00,
									"STATE - MIKE - MKAUSCH FEATURE MODE");
					ggprint8b(&key_msg[6], 0, 0x00ffff00,
										"<k> - Go back to Game Mode");
				}

				ggprint8b(&key_msg[0], 0, 0x00ffff00,
												"<ESC> - Pause Game");
				ggprint8b(&key_msg[1], 0, 0x00ffff00,
												"<F1> - Minimize Help Menu");
				ggprint8b(&key_msg[2], 0, 0x00ffff00,
												"<w> - Move Up");
				ggprint8b(&key_msg[3], 0, 0x00ffff00,
												"<a> - Move Left");
				ggprint8b(&key_msg[4], 0, 0x00ffff00,
												"<s> - Move Down");
				ggprint8b(&key_msg[5], 0, 0x00ffff00,
												"<d> - Move Right");



				// ggprint8b(&score, 100, 0x00DC143C, "Score");
				ggprint8b(&score, 0, 0x00DC143C, "Score : %i",tos.score);
				ggprint8b(&g_time, 0, 0x00DC143C,
											"Time : %i",(int)g.gameTimer.getTime());
#ifdef USE_OPENAL_SOUND
				if (!sounds.get_pause()) {
					ggprint8b(&s_name, 0, 0x00DC143C, "Now Playing: %s",sounds.get_song_name().c_str());
				} else {
					ggprint8b(&s_name, 0, 0x00DC143C, "Music Paused");
				}
#endif
				break;
			case PAUSE:
				// ggprint8b(&score, 100, 0x00DC143C, "Score");
				ggprint8b(&score, 0, 0x00DC143C, "Score : %i",tos.score);
				ggprint8b(&gamestate_msg, 0, 0x00ffff00, "STATE - PAUSE");
				ggprint8b(&key_msg[0], 0, 0x00ffff00, "<ESC> - Un-Pause Game");
				ggprint8b(&g_time, 0, 0x00DC143C,
											"Time : %i",(int)g.gameTimer.getTime());
#ifdef USE_OPENAL_SOUND
				ggprint8b(&s_name, 0, 0x00DC143C, "Music Paused");
#endif
				break;
			case GAMEOVER:
				// ggprint8b(&score, 100, 0x00DC143C, "Score");
				ggprint8b(&score, 0, 0x00DC143C, "Score : %i",tos.score);
				ggprint8b(&g_time, 0, 0x00DC143C,
											"Time : %i",(int)g.gameTimer.getTime());
				ggprint8b(&gamestate_msg, 0, 0x00ffff00, "STATE - GAMEOVER");
				ggprint8b(&key_msg[0], 0, 0x00ffff00, "<ESC> - Back to Main Menu");
				break;

			default:	// error... shouldn't occur... but if it does *<|:^)
				ggprint8b(&gamestate_msg, 0, 0x00ffff00,
												"STATE: FIX YOUR SHIT BRO");
				break;
		}

	}

}

// Entity functions from aparriott
