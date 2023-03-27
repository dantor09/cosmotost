// Modified by: Michael Kausch
// Modified On: 3/22/23
// Lab 9 continuation 
// image.cpp - implementation of the Image class


//Original author:  Gordon Griesel
//Original date:    2014 - 2021


#include "image.h"

Image::~Image() 
{ 
    delete [] data; 
}

Image::Image(const char *fname) 
{
    if (fname[0] == '\0')
	return;
    int ppmFlag = 0;
    char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    char ppmname[80];
    if (strncmp(name+(slen-4), ".ppm", 4) == 0)
	ppmFlag = 1;
    if (ppmFlag) {
	strcpy(ppmname, name);
    } else {
	name[slen-4] = '\0';
	sprintf(ppmname,"%s.ppm", name);
	char ts[100];
	sprintf(ts, "convert %s %s", fname, ppmname);
	system(ts);
    }
    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
	char line[200];
	fgets(line, 200, fpi);
	fgets(line, 200, fpi);
	//skip comments and blank lines
	while (line[0] == '#' || strlen(line) < 2)
	    fgets(line, 200, fpi);
	sscanf(line, "%i %i", &width, &height);
	fgets(line, 200, fpi);
	//get pixel data
	int n = width * height * 3;			
	data = new unsigned char[n];			
	for (int i=0; i<n; i++)
	    data[i] = fgetc(fpi);
	fclose(fpi);
    } else {
	printf("ERROR opening image: %s\n",ppmname);
	exit(0);
    }
    if (!ppmFlag)
	unlink(ppmname);
}

unsigned char *buildAlphaData(Image *img)
{
	//Add 4th component to an RGB stream...
	//RGBA
	//When you do this, OpenGL is able to use the A component to determine
	//transparency information.
	//It is used in this application to erase parts of a texture-map from view.
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		//-----------------------------------------------
		//get largest color component...
		//*(ptr+3) = (unsigned char)((
		//		(int)*(ptr+0) +
		//		(int)*(ptr+1) +
		//		(int)*(ptr+2)) / 3);
		//d = a;
		//if (b >= a && b >= c) d = b;
		//if (c >= a && c >= b) d = c;
		//*(ptr+3) = d;
		//-----------------------------------------------
		//this code optimizes the commented code above.
		//code contributed by student: Chris Smith
		//
		*(ptr+3) = (a|b|c);
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}