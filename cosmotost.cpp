/* Ailand Parriott
 * created: 23.02.17
 * 3350
 * Cosmotost entity maker
 */

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

using namespace std;

class Global {
    public:
	int xres, yres;
	int mousex, mousey;
    int numEnt;

    Global() {
        xres = 1280;
        yres = 720;
    }
} g;

const int MAX_ENTITIES = 128;

class Entity {
	public:
	float dim[2];
	float vel[2];
	float pos[2];
    float curve[2];
	unsigned char color[3];
	/*void set_color(unsigned char col[3]) {
		memcpy(color,  col, sizeof(unsigned char) * 3);
	}*/

	Entity() {
		dim[0] = 4;
		dim[1] = 4;
		pos[0] = g.xres / 2;
		pos[1] = g.yres / 2;
		vel[0] = -1;
		vel[1] = -1;
        curve[0] = 0;
        curve[1] = 0;
	}

	Entity(float wid, float hgt, float posX, float posY, float v0, float v1, float curveX, float curveY) {
		dim[0] = wid;
		dim[1] = hgt;
		pos[0] = posX;
		pos[1] = posY;
		vel[0] = v0;
		vel[1] = v1;
        curve[0] = curveX;
        curve[1] = curveY;
	}
} entity[MAX_ENTITIES];

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
	void check_mouse(XEvent *e);
	int check_keys(XEvent *e);
} x11;

//Function prototypes
void init_opengl(void);
void physics(void);
void render(void);

//=====================================
// MAIN FUNCTION IS HERE
//=====================================
int main() {
	init_opengl();

	//Main loop
	int done = 0;
	while (!done) {
		//Process external events.
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			x11.check_mouse(&e);
			done = x11.check_keys(&e);
		}
		physics();
		render();
		x11.swapBuffers();
		usleep(200);
	}

	return 0;
}

#define rnd() ((float)rand() / (float)RAND_MAX)
int randnum(int min, int max) {
    return min + rand() % ((max + 1) - min);
}

void makeEntity(float posX, float posY, float initVelX, float initVelY, float curveX, float curveY) {
	if (g.numEnt < MAX_ENTITIES) {
		entity[g.numEnt].dim[0] = 4;
		entity[g.numEnt].dim[1] = 4;
		entity[g.numEnt].pos[0] = posX;
		entity[g.numEnt].pos[1] = posY;
		entity[g.numEnt].vel[0] = initVelX;
		entity[g.numEnt].vel[1] = initVelY;
		entity[g.numEnt].color[0] = randnum(80, 120);
		entity[g.numEnt].color[1] = randnum(100, 180);
		entity[g.numEnt].color[2] = randnum(200, 255);
        entity[g.numEnt].curve[0] = curveX;
        entity[g.numEnt].curve[1] = curveY;
		g.numEnt++;
	}	
}

X11_wrapper::~X11_wrapper() {
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

X11_wrapper::X11_wrapper() {
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

void X11_wrapper::set_title() {
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Cosmotost");
}

bool X11_wrapper::getXPending() {
    //See if there are pending events.
    return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent() {
    //Get a pending event.
    XEvent e;
    XNextEvent(dpy, &e);
    return e;
}

void X11_wrapper::swapBuffers() {
    glXSwapBuffers(dpy, win);
}

void X11_wrapper::reshape_window(int width, int height) {
    //window has been resized.
    g.xres = width;
    g.yres = height;
    //
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
}

void X11_wrapper::check_resize(XEvent *e) {
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
        return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != g.xres || xce.height != g.yres) {
        //Window size did change.
        reshape_window(xce.width, xce.height);
    }
}
//-------------------------------------------------------------------------

void X11_wrapper::check_mouse(XEvent *e) {
    static int savex = 0;
    static int savey = 0;
    //Weed out non-mouse events
    if (e->type != ButtonRelease &&
            e->type != ButtonPress &&
            e->type != MotionNotify) {
        //This is not a mouse event that we care about.
        return;
    }
    //
    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button was pressed.
            //int y = g.yres - e->xbutton.y;
            return;
        }
        if (e->xbutton.button==3) {
            //Right button was pressed.
            //makeEntity(e->xbutton.x, g.yres - e->xbutton.y);
            return;
        }
    }
    if (e->type == MotionNotify) {
        //The mouse moved!
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
            /*savex = e->xbutton.x;
            savey = e->xbutton.y;
            //Code placed here will execute whenever the mouse moves.
            makeEntity(e->xbutton.x, g.yres - e->xbutton.y);
            */
        }
    }
}

