//Daniel Torres
#include <iostream>
#include "Global.h"
#include "dtorres.h"
#include <GL/glx.h>
#include <unistd.h>

InfoBoard::InfoBoard() 
{
}
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
	ptimer = NULL;
	min_block_dimension = 1;
	max_block_dimension = 30;
}
int FreezeBlock::randomDimension()
{
	int random_dimension = min_block_dimension + rand() % (max_block_dimension - min_block_dimension) + 1;
	return random_dimension;
}
void FreezeBlock::setMinMaxBlockDimensions(int min, int max)
{
	if(min < 1 || max > g.yres/3 || max > g.xres/3) {
		cout << "Invalid block dimensions" << endl;
		usleep(3000000);
	}
	else {
		min_block_dimension = min;
		max_block_dimension = max;
	}
}
FreezeBlock::~FreezeBlock()
{
}

void FreezeBlock::followPlayer(Item & player)
{
	if (player.pos[0] < pos[0] && player.pos[1] < pos[1]) {
		setVel(-1, -1, 0);
		pos[0] += vel[0];
		pos[1] += vel[1];
	}
	else if (player.pos[0] > pos[0] && player.pos[1] > pos[1]) {
		setVel(1, 1, 0);
		pos[0] += vel[0];
		pos[1] += vel[1];
	}
	else if (player.pos[0] < pos[0] && player.pos[1] > pos[1]) {
		setVel(-1, 1, 0);
		pos[0] += vel[0];
		pos[1] += vel[1];
	}
	else if(player.pos[0] > pos[0] && player.pos[1] < pos[1]) {
		setVel(1, -1, 0);
		pos[0] += vel[0];
		pos[1] += vel[1];
	}
	else if (player.pos[0] == pos[0] && player.pos[1] < pos[1]) {
		setVel(0, -1, 0);
		pos[1] += vel[1];
	}
	else if (player.pos[0] == pos[0] && player.pos[1] > pos[1]) {
		setVel(0, 1, 0);
		pos[1] += vel[1];
	}
	else if (player.pos[0] < pos[0] && player.pos[1] == pos[1]) {
		setVel(-1, 0, 0);
		pos[0] += vel[0];
	}
	else if (player.pos[0] > pos[0] && player.pos[1] == pos[1]) {
		setVel(1, 0, 0);
		pos[0] += vel[0];
	}
}
void FreezeBlock::draw()
{
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

void FreezeBlock::setTimer(int seconds) {
	ptimer = new Timer(seconds);
}