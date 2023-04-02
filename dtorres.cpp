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

float FreezeBlock::getVelocityConsideringArea(float area) 
{
	float minimum_area = min_block_dimension * min_block_dimension;
	float maximum_area = max_block_dimension * max_block_dimension;
	float numerator = area - minimum_area;
	float denominator = maximum_area - minimum_area;
	float velocity = max_velocity - (((numerator) / (denominator)) * (max_velocity - minimum_velocity));
	
	// Normalize the velocity according to the area
	return max_velocity - (((numerator) / (denominator)) * (max_velocity - minimum_velocity));
}
FreezeBlock::FreezeBlock() {
	position_set = false;
	ptimer = NULL;
	min_block_dimension = 1;
	max_block_dimension = 30;
	max_velocity = 2.5;
	minimum_velocity = 0.05;
	melting_rate = 0.1;
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
	h -= melting_rate;
	w -= melting_rate;
	
	if (player.pos[0] < pos[0] && player.pos[1] < pos[1]) {
		setVel(-getVelocityConsideringArea(w * h), -getVelocityConsideringArea(w * h), 0);
	}
	else if (player.pos[0] > pos[0] && player.pos[1] > pos[1]) {
		setVel(getVelocityConsideringArea(w * h), getVelocityConsideringArea(w * h), 0);
	}
	else if (player.pos[0] < pos[0] && player.pos[1] > pos[1]) {
		setVel(-getVelocityConsideringArea(w * h), getVelocityConsideringArea(w * h), 0);
	}
	else if(player.pos[0] > pos[0] && player.pos[1] < pos[1]) {
		setVel(getVelocityConsideringArea(w * h), -getVelocityConsideringArea(w * h), 0);
	}
	else if (player.pos[0] == pos[0] && player.pos[1] < pos[1]) {
		setVel(0, -getVelocityConsideringArea(w * h), 0);
	}
	else if (player.pos[0] == pos[0] && player.pos[1] > pos[1]) {
		setVel(0, getVelocityConsideringArea(w * h), 0);
	}
	else if (player.pos[0] < pos[0] && player.pos[1] == pos[1]) {
		setVel(-getVelocityConsideringArea(w * h), 0, 0);
	}
	else if (player.pos[0] > pos[0] && player.pos[1] == pos[1]) {
		setVel(getVelocityConsideringArea(w * h), 0, 0);
	}
	
	pos[0] += vel[0];
	pos[1] += vel[1];

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