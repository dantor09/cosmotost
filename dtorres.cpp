//Daniel Torres
#include <iostream>
#include "Global.h"
#include "dtorres.h"
#include <GL/glx.h>

void InfoBoard::draw()
{
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glBegin(GL_QUADS);
	glVertex2f(-w, -h);
	glVertex2f(-w,  h);
	glVertex2f( w,  h);
	glVertex2f( w, -h);
	glEnd();
	glPopMatrix();
}

FreezeBlock::FreezeBlock() {
	position_set = false;
	timer = NULL;
}
FreezeBlock::~FreezeBlock(){
}
void FreezeBlock::draw() {
	glColor3ubv(color);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glBegin(GL_QUADS);
	glVertex2f(-w, -h);
	glVertex2f(-w,  h);
	glVertex2f( w,  h);
	glVertex2f( w, -h);
	glEnd();
	glPopMatrix();
}
void FreezeBlock::set_timer(int seconds) {
	timer = new Timer(seconds);
}