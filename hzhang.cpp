//huaiyu zhang
void MoveToster(Box &a)
{
	if (g.keys[XK_w]){
			a.pos[1] += 4;
			cout << "move w"<<a.pos[1]<<endl;
	}
	if (g.keys[XK_s]){
			a.pos[1] -= 4;
			cout << "move s"<<a.pos[1]<<endl;
	}
	if (g.keys[XK_a]){
			a.pos[0] -= 4;
			cout << "move a"<<a.pos[0]<<endl;
	}
	if (g.keys[XK_d]){
			a.pos[0] += 4;
			cout << "move d"<<a.pos[0]<<endl;
	}
}
bool check_touch(Box a,Box b)
{
	bool x = (((a.pos[0]+a.w)-(b.pos[0]-b.w))*((a.pos[0]-a.w)-(b.pos[0]+b.w))) < 0;
	bool y = (((a.pos[1]+a.h)-(b.pos[1]-b.h))*((a.pos[1]-a.h)-(b.pos[1]+b.h))) < 0;
	return x&&y;
}
void draw()
{
  glPushMatrix();
  glColor3ub(188, 226, 232);
  glTranslatef(toaster.pos[0], toaster.pos[1], 0.0f);
  glBegin(GL_QUADS);
      glVertex2f(-toaster.w, -toaster.h);
      glVertex2f(-toaster.w,  toaster.h);
      glVertex2f( toaster.w,  toaster.h);
      glVertex2f( toaster.w, -toaster.h);
  glEnd();
}
