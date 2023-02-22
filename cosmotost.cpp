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

using namespace std;

// Global g;
// Box b1;
// extern Box b1;

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
		// reshape_window(g.xres, g.yres);	// force game to be 600x800 - test

	}
}
//-----------------------------------------------------------------------------
#define rnd() ((float)rand() / (float)RAND_MAX)
int randnum(int min, int max) {
    return min + rand() % ((max + 1) - min);
}

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
				if (selection && (selection->text == "Start Game")) {
					mm.set_orig_color();
					g.state = GAME;
					selection = nullptr;
					prev_selection = nullptr;
					return 0;
				} else if (selection && (selection->text == "Settings")) {
					mm.set_orig_color();
					g.substate = SETTINGS;
					selection = nullptr;
					prev_selection = nullptr;
					return 0;
				} else if (selection && (selection->text == "Quit")) {
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
				if (selection && (selection->text == "Main Menu")) {
					pause_menu.set_orig_color();
					g.state = MAINMENU;
					selection = nullptr;
					prev_selection = nullptr;

				} else if (selection && (selection->text == "Start Over")) {
					pause_menu.set_orig_color();
					g.state = MAINMENU;
					g.state = GAME;
					cerr << "g.state was changed back to GAME (RESET SEQUENCE)" 
							<< endl;
					selection = nullptr;
					prev_selection = nullptr;

				} else if (selection && (selection->text == "Back to Game")) {
					pause_menu.set_orig_color();
					g.state = GAME;
					cerr << "g.state was changed back to GAME" << endl;
					selection = nullptr;
					prev_selection = nullptr;
				} else if (selection && (selection->text == "Quit Game")) {
					pause_menu.set_orig_color();
					cerr << "g.state was changed to should be quitting..." << endl;
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

	if (g.state == SPLASH) {

		// only functional keys are escape to quickly quit game and enter
		// to advance to main menu
		if (e->type == KeyPress) { 
			switch (key) {
				case XK_Return:
					// Enter was pressed
					g.state = MAINMENU;
					cerr << "g.state was changed to MAINMENU" << endl;
					return 0;
				case XK_Escape:
					//Escape key was pressed
					return 1;
			}
		}
		// Game State
		// only functional keys are:
		//     	e: enter Ailand's Entity State
		// Escape: Pauses the game
	} else if ((g.state == MAINMENU) && (g.substate == SETTINGS)) {
		if (e->type == KeyPress) { 
			switch (key) {
				case XK_Escape:	// go back to main menu
					//Escape key was pressed
					//Enter Pause State
					g.state = MAINMENU;
					g.substate = NONE;
					cerr << "g.state was changed to MAINMENU" << endl;
					cerr << "g.substate was changed to NONE" << endl;
					cerr << "Back to the main menu" << endl;
					return 0;
			}
		}
		// Pause Menu State:
		// Only valid key entries are:
		// 		Escape: Leave Pause Menu
		// *** Should be waiting for mouse input on the menu ***
	} else if (g.state == GAME) {
		if (e->type == KeyPress) { 
			switch (key) {
				case XK_e: // e was pressed - toggle Ailand's Entity State
					if (g.substate == NONE) {
						g.substate = ENTITY;
						cerr << "g.substate was changed to ENTITY\n";
					} else if (g.substate == ENTITY) {
						g.substate = NONE;
						cerr << "g.substate was changed back to NONE\n";
					}
					return 0;
				case XK_Escape:	// pause the game
					//Escape key was pressed
					//Enter Pause State
					g.state = PAUSE;
					cerr << "g.state was changed to PAUSE" << endl;
					return 0;
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
					//Enter Pause State
					g.state = GAME;
					cerr << "g.state was changed back to GAME" << endl;
					return 0;
			}
		}
		return 0;

		// Game Over State:
		// Only valid key entries are:
		// 		Escape: Go back to the main menu
		//		Game Over text with credits rolling?
	} else if (g.state == GAMEOVER) {
		if (e->type == KeyPress) {
			switch (key) {
				case XK_Escape:
					// Escape key was pressed
					// Go back to the Main Menu
					g.state = MAINMENU;
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

}

void physics()
{

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

			Rect settings_msg, esc_msg;
			settings_msg.bot = settings_b.pos[1];
			settings_msg.left = settings_b.pos[0];
			settings_msg.center = 1;

			esc_msg.bot = g.yres - 20;
			esc_msg.left = 20;
			esc_msg.center = 0;

			ggprint8b(&settings_msg, 0, 0x00ffff00, "Splash Img Placeholder");
			ggprint8b(&esc_msg, 0, 0x00ffff00, "Press Esc to go back");

		}

	} else if (g.state == GAME) {

		// State Message
		Rect game_msg, pause_msg;
		game_msg.bot = (g.yres - 20);
        game_msg.left = 20;
        game_msg.center = 0;

		pause_msg.bot = (g.yres - 30);
        pause_msg.left = 20;
        pause_msg.center = 0;

        ggprint8b(&game_msg, 0, 0x00ffff00, "In Game State");
		ggprint8b(&pause_msg, 0, 0x00ffff00, "Press Escape to Pause");


		// draw score display

	} else if (g.state == PAUSE) {

		// State Message
		Rect game_msg;
		game_msg.bot = (g.yres - 20);
        game_msg.left = 20;
        game_msg.center = 0;

        ggprint8b(&game_msg, 0, 0x00ffff00, "Paused");

		pause_menu.draw();

	} else if (g.state == GAMEOVER) {

	}

}






