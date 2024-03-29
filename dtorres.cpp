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
	
	// Normalize the velocity according to the area
	// Velocity is inversely proportional to the area
	// of the freeze block
	return max_velocity - (((numerator) / (denominator)) * (max_velocity - minimum_velocity));
}
FreezeBlock::FreezeBlock() 
{
	position_set = false;
	pFreezeTimer = NULL;
	pFollowTimer = NULL;
	min_block_dimension = 1;
	max_block_dimension = 30;
	max_velocity = 1.50;//1.25;
	minimum_velocity = 0.05; //0.005;
	melting_rate = 0.1;
	texture = &g.icecube_texture;
}

FreezeBlock::~FreezeBlock()
{
	if (pFreezeTimer) {
		delete pFreezeTimer;
		pFreezeTimer = NULL;
	}
}

void FreezeBlock::draw()
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, *(pfreeze_block->texture));
  	// glColor3ub(color[0], color[1], color[2]);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
  	glTranslatef(pos[0], pos[1], pos[2]);

	glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
  			glVertex2f(-0, -0);
			glTexCoord2f(0.0f, 1.0f);
  			glVertex2f(-0, w);
			glTexCoord2f(1.0f, 1.0f);
  			glVertex2f(h, w);
			glTexCoord2f(1.0f, 0.0f);
  			glVertex2f(h, 0);
  	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
  	glPopMatrix();
}

void FreezeBlock::melt(float melting_rate)
{
	h -= melting_rate;
	w -= melting_rate;
}

void FreezeBlock::reduceVelocity(float velocity_reduction_rate)
{   
	if(velocity_reduction_rate < 0) {
		velocity_reduction_rate *= -1; // absolute value of velocity_reduction_rate
	}

	if(max_velocity > minimum_velocity) {
		max_velocity -= velocity_reduction_rate;
	}
}

void FreezeBlock::setFreezeTimer(int seconds) 
{
	pFreezeTimer = new Timer(seconds);
}

void FreezeBlock::setFollowTimer(int seconds)
{
	pFollowTimer = new Timer(seconds);
}

int FreezeBlock::randomDimension()
{
	return min_block_dimension + rand() % (max_block_dimension - min_block_dimension) + 1;
}

void FreezeBlock::followItem(Item & object)
{	
	// Set the velocity direction of the freeze block depending 
	// on the object's position and set the velocity magnitude
	// depending on the area of the freeze block 
	if (object.pos[0] < pos[0] && object.pos[1] < pos[1]) {
		setVel(-getVelocityConsideringArea(w * h), -getVelocityConsideringArea(w * h), 0);
	}
	else if (object.pos[0] > pos[0] && object.pos[1] > pos[1]) {
		setVel(getVelocityConsideringArea(w * h), getVelocityConsideringArea(w * h), 0);
	}
	else if (object.pos[0] < pos[0] && object.pos[1] > pos[1]) {
		setVel(-getVelocityConsideringArea(w * h), getVelocityConsideringArea(w * h), 0);
	}
	else if(object.pos[0] > pos[0] && object.pos[1] < pos[1]) {
		setVel(getVelocityConsideringArea(w * h), -getVelocityConsideringArea(w * h), 0);
	}
	else if (object.pos[0] == pos[0] && object.pos[1] < pos[1]) {
		setVel(0, -getVelocityConsideringArea(w * h), 0);
	}
	else if (object.pos[0] == pos[0] && object.pos[1] > pos[1]) {
		setVel(0, getVelocityConsideringArea(w * h), 0);
	}
	else if (object.pos[0] < pos[0] && object.pos[1] == pos[1]) {
		setVel(-getVelocityConsideringArea(w * h), 0, 0);
	}
	else if (object.pos[0] > pos[0] && object.pos[1] == pos[1]) {
		setVel(getVelocityConsideringArea(w * h), 0, 0);
	}

	// Update freeze block position
	pos[0] += vel[0];
	pos[1] += vel[1];
}

void FreezeBlock::reduceFreezeArea(float area_reduction_rate)
{
	if(area_reduction_rate < 0) {
		area_reduction_rate *= -1; // absolute value of reduction_rate
	}

	
	if((w > 1) && (h > 1)) {
		w -= area_reduction_rate;
		h -= area_reduction_rate;
		setVel(getVelocityConsideringArea(w * h), getVelocityConsideringArea(w * h), 0);
	}
}

void FreezeBlock::checkBounce()
{	
	// Check if freeze block is going out of bounds
	if(pos[0] + w - 5>= g.xres) {
		pos[0] = g.xres - w;
		vel[0] *= -1;
		setVel(-getVelocityConsideringArea(w * h), vel[1], 0);
	}
	else if(pos[0] - 5<= 0) {
		pos[0] = w;
		vel[0] *= -1;
		setVel(getVelocityConsideringArea(w * h), vel[1], 0);
	}
	else if(pos[1] + h - 5>= g.yres) {
		pos[1] = g.yres - h;
		vel[1] *= -1;
		setVel(vel[0], -getVelocityConsideringArea(w * h), 0);
	}
	else if(pos[1] - 5<= info_board_1.pos[1] + info_board_1.h) {
		vel[1] *= -1;
		setVel(vel[0], getVelocityConsideringArea(w * h), 0);
	}
	
	// Update freeze block position
	pos[0] += vel[0];
	pos[1] += vel[1];
}
void FreezeBlock::setMinMaxBlockDimensions(int min, int max)
{
	if(min < 1 || max > g.yres/3 || max > g.xres/3) {
		min_block_dimension = 1;
		max_block_dimension = 30;
	}
	else {
		min_block_dimension = min;
		max_block_dimension = max;
	}
}