int X11_wrapper::check_keys(XEvent *e) {
    if (e->type != KeyPress && e->type != KeyRelease) {
        return 0;
    }
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyPress) {
        switch (key) {
            case XK_1:
                //Key 1 was pressed
                break;
            case XK_Escape:
                //Escape key was pressed
                return 1;
        }
    }
    return 0;
}

void init_opengl(void) {
    //OpenGL initialization
    glViewport(0, 0, g.xres, g.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //Set the screen background box.color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    // TEXT ENABLE
    glEnable(GL_TEXTURE_2D);
}

int entityEnter, chainNum = 0, everyOther = 0;
float spawnX, spawnY, spawnVelX, spawnVelY, curveRandX = 0, curveRandY = 0;

void physics() {
    // makeEntity(int posX, int posY,  int initVelX, int initVelY) 

    // could make it gen a random number, check if it is greater than 0, then count down each time??
    if (everyOther == 0) {
        everyOther = 4;
        if (chainNum == 0) {
            chainNum = randnum(4, 12);
            curveRandX = randnum(-4, 0);
            curveRandY = randnum(-4, 4);

            entityEnter = randnum(0, 3);
            if (entityEnter == 0) {
                // makeEntity SPAWN FROM TOP, MOVES LEFT DOWNWARD
                spawnX = randnum(g.xres / 2, g.xres);
                spawnY = g.yres - 5;
                spawnVelX = randnum(-8, -4);
                spawnVelY = randnum(-8, 0);
            } else if (entityEnter <= 2) {
                // makeEntity SPAWN FROM RIGHT, MOVES LEFT, RANDUM UP AND DOWN
                spawnX = g.xres;
                spawnY = randnum(0, g.yres);
                spawnVelX = randnum(-8, -4);
                spawnVelY = randnum(-8, 8);
            } else if (entityEnter == 3) {
                // makeEntity SPAWN FROM BOTTOM, MOVES LEFT AND UP
                spawnX = randnum(g.xres / 2, g.xres);
                spawnY = 5;
                spawnVelX = randnum(-8, -4);
                spawnVelY = randnum(0, 8);
            }
        }
        makeEntity(spawnX, spawnY, spawnVelX, spawnVelY, curveRandX, curveRandY);

        chainNum--;
    }
    everyOther--;

	for (int i = 0; i < g.numEnt; i++) {
        entity[i].pos[0] += entity[i].vel[0];
		entity[i].pos[1] += entity[i].vel[1];

        entity[i].vel[0] += entity[i].curve[0] / 16;
        entity[i].vel[1] += entity[i].curve[1] / 16;

        // DESPAWN
        if (entity[i].pos[1] < -4 || 
                entity[i].pos[1] > g.yres + 4 ||
                entity[i].pos[0] < -4) {	
            entity[i] = entity[--g.numEnt];
        }

        // BOUNCE
		if (entity[i].pos[1] <= 4 ||
				entity[i].pos[1] >= g.yres - 4) {			
			entity[i].vel[1] = -entity[i].vel[1];
        }
    }

}

void render() {

	glClear(GL_COLOR_BUFFER_BIT);   

	//Draw entoty.
	for (int i = 0; i < g.numEnt; i++) {
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






