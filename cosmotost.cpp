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
#include <iomanip>
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
#include "image.h"

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

// #ifdef USE_OPENAL_SOUND
// void check_sound(void);
// #endif

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
		check_level();
		physics();
		render();
#ifdef USE_OPENAL_SOUND
		check_sound();
#endif
		x11.swapBuffers();
		usleep(2000);
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

	// do nothing with mouse at splash screen
	if (g.state == SPLASH) {

	} else if (g.state == MAINMENU && g.substate == NONE) {
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
					g.gameTimer.reset();	// start the game timer
					selection = nullptr;

#ifdef USE_OPENAL_SOUND
					sounds.boop();
#endif

					return 0;
				} else if (selection && (mm.words[selection->id] == "Debug Mode")) {
					mm.set_orig_color();
					g.state = GAME;
					g.substate = DEBUG;
					g.gameTimer.reset();
					selection = nullptr;
				} else if (selection && (mm.words[selection->id] == "High Scores")) {
					mm.set_orig_color();
					g.substate = HIGH_SCORES;
					selection = nullptr;

#ifdef USE_OPENAL_SOUND
					sounds.boop();
#endif

					return 0;
				} else if (selection && (mm.words[selection->id] == "Settings")) {
					mm.set_orig_color();
					g.substate = SETTINGS;
					selection = nullptr;

#ifdef USE_OPENAL_SOUND
					sounds.boop();
#endif

					return 0;
				} else if (selection && (mm.words[selection->id] == "Quit")) {
					mm.set_orig_color();
					selection = nullptr;

#ifdef USE_OPENAL_SOUND
					sounds.boop();
#endif

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

					
					mm.set_orig_color();
					mm.set_highlight(selection);

#ifdef USE_OPENAL_SOUND
					sounds.beep();
#endif

				}
			}
		}
	} else if (g.state == MAINMENU && g.substate == SETTINGS) {
	
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

					// check and see if the user clicked on slider buttons
					selection = vol_slider.checkButtons(savex, g.yres - savey);
					if (selection && (vol_slider.words[selection->id] == "<")) {
						vol_slider.move_slider_down();

#ifdef USE_OPENAL_SOUND
						sounds.updateMusicVol();
						sounds.boop();

#endif
						// std::cerr << "sound turned down to: " << g.m_vol << endl;

						return 0;
					} else if (selection && (vol_slider.words[selection->id] == ">")) {
						vol_slider.move_slider_up();

#ifdef USE_OPENAL_SOUND
						sounds.updateMusicVol();
						sounds.boop();

#endif
						// std::cerr << "sound turned up to: " << g.m_vol << endl;

						return 0;
					} 

					selection = sfx_slider.checkButtons(savex, g.yres - savey);
					if (selection && (sfx_slider.words[selection->id] == "<")) {
						sfx_slider.move_slider_down();

#ifdef USE_OPENAL_SOUND
						sounds.updateSFXVol();
						sounds.boop();

#endif
						// std::cerr << "sfx turned down to: " << g.sfx_vol << endl;

						return 0;
					} else if (selection && (sfx_slider.words[selection->id] == ">")) {
						sfx_slider.move_slider_up();

#ifdef USE_OPENAL_SOUND
						sounds.updateSFXVol();
						sounds.boop();

#endif
						// std::cerr << "sfx turned down to: " << g.sfx_vol << endl;

						return 0;
					}

					
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
					selection = vol_slider.checkButtons(savex, g.yres - savey);
					if (selection) {
						vol_slider.set_orig_color();
						vol_slider.set_highlight(selection);

#ifdef USE_OPENAL_SOUND
						sounds.beep();
#endif

					} else {
						vol_slider.set_orig_color();
					}

					selection = sfx_slider.checkButtons(savex, g.yres - savey);
					if (selection) {
						sfx_slider.set_orig_color();
						sfx_slider.set_highlight(selection);

#ifdef USE_OPENAL_SOUND
						sounds.beep();
#endif

					} else {
						sfx_slider.set_orig_color();
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
					g.gameTimer.pause();
					selection = nullptr;

#ifdef USE_OPENAL_SOUND
					sounds.boop();
#endif
					return 0;

				} else if (selection && (pause_menu.words[selection->id] == "Start Over")) {
					pause_menu.set_orig_color();
					g.state = MAINMENU;
					g.gameReset();
					g.state = GAME;
					cerr << "g.state was changed back to GAME (RESET SEQUENCE)"
							<< endl;
					g.gameTimer.reset();
					selection = nullptr;

#ifdef USE_OPENAL_SOUND
					sounds.boop();
					sounds.rewind_game_music();
					cerr << "rewinding song " << sounds.get_song_name() << endl;
#endif

					return 0;

				} else if (selection && (pause_menu.words[selection->id] == "Back to Game")) {
					pause_menu.set_orig_color();
					g.state = GAME;
					if (g.gameTimer.isPaused()) {
						g.gameTimer.unPause();	// unpause the game
					}
					cerr << "g.state was changed back to GAME" << endl;
					selection = nullptr;

#ifdef USE_OPENAL_SOUND
					sounds.boop();
					sounds.unpause();
					cerr << "unpausing song " << sounds.get_song_name() << endl;
#endif
					return 0;

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

#ifdef USE_OPENAL_SOUND
					sounds.boop();
#endif

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
						pause_menu.set_orig_color();
						pause_menu.set_highlight(selection);
						// selection = nullptr; // reset selection ptr
#ifdef USE_OPENAL_SOUND
					sounds.beep();
#endif

					// }
				} else {
					// was previously on something and now it's not
					pause_menu.set_orig_color();

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

#ifdef USE_OPENAL_SOUND
		// if (key1 == XK_space) {
		// 	sounds.gun_stop();
		// 	sounds.gun_shooting = false;
		// }

#endif

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
					g.gameReset();
					g.substate = NONE;
					return 0;
				case XK_Escape:
					//Escape key was pressed
					return 1;
				case XK_y:
					// test key
					
					return 0;
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
					g.gameReset();
					g.substate = NONE;
					cerr << "g.state was changed to MAINMENU" << endl;
					cerr << "g.substate was changed to NONE" << endl;
					cerr << "Back to the main menu" << endl;
					return 0;

				case XK_7:

					vol_slider.move_slider_down();

#ifdef USE_OPENAL_SOUND
					sounds.updateMusicVol();
#endif
					// cerr << "sound turned down to: " << g.m_vol << endl;

					// sfx_slider.move_slider_down();
					return 0;
				case XK_8:
					vol_slider.move_slider_up();

#ifdef USE_OPENAL_SOUND
					sounds.updateMusicVol();
#endif
					// cerr << "sound turned up to: " << g.m_vol << endl;
					// sfx_slider.move_slider_up();
					return 0;


				case XK_9:

					sfx_slider.move_slider_down();

#ifdef USE_OPENAL_SOUND
					sounds.updateSFXVol();
#endif
					// cerr << "sfx turned down to: " << g.sfx_vol << endl;

					// sfx_slider.move_slider_down();
					return 0;
				case XK_0:
					sfx_slider.move_slider_up();

#ifdef USE_OPENAL_SOUND
					sounds.updateSFXVol();
#endif
					// cerr << "sfx turned up to: " << g.sfx_vol << endl;
					// sfx_slider.move_slider_up();
					return 0;


// #ifdef USE_OPENAL_SOUND
// 				case XK_u:	// go back to main menu
// 					//Escape key was pressed
// 					//Enter Pause State
// 					sounds.toggle_user_pause();
// 					cerr << "toggling pause"  << endl;
// 					return 0;
// #endif

			}
		}
		// Pause Menu State:
		// Only valid key entries are:
		// 		Escape: Leave Pause Menu
		// *** Should be waiting for mouse input on the menu ***
	} 
	
	
	
	else if (g.state == GAME) {
		if (e->type == KeyPress) {
			float dusha = tos.pos[0] + 150*(g.keys[XK_d]-g.keys[XK_a]);
			float dushb = tos.pos[1] + 150*(g.keys[XK_w]-g.keys[XK_s]);
			switch (key) {
				case XK_j:
						// int a = 350;
						if (tos.energy >= 10) {
								tos.pos[0] = dusha;
								tos.pos[1] = dushb;
								tos.energy -= 10;
#ifdef USE_OPENAL_SOUND
							sounds.doosh();
#endif
						}
						// pos[0] = 350;
						// pos[0] += 50;
						// pos[1] = 350;  			// std::cout << "move w"<<pos[1]<<std::endl;
				// std::cout << "move w"<<pos[1]<<std::endl;
						return 0;
				case XK_e:
						tos.bulletReload();
						return 0;
				case XK_p: // p was pressed - toggle Ailand's Entity State
					// if (g.substate == NONE) {
					if (g.entity_active == false) {
						// g.substate = ENTITY;
						g.entity_active = true;
						cerr << "g.entity_active set to true\n";
					// } else if (g.substate == ENTITY) {
					} else if (g.entity_active == true) {
						// g.substate = NONE;
						g.entity_active = false;
						cerr << "g.entity_active set to false\n";
					}
					return 0;
				case XK_t: // t was pressed - toggle Dan's Feature Mode
					if (g.dtorres_active == false) {
						// g.substate = DTORRES;
						g.dtorres_active = true;
						cerr << "g.dtorres_active set to true\n";
					// } else if (g.substate == DTORRES) {
					} else if (g.dtorres_active == true) {
						// g.substate = NONE;
						g.dtorres_active = false;
						cerr << "g.dtorres_active set to false\n";
					}
					return 0;
				case XK_h: // h was pressed - toggle Huaiyu's Feature Mode
					// if (g.substate == NONE) {
					if (g.huaiyu_active == false) {
						// g.substate = HUAIYU;
						g.huaiyu_active = true;
						cerr << "g.huaiyu_active set to true\n";
					// } else if (g.substate == HUAIYU) {
					} else if (g.huaiyu_active == true) {
						g.huaiyu_active = false;
						// g.substate = NONE;
						cerr << "g.huaiyu_active set to false\n";
					}
					return 0;
				case XK_k: // t was pressed - toggle Dan's Feature Mode
					// if (g.substate == NONE) {
					if (g.mike_active == false) {
						// g.substate = MIKE;
						g.mike_active = true;
						blocky = (blocky == &vblocky) ? &hblocky : &vblocky;
						blocky_health = (blocky_health == &vblocky_health) ? 
															&hblocky_health : &hblocky_health;
						cerr << "g.mike_active set to true\n";
					// } else if (g.substate == MIKE) {
					} else if (g.mike_active == true) {
						g.mike_active = false;
						blocky->gamereset();
						// g.substate = NONE;
						cerr << "g.mike_active set to false\n";
					}
					return 0;

				case XK_q:
					bomb.launch();
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
					cerr << "toggling pause"  << endl;
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
	} else if (g.state == GAMEOVER && g.substate == NONE) {
		// if (tos.score > record.highscore) {
		int key = XLookupKeysym(&e->xkey, 0);
		if (e->type == KeyPress) {
			// cout << key << endl;
			if(key >=97 && key <= 122) {
					record.gamer[record.n]= (char)key;
					// record.str = record.gamer;
					cout << (char)key << endl;
					cout << record.gamer << endl;
					if (record.n < 9)
						record.n++;
			}
			if(key == XK_BackSpace) {
					cout << "<<backspace>>" <<endl;
					record.gamer[record.n] = ' ';
					if(record.n > 0)
							record.n--;
			}
			if(key == XK_Return) {
				record.submitRecord(tos.score);
				// g.state = MAINMENU;
				// g.gameReset();
				g.substate = HIGH_SCORES;
				cerr << "g.substate was changed to HIGH_SCORES" << endl;
				return 0;
			}

			if (key == XK_Escape) {
				// Escape key was pressed
				// Go back to the Main Menu
				// g.state = MAINMENU;
				// g.gameReset();
				g.substate = HIGH_SCORES;
				cerr << "g.substate was changed to HIGH_SCORES" << endl;
				return 0;
			}
		}
	} else if (g.state == GAMEOVER && (g.substate == HIGH_SCORES ||
											g.substate == DEBUG)) {
		// if (tos.score > record.highscore) {
		int key = XLookupKeysym(&e->xkey, 0);
		if (e->type == KeyPress) {
			// cout << key << endl;
			if(key == XK_Return) {
				// g.state = MAINMENU;
				g.gameReset();
				g.state = MAINMENU;
				g.substate = NONE;
				record.makeMenu();
				// record.hs_menu->set_orig_color();
				cerr << "g.state was changed to MAINMENU" << endl;
				return 0;
			}
			if (key == XK_Escape) {
				// Escape key was pressed
				// Go back to the Main Menu
				// g.state = MAINMENU;
				g.gameReset();
				g.state = MAINMENU;
				g.substate = NONE;
				record.makeMenu();
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
	cerr << "starting initializing opengl\n";
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

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);


    // initialize fonts

	glEnable(GL_TEXTURE_2D);
	initialize_fonts();

	// set initial game state
	g.state = SPLASH;
	g.substate = NONE;
	g.level = LEVEL1;
	blocky = &vblocky;
	blocky_health = &vblocky_health;

	/*
		generate and define textures
	*/

	glGenTextures(1, &g.bkg_texture);
	int w = background.width;
    int h = background.height;
    glBindTexture(GL_TEXTURE_2D, g.bkg_texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, background.data);



	w = toaster_img.width;
    h = toaster_img.height;
	// glGenTextures(1, &g.toaster_texture);
    // glBindTexture(GL_TEXTURE_2D, g.toaster_texture);
	glGenTextures(1, &g.toaster_silhouette);
	glBindTexture(GL_TEXTURE_2D, g.toaster_silhouette);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *silhouetteData = buildAlphaData(&toaster_img);
    // glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//     GL_RGB, GL_UNSIGNED_BYTE, toaster_img.data);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	// 	GL_RGBA, GL_UNSIGNED_BYTE, toaster_img.data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
								GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);



	cerr << "finished initializing opengl" << endl;
}


void physics()
{
	if (g.state == SPLASH) {


	}

	if (g.state == GAME) {
		int distanceBread = g.xres;
		int whichBread = -1;
		bool entity_or_tos = true;
		// ENTITY PHYSICS

		if (bomb.is_thrown) {
			bomb.move();
		}

		if (g.entity_active == true) {
			// spawn_speed determines how many ticks until spawning another
			// entity
			if (e.spawn_speed == 0) {
				e.spawn_speed = 6;
				if (e.chain_len == 0) {
					e.chain_len = e.randNum(4, 12);
					e.curve_rand_x = e.randNum(-4, 0);
					e.curve_rand_y = e.randNum(-4, 4);
					e.enter_loc = e.randNum(0, 3);
					if (e.enter_loc == 0) {
						// makeEntity SPAWN FROM TOP, MOVES LEFT DOWNWARD
						e.spawn_x = e.randNum(g.xres / 2, g.xres);
						e.spawn_y = g.yres - 5;
						e.spawn_vel_x = e.randNum(-8, -4);
						e.spawn_vel_y = e.randNum(-8, 0);
					} else if (e.enter_loc <= 2) {
						// makeEntity SPAWN FROM RIGHT, MOVES LEFT, RANDUM UP
						// AND DOWN
						e.spawn_x = g.xres;
						e.spawn_y = e.randNum(0, g.yres);
						e.spawn_vel_x = e.randNum(-8, -4);
						e.spawn_vel_y = e.randNum(-8, 8);
					} else if (e.enter_loc == 3) {
						// makeEntity SPAWN FROM BOTTOM, MOVES LEFT AND UP
						e.spawn_x = e.randNum(g.xres / 2, g.xres);
						e.spawn_y = 5;
						e.spawn_vel_x = e.randNum(-8, -4);
						e.spawn_vel_y = e.randNum(0, 8);
					}
				}
				e.makeEntity(e.spawn_x, e.spawn_y, e.spawn_vel_x, e.spawn_vel_y,
							e.curve_rand_x, e.curve_rand_y);
				e.chain_len--;
			}
			e.spawn_speed--;

			for (int i = 0; i < e.num_ent; i++) {
				if (entity[i].hpCheck()) {
					tos.score += entity[i].point;
					entity[i] = entity[--e.num_ent];
				}
				entity[i].pos[0] += entity[i].vel[0]/2;
				entity[i].pos[1] += entity[i].vel[1]/2;
				entity[i].vel[0] += entity[i].curve[0] / 32;
				entity[i].vel[1] += entity[i].curve[1] / 32;

				
				if (blocky->is_alive() && blocky->explode_done)  {
					if (entity[i].collision(*blocky)) {
						entity[i].hpDamage(*blocky);
						if (entity[i].hpCheck()) {
							entity[i] = entity[--e.num_ent];
						} 
					}
				} else if (!blocky->explode_done) {
					if (blocky->subBoxCollision(entity[i])) {
						entity[i].hpDamage(*blocky);
						if (entity[i].hpCheck()) {
							entity[i] = entity[--e.num_ent];
						} 
					}
				}

				if (entity[i].collision(tos)) {
					entity[i].hpDamage(tos);
					tos.hpDamage(entity[i]);
					if (entity[i].hpCheck()) {
						tos.score += entity[i].point;
						entity[i] = entity[--e.num_ent];
					}

					if (tos.hpCheck() && (tos.lives - 1 > 0)) {
						tos.lives--;
						tos.setHP(80);
					} else if (tos.hpCheck()) {
						g.state = GAMEOVER;
					}

				}

				if (bomb.is_exploding &&
						bomb.collision(entity[i])) {

						entity[i].hp -= entity[i].hp;	// wipe out all health
						tos.score += entity[i].point;
						entity[i] = entity[--e.num_ent];
					
				}


				for (int j=0; j < g.n_Bullet; j++) {
					if (entity[i].collision(bul[j])) {
							entity[i].hpDamage(bul[j]);
							bul[j].hpDamage(entity[i]);
							bul[j] = bul[--g.n_Bullet];
					}
				}


				// DESPAWN
				if (entity[i].pos[1] < -4 ||
						entity[i].pos[1] > g.yres + 4 ||
						entity[i].pos[0] < -4) {
					entity[i] = entity[--e.num_ent];
				}
				// BOUNCE
				if (entity[i].pos[1] <= 4 ||
						entity[i].pos[1] >= g.yres - 4) {
					entity[i].vel[1] = -entity[i].vel[1];
				}
				if (tos.laserCollision(entity[i])) {
						if((entity[i].pos[0] - tos.pos[0] - entity[i].dim[0] - tos.w) < distanceBread) {
								distanceBread = entity[i].pos[0] - tos.pos[0] - entity[i].dim[0] - tos.w;
								whichBread = i;
						}
				}
			}
		}
		if (g.mike_active == true) {
			blocky->move();
			if (tos.laserCollision(*blocky)){
				whichBread = -2;
				distanceBread = blocky->pos[0] - tos.pos[0] - blocky->w - tos.w;
			}
			// check toaster collision with blocky
			if (blocky->collision(tos)) {
				tos.hpDamage(*blocky);
				// blocky->hpDamage(tos);

				// cerr << "resetting blocky..." << endl;
				blocky->reset();

				// if (blocky->hpCheck()) {
				// 	blocky->reset();
				// }

				if (tos.hpCheck() && (tos.lives - 1 > 0)) {
					tos.lives--;
					tos.setHP(80);
				} else if (tos.hpCheck()) {
					g.state = GAMEOVER;
				}


			}


			// check blocky's collision with bullets
			for (int j=0; j < g.n_Bullet; j++) {
				if (blocky->collision(bul[j])) {
						blocky->hpDamage(bul[j]);
						bul[j].hpDamage(*blocky);
						if(blocky->hpCheck()) {
							tos.score += blocky->point;
							blocky->reset();
						}
						bul[j] = bul[--g.n_Bullet];
				}
			}

			// check for blocky's (and sub blockies) collision with enemies
			if (blocky->is_alive() && blocky->explode_done) {
				// blocky's collision with bread
				for (int i = 0; i < g.n_Bread; i++) {
					
					if (bread[i].collision(*blocky)) {
						bread[i].hpDamage(*blocky);
						if (bread[i].hpCheck()) {
							bread[i] = bread[--g.n_Bread];
							// tos.score += bread[i].point;

#ifdef USE_OPENAL_SOUND
							sounds.playZap2();
#endif

						}
					}
				}

				// blocky's collision with entities
				for (int i = 0; i < e.num_ent; i++) {
					if (entity[i].collision(*blocky)) {
						entity[i].hpDamage(*blocky);
						if(entity[i].hpCheck()) {
							// tos.score += entity[i].point;
							entity[i] = entity[--e.num_ent];
#ifdef USE_OPENAL_SOUND
							sounds.playZap2();
#endif
						}
					}
				}

				if (bomb.is_exploding &&
						bomb.collision(*blocky)) {

						blocky->hp = 0;	// wipe out all health
						tos.score += blocky->point;
						blocky->reset();
				}

				

			} else if (!blocky->explode_done) {
				// sub-blocky's collision with bread
				for (int i = 0; i < g.n_Bread; i++) {
					if (blocky->subBoxCollision(bread[i])) {
						bread[i].hpDamage(*blocky);
						if (bread[i].hpCheck()) {
							bread[i] = bread[--g.n_Bread];
							tos.score += bread[i].point;

#ifdef USE_OPENAL_SOUND
							sounds.playZap2();
#endif
						}
					}
				}

				// blocky's collision with entities
				for (int i = 0; i < e.num_ent; i++) {
					if (blocky->subBoxCollision(entity[i])) {
					// if (entity[i].collision(*blocky)) {
						entity[i].hpDamage(*blocky);
						if(entity[i].hpCheck()) {
							tos.score += entity[i].point;
							entity[i] = entity[--e.num_ent];

#ifdef USE_OPENAL_SOUND
							sounds.playZap2();
#endif
						}
					}
				}
			}



		}
		if (g.huaiyu_active == true) {
				for (int i=0; i < g.n_Bread; i++) {
						if(bread[i].trace) {
								bread[i].cd--;
								if(0 == bread[i].cd)
										bread[i].trace = false;
						}
				}
				if (g.BreadCD == 0 && (int)rand()%3 == 0)
						makeBread(g.xres-60.0,(((float)rand()) / (float)RAND_MAX)*g.yres,0.0,3,1);
		}
		// cout << tos.pos[0] << endl;
		// move of toaster
		tos.moveToster();
		if (g.BulletCD > 0) g.BulletCD--;
		else g.BulletCD=5;
		// auto create bread
		if (g.BreadCD > 0) g.BreadCD--;
		else {
			g.BreadCD=30;
			float alp=(((float)rand()) / (float)RAND_MAX);
			int breadrand = (int)rand()%g.levelchance;
			if(breadrand !=0 && (int)rand()%3 != 0)
					makeBread(g.xres,alp*g.yres,0.0,1,1);
			else
					makeBread(g.xres,alp*g.yres,0.0,4,1);
			if(breadrand==0) makeBread(g.xres-10 ,0.5*g.yres,0.0,2,1);
		}

		// move of all bullet
		for (int i=0; i < g.n_Bullet; i++) {
			// testing to see if this fixes crash
				if (bul[i].screenOut()) bul[i] = bul[--g.n_Bullet];
				bul[i].moveBullet();
			}
		//move of all bread and check collision with bullet and Toaster
		for (int i=0; i < g.n_Bread; i++) {
				if (bread[i].screenOut()) {
					bread[i] = bread[--g.n_Bread];
				}
				if (bread[i].hpCheck()) {
					tos.score += bread[i].point;
					bread[i] = bread[--g.n_Bread];

				}
				// ckeak if collision with toaster
				if (bread[i].collision(tos)) {
						if (bread[i].item_type == 11 || bread[i].item_type == 13 || bread[i].item_type == 14)	{
								bread[i].hpDamage(tos);
								tos.hpDamage(bread[i]);
								
								// D.T Reset HP and decrease lives if toaster still has lives left
								if(tos.hpCheck() && (tos.lives - 1 > 0)) {
									tos.lives--;
									tos.setHP(80);
								}
								else if(tos.hpCheck()) {
									g.state = GAMEOVER;
								}
								if(bread[i].hpCheck())
										bread[i] = bread[--g.n_Bread];
						}
						if (bread[i].item_type == 12) {
							if (tos.bullet_type_prime != 4	&&	tos.bullet_type_prime != 8) tos.bullet_type_prime++;
							bread[i] = bread[--g.n_Bread];
						}
						break;
				}
				// ckeak if collision with bullet
				for (int j=0; j < g.n_Bullet; j++) {
					if (bread[i].collision(bul[j])&&(bread[i].item_type == 11 || bread[i].item_type == 13 || bread[i].item_type == 14)) {
						bread[i].hpDamage(bul[j]);
						bul[j].hpDamage(bread[i]);
						bul[j] = bul[--g.n_Bullet];
						if(bread[i].hpCheck()) {
							tos.score += bread[i].point;
							bread[i] = bread[--g.n_Bread];
						}
					}

				}


				if (bomb.is_exploding && 
						(bomb.collision(bread[i]))) {
						bread[i].hp = 0;
						tos.score += bread[i].point;
						bread[i] = bread[--g.n_Bread];
				}


				if(!bread[i].trace)
						bread[i].moveBread();
				if(tos.laserCollision(bread[i])&& !bread[i].hpCheck()) {
						if((bread[i].pos[0] - tos.pos[0] - bread[i].w - tos.w) < distanceBread) {
								distanceBread = bread[i].pos[0] - tos.pos[0] - bread[i].w - tos.w;
								whichBread = i;
								entity_or_tos = false;
						}
				}
		// time stuff/ change when timer finish
		// for bullet
		}
		tos.setDistance(distanceBread);
		if(tos.laserOn)  {
			if (whichBread == -2) {
					tos.laserDamage(*blocky);
					cerr << "distanceBread: " << distanceBread << " whichBread " << whichBread << endl;
			} 
			else if (whichBread != -1 && entity_or_tos) {
					tos.laserDamage(entity[whichBread]);
        			cerr << "distanceBread: " << distanceBread << " whichBread " << whichBread << endl;
			}
			else if (whichBread != -1 && !entity_or_tos) {
					tos.laserDamage(bread[whichBread]);
        			cerr << "distanceBread: " << distanceBread << " whichBread " << whichBread << endl;
			}

		}
	}
}

void render()
{
	glClearColor(24.0/255, 38.0/255, 37.0/255, 1.0);	// clear it to a bluish
	glClear(GL_COLOR_BUFFER_BIT);	// clear screen


	/*
		render background
	*/

	glBindTexture(GL_TEXTURE_2D, g.bkg_texture);

    glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0,  g.xres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres,  g.yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);



	if (g.state == SPLASH) {
		Box splash_img;
		splash_img.setColor(61, 90, 115);
		glColor3ubv(splash_img.color);
		splash_img.setDim(100.0f, 100.0f);
		splash_img.setPos(g.xres/2.0f, g.yres * (2.0/3.0f), 0);


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
			// Box settings_b;
			// settings_b.setColor(61, 90, 115);
			// glColor3ubv(settings_b.color);
			// settings_b.setDim(100.0f, 100.0f);
			// settings_b.setPos(g.xres/2.0f, g.yres * (2.0/3.0f), 0);

			// glPushMatrix();
			// glTranslatef(settings_b.pos[0], settings_b.pos[1], settings_b.pos[2]);
			// glBegin(GL_QUADS);
			// 	glVertex2f(-settings_b.w, -settings_b.h);
			// 	glVertex2f(-settings_b.w,  settings_b.h);
			// 	glVertex2f( settings_b.w,  settings_b.h);
			// 	glVertex2f( settings_b.w, -settings_b.h);
			// glEnd();
			// glPopMatrix();

			// Rect settings_msg;
			// settings_msg.bot = settings_b.pos[1];
			// settings_msg.left = settings_b.pos[0];
			// settings_msg.center = 1;

			// ggprint8b(&settings_msg, 0, 0x00ffff00, "Settings Img Placeholder");

			vol_slider.draw();
			sfx_slider.draw();


		} else if (g.substate == HIGH_SCORES) {

			record.hs_menu->draw();

		}

	} else if (g.state == GAME || g.state == PAUSE ) {

		

		// State Message

		// Set up and display Information board on bottom of screen.
		// info_board object in global.h ... InfoBoard class in dtorres.h
		info_board_1.setColor(0, 0, 0);
		glColor3ubv(info_board_1.color);
		info_board_1.setDim(g.xres/2, g.yres/20);
		info_board_1.setPos(g.xres/2, g.yres/40, 0);
		info_board_1.draw();

		
		


		// draw Toaster bullet and bread
		tos.tdraw();
		tos_health.draw();
		// cerr << "gonna draw..." << endl;
		tos_cd.draw();
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
		// if (g.substate == ENTITY || g.state == PAUSE) {
		if (g.entity_active == true &&
			(g.state == GAME || g.state == PAUSE)) {

			for (int i = 0; i < e.num_ent; i++) {
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

		// if ((g.substate == MIKE || g.state == PAUSE) &&
		// 	(blocky.is_alive() || !blocky.explode_done)) {
		if ((g.mike_active == true &&
			(g.state == GAME || g.state == PAUSE)) &&
			(blocky->is_alive() || !blocky->explode_done)) {

			blocky->draw();
			blocky_health->draw();
		}

		if (g.state == PAUSE) {
			pause_menu.draw();
		}

		// if(g.substate == DTORRES) {
		if(g.dtorres_active == true &&
		   (g.state == GAME || g.state == PAUSE)) {

			if (pfreeze_block == NULL) {
				try {
					pfreeze_block = new FreezeBlock;
					pfreeze_block->setColor(162, 210, 223); // Sky blue
					pfreeze_block->setMinMaxBlockDimensions(40, 80); // set min and max freeze block dimensions
					pfreeze_block->w = pfreeze_block->randomDimension(); // random width
					pfreeze_block->h = pfreeze_block->randomDimension(); // random height

					// set position of freeze block to random location on screen
					// ensure freeze block is within screen bounds
					pfreeze_block->setPos(pfreeze_block->w + (rand() % (int)(g.xres - pfreeze_block->w)), 
										  (pfreeze_block->h + (info_board_1.h * 2)) + (rand() % (int)(g.yres - pfreeze_block->h)), 0);
					pfreeze_block->position_set = true;
				} 
				catch (bad_alloc) {
					cerr << "Freeze Block Bad Allocation" << endl;
					exit(EXIT_FAILURE);
				}
			}

			// Follow the item passed into followItem() 
			// Freeze block could be set to follow any Item object
			if(pfreeze_block->position_set && tos.disable_keys == false) {	
				pfreeze_block->followItem(tos);
				pfreeze_block->melt(0.25);
				pfreeze_block->draw();
			}

			// Freeze the toster as soon as collision is detected and freeze for 
			// pre-determined amount of seconds
			if(pfreeze_block->collision(tos) && !tos.disable_keys) {
				tos.disable_keys = true;
				pfreeze_block->position_set = false;
				pfreeze_block->setFreezeTimer(2);
			}

			// Unfreeze the toaster after timer is done or freeze block melted away
			if(tos.disable_keys && pfreeze_block->ptimer->isDone() 
			   || pfreeze_block -> h <= 0
			   || pfreeze_block -> w <= 0 ) {

				delete pfreeze_block->ptimer;
				pfreeze_block->ptimer = NULL;
				delete pfreeze_block;
				pfreeze_block = NULL;
				tos.disable_keys = false;
			}
		}

		bomb.draw();


	} else if (g.state == GAMEOVER && g.substate == NONE) {

		Rect game_msg;
		Rect typename_m, hscore_msg;

		game_msg.bot = g.yres / 2.0f;
		game_msg.left = g.xres / 2.0f;
		game_msg.center = 1;

		hscore_msg.bot = game_msg.bot - 40;
        hscore_msg.left = game_msg.left;
        hscore_msg.center = 1;

		typename_m.bot = hscore_msg.bot - 40;
		typename_m.left = hscore_msg.left;
		typename_m.center = 1;

		glPushMatrix();
		glTranslatef(typename_m.left, typename_m.bot, 0);
		glColor3ub(255, 0, 0);
		glBegin(GL_QUADS);
			glVertex2f(-75, 0);
			glVertex2f(75, 0);
			glVertex2f(75, 4);
			glVertex2f(-75, 4);
		glEnd();
		glPopMatrix();

		ggprint16(&typename_m, 0, 0x00ffffff, "%s", record.gamer);
		ggprint16(&hscore_msg, 0, 0x00DC143C, "New Record: %i", tos.score);
		ggprint16(&game_msg, 0, 0x00ffffff, "GAME OVER");



	} else if (g.state == GAMEOVER && g.substate == HIGH_SCORES) {
	

		record.hs_menu->draw();

	} else if (g.state == GAMEOVER && g.substate == DEBUG) {
	

		record.hs_menu->draw();

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

		Rect help_msg, score, g_time, bullets, lvl;

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

		bullets.bot = score.bot;
		bullets.left = score.left-80;
		bullets.center = 0;

		lvl.bot = bullets.bot-20;
		lvl.left = bullets.left;
		lvl.center = 0;


		if (g.state == GAME || g.state == PAUSE) {

			ggprint8b(&help_msg, 0, 0x00ffff00, "Press <F1> for help");
			ggprint8b(&score, 0, 0x00DC143C, "Score : %i",tos.score);
			ggprint8b(&g_time, 0, 0x00DC143C, "Time : %d %s : %d %s",g.gameTimer.getTime('m')," m", g.gameTimer.getTime('s'), " s");
			ggprint8b(&lvl, 0, 0x00DC143C, "Level %i", g.level);
			// ggprint8b(&bullets, 0, 0x00DC143C, "Active bullets : %i",g.n_Bullet);	// debug output

#ifdef USE_OPENAL_SOUND

			if (g.state == GAME)
				ggprint8b(&s_name, 0, 0x00DC143C, "Now Playing: %s",sounds.get_song_name().c_str());
			else
				ggprint8b(&s_name, 0, 0x00DC143C, "Music Paused");
#endif

		}

	} else if (g.show_help_menu == true) {
		const unsigned int KEY_MESSAGES = 13;
		Rect gamestate_msg, key_msg[KEY_MESSAGES], score, g_time, s_name, bullets, lvl;

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

		bullets.bot = g_time.bot-20;
		bullets.left = g_time.left-80;
		bullets.center = 0;

		lvl.bot = bullets.bot-20;
		lvl.left = bullets.left;
		lvl.center = 0;

		// bottom right of the screen
#ifdef USE_OPENAL_SOUND
			s_name.bot = 20;
			s_name.left = g.xres - 300;
			s_name.center = 0;

#endif




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
					// if(record.highscore == 0) {
					// 		ggprint16(&hscore_msg, 0, 0x00DC143C,
					// 			 						"No Record Yet!!");
					// } else {
					// 		ggprint16(&hscore_msg, 0, 0x00DC143C,
					// 			 						"Score : %i",tos.score);
					// 		// ggprint16(&typename_m, 0, 0x00DC143C,
					// 		// 	 						"%s",record.reName);
					// }
				}

				break;
			case GAME:
				if (g.substate == NONE) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00, "STATE - GAME");

					ggprint8b(&key_msg[8], 0, 0x00ffff00,
										"<p> - Go To AParriott Feature Mode");
					ggprint8b(&key_msg[9], 0, 0x00ffff00,
										"<h> - Go To HZhang Feature Mode");
					ggprint8b(&key_msg[10], 0, 0x00ffff00,
										"<k> - Go To MKausch Feature Mode");
					ggprint8b(&key_msg[11], 0, 0x00ffff00,
										"<t> - Go To DTorres Feature Mode");
					ggprint8b(&key_msg[12], 0, 0x00ffff00,
										"<u> - Cycle Music");

				// } else if (g.substate == ENTITY) {
				} else if (g.entity_active == true) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00,
									"STATE - ENTITY - APARRIOTT FEATURE MODE");
					ggprint8b(&key_msg[8], 0, 0x00ffff00,
										"<p> - Go back to Game Mode");
				// } else if (g.substate == DTORRES) {
				} else if (g.dtorres_active == true) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00,
									"STATE - DTORRES - DTORRES FEATURE MODE");
					ggprint8b(&key_msg[8], 0, 0x00ffff00,
										"<t> - Go back to Game Mode");
				// } else if (g.substate == HUAIYU) {
				} else if (g.dtorres_active == true) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00,
									"STATE - HUAIYU - HZHANG FEATURE MODE");
					ggprint8b(&key_msg[8], 0, 0x00ffff00,
										"<h> - Go back to Game Mode");
				// } else if (g.substate == MIKE) {
				} else if (g.mike_active == true) {
					ggprint8b(&gamestate_msg, 0, 0x00ffff00,
									"STATE - MIKE - MKAUSCH FEATURE MODE");
					ggprint8b(&key_msg[8], 0, 0x00ffff00,
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
				ggprint8b(&key_msg[6], 0, 0x00ffff00,
												"<spacebar> - Shoot");
				ggprint8b(&key_msg[7], 0, 0x00ffff00,
												"<j> - Dodge");


				// ggprint8b(&score, 100, 0x00DC143C, "Score");
				ggprint8b(&score, 0, 0x00DC143C, "Score : %i",tos.score);
				ggprint8b(&g_time, 0, 0x00DC143C,
											"Time : %d %s : %d %s",g.gameTimer.getTime('m')," m", g.gameTimer.getTime('s'), " s");
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
											"Time : %d %s : %d %s",g.gameTimer.getTime('m')," m", g.gameTimer.getTime('s'), " s");
#ifdef USE_OPENAL_SOUND
				ggprint8b(&s_name, 0, 0x00DC143C, "Music Paused");
#endif
				break;
			case GAMEOVER:
				ggprint8b(&score, 100, 0x00DC143C, "Score");
				ggprint8b(&score, 0, 0x00DC143C, "Score : %i",tos.score);
				ggprint8b(&g_time, 0, 0x00DC143C,
											"Time : %d %s : %d %s",g.gameTimer.getTime('m')," m", g.gameTimer.getTime('s'), " s");
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
